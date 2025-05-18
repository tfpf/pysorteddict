#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>

#include "sorted_dict_type.hh"
#include "sorted_dict_keys_type.hh"
#include "sorted_dict_utils.hh"

void SortedDictKeysType::deinit(void){
    Py_DECREF(this->sd);
}

PyObject* SortedDictKeysType::repr(void){
    std::string this_repr_utf8 = "SortedDictKeys" LEFT_PARENTHESIS LEFT_CURLY_BRACKET;
    for (auto& item : *this->sd->map){}
    this_repr_utf8.append(RIGHT_CURLY_BRACKET RIGHT_PARENTHESIS);
    return PyUnicode_FromStringAndSize(this_repr_utf8.data(), this_repr_utf8.size());  // 🆕
}

PyObject* SortedDictKeysType::New(PyTypeObject*type, SortedDictType*sd){
    PyObject* self = type->tp_alloc(type, 0);  // 🆕
    if (self == nullptr)
    {
        return nullptr;
    }

    SortedDictKeysType*sdk = reinterpret_cast<SortedDictKeysType*>(self);
    sdk->sd = sd;
    Py_INCREF(sdk->sd);
    return self;
}
