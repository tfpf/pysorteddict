#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "sorted_dict_type.hh"
#include "sorted_dict_keys_type.hh"

void SortedDictKeysType::deinit(void){
    Py_DECREF(this->sd);
}

PyObject* New(PyTypeObject*type, SortedDictType*sd){
    PyObject* self = type->tp_alloc(type, 0);  // 🆕
    if (self == nullptr)
    {
        return nullptr;
    }

    SortedDictKeysType*sdk = reinterpret_cast<SortedDictKeysType*>(self);
    sdk->sd = Py_NewRef(sd);
    return self;
}
