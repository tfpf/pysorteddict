#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <map>

// clang-format off
struct SortedDictType
{
    PyObject_HEAD
    void *map;
    PyTypeObject *key_type;
};
// clang-format on

/**
 * C++-style comparison implementation for Python objects.
 */
struct ComparePyObjects
{
    bool operator()(PyObject* a, PyObject* b)
    {
        // This assumes that the comparison operation will never error out. I
        // think it should be enough to ensure that the two Python objects
        // being compared always have the same type.
        return PyObject_RichCompareBool(a, b, Py_LE) == 1;
    }
};

/**
 * Deinitialise and deallocate.
 */
static void sorted_dict_type_dealloc(PyObject *self){
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

    PyObject* key_type;  // Borrowed reference.
    // Casting a string constant to a non-const pointer is not permitted in
    // C++, but the signature of this function is such that I am forced to.
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|", (char*[]) { "key_type", nullptr }, &key_type))
    {
        return nullptr;
    }

    // Check the type to use for keys.
    if (PyObject_RichCompareBool(key_type, (PyObject*)&PyLong_Type, Py_EQ) != 1)
    {
        PyErr_SetString(PyExc_ValueError, "constructor argument must be a supported type");
        return nullptr;
    }

    SortedDictType* sd = (SortedDictType*)self;
    sd->map = new std::map<PyObject*, PyObject*, ComparePyObjects>;
    sd->key_type = (PyTypeObject*)key_type;
    Py_INCREF(sd->key_type);
    return self;
}

// clang-format off
static PyTypeObject sorted_dict_type = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0)
    .tp_name = "pysorteddict.SortedDict",
    .tp_basicsize = sizeof(SortedDictType),
    .tp_itemsize = 0,
    .tp_dealloc = sorted_dict_type_dealloc,
    // .tp_repr = sorted_dict_type_repr,
    // .tp_as_mapping = &sorted_dict_type_mapping,
    .tp_hash = PyObject_HashNotImplemented,
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
