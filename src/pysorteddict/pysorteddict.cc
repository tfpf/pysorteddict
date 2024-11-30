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
 *
 * @param ob Python object.
 *
 * @return Pair of a string and a stringification success flag.
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
 *
 * @param ob Python object.
 *
 * @return Pair of a string and a stringification success flag.
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

    // The type of each key.
    PyObject *key_type = nullptr;

    // These methods are named after the Python functions they emulate.
    bool is_type_key_type(PyObject *);
    PyObject *getitem(PyObject *);
    int setitem(PyObject *, PyObject*);
};
// clang-format on

/**
 * Check whether a Python object has the correct type for use as a key. If not,
 * set a Python exception.
 *
 * @param ob Python object.
 *
 * @return `true` if its type is the same as the key type, else `false`.
 */
bool SortedDictType::is_type_key_type(PyObject* ob)
{
    if (Py_IS_TYPE(ob, reinterpret_cast<PyTypeObject*>(this->key_type)) != 0)
    {
        return true;
    }
    PyObject* key_type_repr = PyObject_Repr(this->key_type);  // New reference.
    if (key_type_repr == nullptr)
    {
        return false;
    }
    PyErr_Format(PyExc_TypeError, "key must be of type %s", PyUnicode_AsUTF8(key_type_repr));
    Py_DECREF(key_type_repr);
    return false;
}

/**
 * Find the value mapped to a key without checking the type of the key. If not
 * found, set a Python exception.
 *
 * @param key Key.
 *
 * @return Value if found, else `nullptr`.
 */
PyObject* SortedDictType::getitem(PyObject* key)
{
    auto it = this->map->find(key);
    if (it == this->map->end())
    {
        PyErr_SetObject(PyExc_KeyError, key);
        return nullptr;
    }
    return Py_NewRef(it->second);
}

/**
 * Map a value to a key or remove a key-value pair without checking the type of
 * the key. If not removed when removal was requested, set a Python exception.
 *
 * @param key Key.
 * @param value Value.
 *
 * @return 0 if mapped or removed, else -1.
 */
int SortedDictType::setitem(PyObject* key, PyObject* value)
{
    // Insertion will be faster if the approximate location is known. Hence,
    // look for the nearest match.
    auto it = this->map->lower_bound(key);
    bool found = it != this->map->end() && !this->map->key_comp()(key, it->first);

    // Remove the key-value pair.
    if (value == nullptr)
    {
        if (!found)
        {
            PyErr_SetObject(PyExc_KeyError, key);
            return -1;
        }
        Py_DECREF(it->first);
        Py_DECREF(it->second);
        this->map->erase(it);
        return 0;
    }

    // Map the value to the key. This merely stores additional references to
    // the key (if applicable) and the value. If I ever plan to allow mutable
    // types as keys, I should store references to their copies instead. Like
    // the C++ standard library containers do.
    if (!found)
    {
        // Create a new key-value pair. The hint is correct; the key will get
        // inserted just before it.
        it = this->map->emplace_hint(it, key, value);
        Py_INCREF(it->first);
    }
    else
    {
        // Replace the previously-mapped value.
        Py_DECREF(it->second);
        it->second = value;
    }
    Py_INCREF(it->second);
    return 0;
}

/**
 * Deinitialise and deallocate.
 */
static void sorted_dict_type_dealloc(PyObject* self)
{
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
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

    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
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
    if (PyObject_RichCompareBool(sd->key_type, reinterpret_cast<PyObject*>(&PyLong_Type), Py_EQ) != 1)
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
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
    return sd->map->size();
}

/**
 * Find the value mapped to a key.
 */
static PyObject* sorted_dict_type_getitem(PyObject* self, PyObject* key)
{
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
    if (!sd->is_type_key_type(key))
    {
        return nullptr;
    }
    return sd->getitem(key);
}

/**
 * Map a value to a key or remove a key-value pair.
 */
static int sorted_dict_type_setitem(PyObject* self, PyObject* key, PyObject* value)
{
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
    if (!sd->is_type_key_type(key))
    {
        return -1;
    }
    return sd->setitem(key, value);
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
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
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

PyDoc_STRVAR(
    sorted_dict_type_items_doc,
    "d.items() -> list[tuple[object, object]]\n"
    "Create and return a new list containing the key-value pairs in the sorted dictionary ``d``. "
    "This list will be sorted."
);

static PyObject* sorted_dict_type_items(PyObject* self, PyObject* args)
{
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
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

PyDoc_STRVAR(
    sorted_dict_type_keys_doc,
    "d.keys() -> list[object]\n"
    "Create and return a new list containing the keys in the sorted dictionary ``d``. This list will be sorted."
);

static PyObject* sorted_dict_type_keys(PyObject* self, PyObject* args)
{
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
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

PyDoc_STRVAR(
    sorted_dict_type_values_doc,
    "d.values() -> list[object]\n"
    "Create and return a new list containing the values in the sorted dictionary ``d``. "
    "This list will be sorted by the keys which the values are mapped to."
);

static PyObject* sorted_dict_type_values(PyObject* self, PyObject* args)
{
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
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
        "items",                     // ml_name
        sorted_dict_type_items,      // ml_meth
        METH_NOARGS,                 // ml_flags
        sorted_dict_type_items_doc,  // ml_doc
    },
    {
        "keys",                     // ml_name
        sorted_dict_type_keys,      // ml_meth
        METH_NOARGS,                // ml_flags
        sorted_dict_type_keys_doc,  // ml_doc
    },
    {
        "values",                     // ml_name
        sorted_dict_type_values,      // ml_meth
        METH_NOARGS,                  // ml_flags
        sorted_dict_type_values_doc,  // ml_doc
    },
    {
        nullptr,
    }
};
// clang-format on

PyDoc_STRVAR(
    sorted_dict_type_doc,
    "SortedDict(key_type: type) -> SortedDict\n"
    "Create a new sorted dictionary in which the keys are of type ``key_type``. "
    "As of the current version, ``key_type`` must be ``int``. Support for some more types will be added in due course."
);

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
    sorted_dict_type_doc,                   // tp_doc
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

PyDoc_STRVAR(
    sorted_dict_module_doc,
    "Provides ``SortedDict``, which is a Python sorted dictionary: "
    "a Python dictionary in which the keys are always in ascending order."
);

// clang-format off
static PyModuleDef sorted_dict_module = {
    PyModuleDef_HEAD_INIT,   // m_base
    "pysorteddict",          // m_name
    sorted_dict_module_doc,  // m_doc
    -1,                      // m_size
    nullptr,                 // m_methods
    nullptr,                 // m_slots
    nullptr,                 // m_traverse
    nullptr,                 // m_clear
    nullptr,                 // m_free
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
    if (PyModule_AddObjectRef(mod, "SortedDict", reinterpret_cast<PyObject*>(&sorted_dict_type)) < 0)
    {
        Py_DECREF(mod);
        return nullptr;
    }
    return mod;
}
