#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iterator>
#include <map>
#include <sstream>

/**
 * C++-style comparison implementation for Python objects.
 */
struct PyObject_CustomCompare
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
    PyErr_Format(exc, fmt, PyUnicode_AsUTF8(repr));
    Py_DECREF(repr);
}

// clang-format off
struct SortedDictType
{
    PyObject_HEAD
    // Pointer to an object on the heap. Can't be the object itself, because
    // this container will be allocated a definite amount of space, which won't
    // allow the object to grow.
    std::map<PyObject *, PyObject*, PyObject_CustomCompare> *map = nullptr;
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
    Py_XDECREF(sd->key_type);
    if (sd->map != nullptr)
    {
        for (auto& item : *sd->map)
        {
            Py_DECREF(item.first);
            Py_DECREF(item.second);
        }
    }
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
    char* args_names[] = { "key_type", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|", args_names, &sd->key_type))
    {
        Py_DECREF(self);
        return nullptr;
    }

    // Check the type to use for keys.
    if (PyObject_RichCompareBool(sd->key_type, (PyObject*)&PyLong_Type, Py_EQ) != 1)
    {
        PyErr_SetString(PyExc_ValueError, "constructor argument must be a supported type");
        // I haven't increased its reference count, so the deallocator
        // shouldn't decrease it. Hence, set it to a null pointer.
        sd->key_type = nullptr;
        Py_DECREF(self);
        return nullptr;
    }

    sd->map = new std::map<PyObject*, PyObject*, PyObject_CustomCompare>;
    Py_INCREF(sd->key_type);
    return self;
}

/**
 * Obtain the number of keys.
 */
static Py_ssize_t sorted_dict_type_len(PyObject* self)
{
    SortedDictType* sd = (SortedDictType*)self;
    return sd->map->size();
}

/**
 * Query the value at a key.
 */
static PyObject* sorted_dict_type_getitem(PyObject* self, PyObject* key)
{
    SortedDictType* sd = (SortedDictType*)self;
    if (PyObject_IsInstance(key, sd->key_type) != 1)
    {
        PyErr_FormatWrapper(PyExc_TypeError, "key must be of type %s", sd->key_type);
        return nullptr;
    }
    auto it = sd->map->find(key);
    if (it == sd->map->end())
    {
        PyErr_SetObject(PyExc_KeyError, key);
        return nullptr;
    }
    return Py_NewRef(it->second);
}

/**
 * Assign the value at a key.
 */
static int sorted_dict_type_setitem(PyObject* self, PyObject* key, PyObject* value)
{
    SortedDictType* sd = (SortedDictType*)self;
    if (PyObject_IsInstance(key, sd->key_type) != 1)
    {
        PyErr_FormatWrapper(PyExc_TypeError, "key must be of type %s", sd->key_type);
        return -1;
    }

    // Remove the key.
    auto it = sd->map->find(key);
    if (value == nullptr)
    {
        if (it == sd->map->end())
        {
            PyErr_SetObject(PyExc_KeyError, key);
            return -1;
        }
        Py_DECREF(it->first);
        Py_DECREF(it->second);
        sd->map->erase(it);
        return 0;
    }

    // Insert or replace the value. This merely stores additional references to
    // the key (if applicable) and the value. If I ever plan to allow mutable
    // types as keys, I should store references to their copies instead. Like
    // the C++ standard library containers do.
    if (it == sd->map->end())
    {
        it = sd->map->insert({ key, value }).first;
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

// clang-format off
static PyMappingMethods sorted_dict_type_mapping = {
    sorted_dict_type_len,      // mp_length
    sorted_dict_type_getitem,  // mp_subscript
    sorted_dict_type_setitem,  // mp_ass_subscript
};
// clang-format on

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
    for (auto& item : *sd->map)
    {
        PyObject* key_repr = PyObject_Repr(item.first);  // New reference.
        PyObject* value_repr = PyObject_Repr(item.second);  // New reference.
        oss << delimiter << PyUnicode_AsUTF8(key_repr) << ": " << PyUnicode_AsUTF8(value_repr);
        delimiter = actual_delimiter;
        Py_DECREF(key_repr);
        Py_DECREF(value_repr);
    }
    oss << '\x7d';
    return PyUnicode_FromString(oss.str().data());  // New reference.
}

/**
 * Create a list containing pairs of the keys and values in the dictionary.
 */
static PyObject* sorted_dict_type_items(PyObject* self, PyObject* args)
{
    SortedDictType* sd = (SortedDictType*)self;
    PyObject* pyitems = PyList_New(sd->map->size());  // New reference.
    if (pyitems == nullptr)
    {
        return nullptr;
    }
    Py_ssize_t idx = 0;
    for (auto& item : *sd->map)
    {
        PyObject* pyitem = PyTuple_New(2);  // New reference.
        PyTuple_SET_ITEM(pyitem, 0, item.first);
        Py_INCREF(item.first);
        PyTuple_SET_ITEM(pyitem, 1, item.second);
        Py_INCREF(item.second);
        PyList_SET_ITEM(pyitems, idx++, pyitem);
    }
    return pyitems;
}

/**
 * Create a list containing the keys in the dictionary.
 */
static PyObject* sorted_dict_type_keys(PyObject* self, PyObject* args)
{
    SortedDictType* sd = (SortedDictType*)self;
    PyObject* pykeys = PyList_New(sd->map->size());  // New reference.
    if (pykeys == nullptr)
    {
        return nullptr;
    }
    Py_ssize_t idx = 0;
    for (auto& item : *sd->map)
    {
        PyList_SET_ITEM(pykeys, idx++, item.first);
        Py_INCREF(item.first);
    }
    return pykeys;
}

/**
 * Create a list containing the values in the dictionary.
 */
static PyObject* sorted_dict_type_values(PyObject* self, PyObject* args)
{
    SortedDictType* sd = (SortedDictType*)self;
    PyObject* pyvalues = PyList_New(sd->map->size());  // New reference.
    if (pyvalues == nullptr)
    {
        return nullptr;
    }
    Py_ssize_t idx = 0;
    for (auto& item : *sd->map)
    {
        PyList_SET_ITEM(pyvalues, idx++, item.second);
        Py_INCREF(item.second);
    }
    return pyvalues;
}

// clang-format off
static PyMethodDef sorted_dict_type_methods[] = {
    {
        "items",                 // ml_name
        sorted_dict_type_items,  // ml_meth
        METH_NOARGS,             // ml_flags
        nullptr,                 // ml_doc
    },
    {
        "keys",                 // ml_name
        sorted_dict_type_keys,  // ml_meth
        METH_NOARGS,            // ml_flags
        nullptr,                // ml_doc
    },
    {
        "values",                 // ml_name
        sorted_dict_type_values,  // ml_meth
        METH_NOARGS,              // ml_flags
        nullptr,                  // ml_doc
    },
    {
        nullptr,
    }
};
// clang-format on

// clang-format off
static PyTypeObject sorted_dict_type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)  // ob_base
    "SortedDict",                           // tp_name
    sizeof(SortedDictType),                 // tp_basicsize
    0,                                      // tp_itemsize
    sorted_dict_type_dealloc,               // tp_dealloc
    0,                                      // tp_vectorcall_offset
    nullptr,                                // tp_getattr
    nullptr,                                // tp_setattr
    nullptr,                                // tp_as_async
    nullptr,                                // tp_repr
    nullptr,                                // tp_as_number
    nullptr,                                // tp_as_sequence
    &sorted_dict_type_mapping,              // tp_as_mapping
    PyObject_HashNotImplemented,            // tp_hash
    nullptr,                                // tp_call
    sorted_dict_type_str,                   // tp_str
    nullptr,                                // tp_getattro
    nullptr,                                // tp_setattro
    nullptr,                                // tp_as_buffer
    Py_TPFLAGS_DEFAULT,                     // tp_flags
    nullptr,                                // tp_doc
    nullptr,                                // tp_traverse
    nullptr,                                // tp_clear
    nullptr,                                // tp_richcompare
    0,                                      // tp_weaklistoffset
    nullptr,                                // tp_iter
    nullptr,                                // tp_iternext
    sorted_dict_type_methods,               // tp_methods
    nullptr,                                // tp_members
    nullptr,                                // tp_getset
    nullptr,                                // tp_base
    nullptr,                                // tp_dict
    nullptr,                                // tp_descr_get
    nullptr,                                // tp_descr_set
    0,                                      // tp_dictoffset
    nullptr,                                // tp_init
    PyType_GenericAlloc,                    // tp_alloc
    sorted_dict_type_new,                   // tp_new
    PyObject_Del,                           // tp_free
    nullptr,                                // tp_is_gc
    nullptr,                                // tp_bases
    nullptr,                                // tp_mro
    nullptr,                                // tp_cache
    nullptr,                                // tp_subclasses
    nullptr,                                // tp_weaklist
    nullptr,                                // tp_del
};
// clang-format on

// clang-format off
static PyModuleDef sorted_dict_module = {
    PyModuleDef_HEAD_INIT,  // m_base
    "pysorteddict",         // m_name
    nullptr,                // m_doc
    -1,                     // m_size
    nullptr,                // m_methods
    nullptr,                // m_slots
    nullptr,                // m_traverse
    nullptr,                // m_clear
    nullptr,                // m_free
};
// clang-format on

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
