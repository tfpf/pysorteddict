#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iterator>
#include <map>
#include <memory>
#include <string>

#define LEFT_CURLY_BRACKET "\u007B"
#define RIGHT_CURLY_BRACKET "\u007D"

/**
 * C++-style clean-up implementation for Python objects.
 */
struct PyObject_Delete
{
    void operator()(PyObject* ob)
    {
        Py_XDECREF(ob);
    }
};

using PyObjectWrapper = std::unique_ptr<PyObject, PyObject_Delete>;

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

struct SortedDictType
{
    PyObject_HEAD;

    // Pointer to an object on the heap. Can't be the object itself, because
    // this container will be allocated a definite amount of space, which won't
    // allow the object to grow.
    std::map<PyObject*, PyObject*, PyObject_CustomCompare>* map;

    // The type of each key.
    PyObject* key_type;

    void deinit(void);
    bool is_key_type_set(bool);
    bool can_use_as_key(PyObject*, bool);
    bool are_key_type_and_key_valid(PyObject*, bool);
    PyObject* repr(void);
    int contains(PyObject*);
    PyObject* getitem(PyObject*);
    int setitem(PyObject*, PyObject*);
    PyObject* clear(void);
    PyObject* copy(void);
    int init(PyObject*, PyObject*);
    static PyObject* New(PyTypeObject*, PyObject*, PyObject*);
};

void SortedDictType::deinit(void)
{
    Py_XDECREF(this->key_type);
    for (auto& item : *this->map)
    {
        Py_DECREF(item.first);
        Py_DECREF(item.second);
    }
    delete this->map;
}

/**
 * Check whether the key type of this sorted dictionary is set.
 *
 * @param raise Whether to set a Python exception if the check fails.
 *
 * @return `true` if the check succeeds, else `false`.
 */
bool SortedDictType::is_key_type_set(bool raise)
{
    if (this->key_type != nullptr)
    {
        return true;
    }
    if (raise)
    {
        PyErr_SetString(PyExc_ValueError, "key type not set");
    }
    return false;
}

/**
 * Check whether the given object can be used as a key in this sorted
 * dictionary.
 *
 * The caller must ensure that the key type of this sorted dictionary is set
 * prior to calling this method.
 *
 * @param raise Whether to set a Python exception if the check fails.
 *
 * @return `true` if the check succeeds, else `false`.
 */
bool SortedDictType::can_use_as_key(PyObject* ob, bool raise)
{
    if (Py_IS_TYPE(ob, reinterpret_cast<PyTypeObject*>(this->key_type)) != 0)
    {
        return true;
    }
    if (raise)
    {
        PyErr_SetString(PyExc_TypeError, "key is of wrong type");
    }
    return false;
}

/**
 * Check whether the key type of this sorted dictionary is set and whether the
 * given object can be used as a key in this sorted dictionary.
 *
 * @param ob Python object.
 * @param raise Whether to set a Python exception if the check fails.
 *
 * @return `true` if the check succeeds, else `false`.
 */
bool SortedDictType::are_key_type_and_key_valid(PyObject* ob, bool raise = true)
{
    return this->is_key_type_set(raise) && this->can_use_as_key(ob, raise);
}

PyObject* SortedDictType::repr(void)
{
    char const* delimiter = "";
    char const* actual_delimiter = ", ";
    std::string this_repr = LEFT_CURLY_BRACKET;
    for (auto& item : *this->map)
    {
        PyObjectWrapper key_repr(PyObject_Repr(item.first));  // 🆕
        if (key_repr == nullptr)
        {
            return nullptr;
        }
        PyObjectWrapper value_repr(PyObject_Repr(item.second));  // 🆕
        if (value_repr == nullptr)
        {
            return nullptr;
        }
        this_repr.append(delimiter)
            .append(PyUnicode_AsUTF8(key_repr.get()))
            .append(": ")
            .append(PyUnicode_AsUTF8(value_repr.get()));
        delimiter = actual_delimiter;
    }
    this_repr.append(RIGHT_CURLY_BRACKET);
    return PyUnicode_FromStringAndSize(this_repr.data(), this_repr.size());  // 🆕
}

/**
 * Check whether a key is present.
 *
 * @param ob Python object.
 *
 * @return 1 if it is present, else 0.
 */
int SortedDictType::contains(PyObject* key)
{
    if (!this->are_key_type_and_key_valid(key, false) || this->map->find(key) == this->map->end())
    {
        return 0;
    }
    return 1;
}

/**
 * Find the value mapped to a key. On failure, set a Python exception.
 *
 * @param key Key.
 *
 * @return Value if present, else `nullptr`.
 */
PyObject* SortedDictType::getitem(PyObject* key)
{
    if (!this->are_key_type_and_key_valid(key))
    {
        return nullptr;
    }
    auto it = this->map->find(key);
    if (it == this->map->end())
    {
        PyErr_SetObject(PyExc_KeyError, key);
        return nullptr;
    }
    return Py_NewRef(it->second);
}

/**
 * Map a value to a key or remove a key-value pair. On failure, set a Python
 * exception.
 *
 * @param key Key.
 * @param value Value.
 *
 * @return 0 if a key-value pair was inserted or removed, else -1.
 */
int SortedDictType::setitem(PyObject* key, PyObject* value)
{
    if (this->key_type == nullptr && value != nullptr)
    {
        // The first key-value pair is being inserted. Register the key type.
        PyObject* key_type = reinterpret_cast<PyObject*>(Py_TYPE(key));
        if (PyObject_RichCompareBool(key_type, reinterpret_cast<PyObject*>(&PyLong_Type), Py_EQ) != 1)
        {
            PyErr_SetString(PyExc_TypeError, "unsupported key type");
            return -1;
        }
        this->key_type = Py_NewRef(key_type);
    }

    if (!this->are_key_type_and_key_valid(key))
    {
        return -1;
    }

    // Insertion will be faster if the approximate location is known. Hence,
    // look for the nearest match.
    auto it = this->map->lower_bound(key);
    bool found = it != this->map->end() && !this->map->key_comp()(key, it->first);

    if (value == nullptr)
    {
        // Remove the key-value pair.
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
        // Insert a new key-value pair. The hint is correct; the key will get
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

PyObject* SortedDictType::clear(void)
{
    for (auto& item : *this->map)
    {
        Py_DECREF(item.first);
        Py_DECREF(item.second);
    }
    this->map->clear();
    Py_RETURN_NONE;
}

PyObject* SortedDictType::copy(void)
{
    PyTypeObject* type = Py_TYPE(this);
    PyObject* sd_copy = type->tp_alloc(type, 0);  // 🆕
    if (sd_copy == nullptr)
    {
        return nullptr;
    }
    SortedDictType* this_copy = reinterpret_cast<SortedDictType*>(sd_copy);
    this_copy->map = new std::map<PyObject*, PyObject*, PyObject_CustomCompare>(*this->map);
    for (auto& item : *this_copy->map)
    {
        Py_INCREF(item.first);
        Py_INCREF(item.second);
    }
    this_copy->key_type = Py_XNewRef(this->key_type);  // 🆕
    return sd_copy;
}

int SortedDictType::init(PyObject* args, PyObject* kwargs)
{
    // All initialisation is done immediately after allocation in order to
    // avoid leaving essential members uninitialised. This method is kept to
    // allow adding functionality in the future.
    return 0;
}

PyObject* SortedDictType::New(PyTypeObject* type, PyObject* args, PyObject* kwargs)
{
    PyObject* self = type->tp_alloc(type, 0);  // 🆕
    if (self == nullptr)
    {
        return nullptr;
    }

    // Python's default allocator claims to initialise the contents of the
    // allocated memory to null, but actually writes zeros to it. Hence,
    // explicitly initialise them.
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
    sd->map = new std::map<PyObject*, PyObject*, PyObject_CustomCompare>;
    sd->key_type = nullptr;
    return self;
}

/******************************************************************************
 * Code required to define the Python module and class can be found below this
 * point. Everything referenced therein is defined above in C++ style.
 *****************************************************************************/

/**
 * Deinitialise and deallocate.
 */
static void sorted_dict_type_dealloc(PyObject* self)
{
    // I could have separated deinitialisation and deallocation the way I
    // separated allocation and initialisation using the Python type methods,
    // but it looks like the deinitialisation (finalisation) function is not
    // automatically called prior to deallocation. Further, the documentation
    // recommends doing both here.
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
    sd->deinit();
    Py_TYPE(self)->tp_free(self);
}

/**
 * Stringify.
 */
static PyObject* sorted_dict_type_repr(PyObject* self)
{
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
    return sd->repr();
}

/**
 * Check whether a key is present.
 */
static int sorted_dict_type_contains(PyObject* self, PyObject* key)
{
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
    return sd->contains(key);
}

// clang-format off
static PySequenceMethods sorted_dict_type_sequence = {
    nullptr,                    // sq_length
    nullptr,                    // sq_concat
    nullptr,                    // sq_repeat
    nullptr,                    // sq_item
    nullptr,                    // was_sq_slice
    nullptr,                    // sq_ass_item
    nullptr,                    // was_sq_ass_slice
    sorted_dict_type_contains,  // sq_contains
    nullptr,                    // sq_inplace_concat
    nullptr,                    // sq_inplace_repeat
};
// clang-format on

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
    return sd->getitem(key);
}

/**
 * Map a value to a key or remove a key-value pair.
 */
static int sorted_dict_type_setitem(PyObject* self, PyObject* key, PyObject* value)
{
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
    return sd->setitem(key, value);
}

// clang-format off
static PyMappingMethods sorted_dict_type_mapping = {
    sorted_dict_type_len,      // mp_length
    sorted_dict_type_getitem,  // mp_subscript
    sorted_dict_type_setitem,  // mp_ass_subscript
};
// clang-format on

PyDoc_STRVAR(
    sorted_dict_type_clear_doc,
    "d.clear()\n"
    "Remove all key-value pairs in the sorted dictionary ``d``."
);

static PyObject* sorted_dict_type_clear(PyObject* self, PyObject* args)
{
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
    return sd->clear();
}

PyDoc_STRVAR(
    sorted_dict_type_copy_doc,
    "d.copy() -> SortedDict\n"
    "Return a shallow copy of the sorted dictionary ``d``."
);

static PyObject* sorted_dict_type_copy(PyObject* self, PyObject* args)
{
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
    return sd->copy();
}

// clang-format off
static PyMethodDef sorted_dict_type_methods[] = {
    {
        "clear",                      // ml_name
        sorted_dict_type_clear,       // ml_meth
        METH_NOARGS,                  // ml_flags
        sorted_dict_type_clear_doc,   // ml_doc
    },
    {
        "copy",                       // ml_name
        sorted_dict_type_copy,        // ml_meth
        METH_NOARGS,                  // ml_flags
        sorted_dict_type_copy_doc,    // ml_doc
    },
    {
        nullptr,
    }
};
// clang-format on

/**
 * Initialise.
 */
static int sorted_dict_type_init(PyObject* self, PyObject* args, PyObject* kwargs)
{
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
    return sd->init(args, kwargs);
}

/**
 * Allocate.
 */
static PyObject* sorted_dict_type_new(PyTypeObject* type, PyObject* args, PyObject* kwargs)
{
    return SortedDictType::New(type, args, kwargs);
}

PyDoc_STRVAR(
    sorted_dict_type_doc,
    "SortedDict(*args, **kwargs) -> SortedDict\n"
    "Create an empty sorted dictionary."
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
    sorted_dict_type_repr,                  // tp_repr
    nullptr,                                // tp_as_number
    &sorted_dict_type_sequence,             // tp_as_sequence
    &sorted_dict_type_mapping,              // tp_as_mapping
    PyObject_HashNotImplemented,            // tp_hash
    nullptr,                                // tp_call
    nullptr,                                // tp_str
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
    sorted_dict_type_init,                  // tp_init
    PyType_GenericAlloc,                    // tp_alloc
    sorted_dict_type_new,                   // tp_new
    PyObject_Free,                          // tp_free
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
    PyObject* mod = PyModule_Create(&sorted_dict_module);  // New reference.
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
