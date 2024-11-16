#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iterator>
#include <map>
#include <sstream>

/**
 * C++-style comparison implementation for Python objects.
 */
struct ComparePyObjects
{
    bool operator()(PyObject* a, PyObject* b) const
    {
        // This assumes that the comparison operation will never error out. I
        // think it should be enough to ensure that the two Python objects
        // being compared always have the same type.
        return PyObject_RichCompareBool(a, b, Py_LT) == 1;
    }
};

/**
 * Set an error message containing the string representation of a Python
 * object.
 */
static void PyErr_FormatWrapper(PyObject* exc, char const* fmt, PyObject* ob)
{
    PyObject* repr = PyObject_Repr(ob);  // New reference.
    // The second argument is no longer a string constant. Is there an elegant
    // fix?
    PyErr_Format(PyExc_ValueError, fmt, PyUnicode_AsUTF8(repr));
    Py_DECREF(repr);
}

// clang-format off
struct SortedDictType
{
    PyObject_HEAD
    // Pointer to an object on the heap. Can't be the object itself, because
    // this container will be allocated a definite amount of space, which won't
    // allow the object to grow.
    std::map<PyObject *, PyObject*, ComparePyObjects> *map = nullptr;
    // The type of the keys of the dictionary.
    PyObject *key_type = nullptr;
};
// clang-format on

/**
 * Deinitialise and deallocate.
 */
static void sorted_dict_type_dealloc(PyObject* self)
{
    SortedDictType* sd = (SortedDictType*)self;
    Py_DECREF(sd->key_type);
    delete sd->map;
    Py_TYPE(self)->tp_free(self);
}

/**
 * Allocate and initialise.
 */
static PyObject* sorted_dict_type_new(PyTypeObject* type, PyObject* args, PyObject* kwargs)
{
    PyObject* self = type->tp_alloc(type, 0);  // New reference.
    if (self == nullptr)
    {
        return nullptr;
    }

    SortedDictType* sd = (SortedDictType*)self;
    // Casting a string constant to a non-const pointer is not permitted in
    // C++, but the signature of this function is such that I am forced to.
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|", (char*[]) { "key_type", nullptr }, &sd->key_type))
    {
        Py_DECREF(self);
        return nullptr;
    }

    // Check the type to use for keys.
    if (PyObject_RichCompareBool(sd->key_type, (PyObject*)&PyLong_Type, Py_EQ) != 1)
    {
        PyErr_SetString(PyExc_ValueError, "constructor argument must be a supported type");
        Py_DECREF(self);
        return nullptr;
    }

    sd->map = new std::map<PyObject*, PyObject*, ComparePyObjects>;
    Py_INCREF(sd->key_type);
    return self;
}

/**
 * Query the value at a key.
 */
static PyObject* sorted_dict_type_getitem(PyObject* self, PyObject* key)
{
    SortedDictType* sd = (SortedDictType*)self;
    if (PyObject_IsInstance(key, sd->key_type) != 1)
    {
        PyErr_FormatWrapper(PyExc_ValueError, "key must be of type %s", sd->key_type);
        return nullptr;
    }
    auto it = sd->map->find(key);
    if (it == sd->map->end())
    {
        PyErr_FormatWrapper(PyExc_KeyError, "%s", sd->key_type);
        return nullptr;
    }
    return it->second;
}

/**
 * Assign the value at a key.
 */
static int sorted_dict_type_setitem(PyObject* self, PyObject* key, PyObject* value)
{
    SortedDictType* sd = (SortedDictType*)self;
    if (PyObject_IsInstance(key, sd->key_type) != 1)
    {
        PyErr_FormatWrapper(PyExc_ValueError, "key must be of type %s", sd->key_type);
        return -1;
    }

    // Remove the key.
    auto it = sd->map->find(key);
    if (value == nullptr)
    {
        if (it == sd->map->end())
        {
            PyErr_FormatWrapper(PyExc_KeyError, "%s", sd->key_type);
            return -1;
        }
        Py_DECREF(it->first);
        Py_DECREF(it->second);
        sd->map->erase(it);
        return 0;
    }

    // Insert or replace the value.
    if (it == sd->map->end())
    {
        auto status = sd->map->insert({ key, value });
        it = status.first;
        Py_INCREF(it->first);
    }
    else
    {
        Py_DECREF(it->second);
        it->second = value;
    }
    Py_INCREF(it->second);
    return 0;
}

static PyMappingMethods sorted_dict_type_mapping = {
    .mp_subscript = sorted_dict_type_getitem,
    .mp_ass_subscript = sorted_dict_type_setitem,
};

/**
 * Stringify.
 */
static PyObject* sorted_dict_type_str(PyObject* self)
{
    SortedDictType* sd = (SortedDictType*)self;
    std::ostringstream oss;
    char const* delimiter = "";
    char const* actual_delimiter = ", ";
    oss << '\x7b';
    for (auto it = sd->map->begin(); it != sd->map->end(); it = std::next(it))
    {
        PyObject* key_repr = PyObject_Repr(it->first);  // New reference.
        PyObject* value_repr = PyObject_Repr(it->second);  // New reference.
        oss << delimiter << PyUnicode_AsUTF8(key_repr) << ": " << PyUnicode_AsUTF8(value_repr);
        delimiter = actual_delimiter;
        Py_DECREF(key_repr);
        Py_DECREF(value_repr);
    }
    oss << '\x7d';
    return PyUnicode_FromString(oss.str().data());
}

// clang-format off
static PyTypeObject sorted_dict_type = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0)
    .tp_name = "pysorteddict.SortedDict",
    .tp_basicsize = sizeof(SortedDictType),
    .tp_itemsize = 0,
    .tp_dealloc = sorted_dict_type_dealloc,
    // .tp_repr = sorted_dict_type_repr,
    .tp_as_mapping = &sorted_dict_type_mapping,
    .tp_hash = PyObject_HashNotImplemented,
    .tp_str = sorted_dict_type_str,
    // .tp_getattro =  PyObject_GenericGetAttr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    // .tp_methods = sorted_dict_type_methods,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = sorted_dict_type_new,
    .tp_free = PyObject_Del,
};
// clang-format on

static PyModuleDef sorted_dict_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "pysorteddict",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_pysorteddict(void)
{
    if (PyType_Ready(&sorted_dict_type) < 0)
    {
        return nullptr;
    }
    PyObject* mod = PyModule_Create(&sorted_dict_module);
    if (mod == nullptr)
    {
        return nullptr;
    }
    if (PyModule_AddObjectRef(mod, "SortedDict", (PyObject*)&sorted_dict_type) < 0)
    {
        Py_DECREF(mod);
        return nullptr;
    }
    return mod;
}
