#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <utility>

/**
 * C++-style comparison implementation for Python objects.
 */
struct PyObject_CustomCompare
{
    bool operator()(PyObject* a, PyObject* b) const
    {
        // There must exist a total order on the set of possible keys. (Else,
        // this comparison may error out.) Hence, only instances of the type
        // passed to the constructor may be used as keys. (Instances of types
        // derived from that type are not allowed, because comparisons between
        // them may error out. See the constructor code.) With these
        // precautions, this comparison should always work.
        return PyObject_RichCompareBool(a, b, Py_LT) == 1;
    }
};

/**
 * Obtain the Python representation of a Python object.
 */
static std::pair<std::string, bool> repr(PyObject* ob)
{
    PyObject* ob_repr = PyObject_Repr(ob);  // New reference.
    if (ob_repr == nullptr)
    {
        return { "", false };
    }
    std::pair<std::string, bool> result = { PyUnicode_AsUTF8(ob_repr), true };
    Py_DECREF(ob_repr);
    return result;
}

/**
 * Obtain a human-readable string representation of a Python object.
 */
static std::pair<std::string, bool> str(PyObject* ob)
{
    PyObject* ob_str = PyObject_Str(ob);  // New reference.
    if (ob_str == nullptr)
    {
        return { "", false };
    }
    std::pair<std::string, bool> result = { PyUnicode_AsUTF8(ob_str), true };
    Py_DECREF(ob_str);
    return result;
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
    // Up to Python 3.12, the argument parser below took an array of pointers
    // (with each pointer pointing to a C string) as its fourth argument.
    // However, C++ does not allow converting a string constant to a pointer.
    // Hence, I use a character array to construct the C string, and then place
    // it in an array of pointers.
    char key_type[] = "key_type";
    char* args_names[] = { key_type, nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|", args_names, &sd->key_type))
    {
        Py_DECREF(self);
        return nullptr;
    }

    // Check the type to use for keys.
    if (PyObject_RichCompareBool(sd->key_type, (PyObject*)&PyLong_Type, Py_EQ) != 1)
    {
        PyErr_SetString(PyExc_TypeError, "constructor argument must be a supported type");
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
    if (Py_IS_TYPE(key, (PyTypeObject*)sd->key_type) == 0)
    {
        PyObject* key_type_repr = PyObject_Repr(sd->key_type);  // New reference.
        if (key_type_repr == nullptr)
        {
            return nullptr;
        }
        PyErr_Format(PyExc_TypeError, "key must be of type %s", PyUnicode_AsUTF8(key_type_repr));
        Py_DECREF(key_type_repr);
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
    if (Py_IS_TYPE(key, (PyTypeObject*)sd->key_type) == 0)
    {
        PyObject* key_type_repr = PyObject_Repr(sd->key_type);  // New reference.
        if (key_type_repr == nullptr)
        {
            return -1;
        }
        PyErr_Format(PyExc_TypeError, "key must be of type %s", PyUnicode_AsUTF8(key_type_repr));
        Py_DECREF(key_type_repr);
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
        PyObject* key_str = PyObject_Str(item.first);  // New reference.
        if (key_str == nullptr)
        {
            return nullptr;
        }
        PyObject* value_str = PyObject_Str(item.second);  // New reference.
        if (value_str == nullptr)
        {
            Py_DECREF(key_str);
            return nullptr;
        }
        oss << delimiter << PyUnicode_AsUTF8(key_str) << ": " << PyUnicode_AsUTF8(value_str);
        delimiter = actual_delimiter;
        Py_DECREF(key_str);
        Py_DECREF(value_str);
    }
    oss << '\x7d';
    std::string oss_str = oss.str();
    return PyUnicode_FromStringAndSize(oss_str.data(), oss_str.size());  // New reference.
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
        if (pyitem == nullptr)
        {
            Py_DECREF(pyitems);
            return nullptr;
        }
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
