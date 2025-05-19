#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iterator>
#include <map>
#include <string>

#include "sorted_dict_keys_type.hh"
#include "sorted_dict_type.hh"
#include "sorted_dict_utils.hh"

void SortedDictKeysType::deinit(void)
{
    Py_DECREF(this->sd);
}

PyObject* SortedDictKeysType::repr(void)
{
    char const* delimiter = "";
    char const* actual_delimiter = ", ";
    std::string this_repr_utf8 = "SortedDictKeys" LEFT_PARENTHESIS LEFT_CURLY_BRACKET;
    for (auto& item : *this->sd->map)
    {
        PyObjectWrapper key_repr(PyObject_Repr(item.first));  // 🆕
        if (key_repr == nullptr)
        {
            return nullptr;
        }
        Py_ssize_t key_repr_size;
        char const* key_repr_utf8 = PyUnicode_AsUTF8AndSize(key_repr.get(), &key_repr_size);
        this_repr_utf8.append(delimiter).append(key_repr_utf8, key_repr_size);
        delimiter = actual_delimiter;
    }
    this_repr_utf8.append(RIGHT_CURLY_BRACKET RIGHT_PARENTHESIS);
    return PyUnicode_FromStringAndSize(this_repr_utf8.data(), this_repr_utf8.size());  // 🆕
}

Py_ssize_t SortedDictKeysType::len(void)
{
    return this->sd->map->size();
}

PyObject* SortedDictKeysType::New(PyTypeObject* type, SortedDictType* sd)
{
    PyObject* self = type->tp_alloc(type, 0);  // 🆕
    if (self == nullptr)
    {
        return nullptr;
    }

    SortedDictKeysType* sdk = reinterpret_cast<SortedDictKeysType*>(self);
    sdk->sd = sd;
    Py_INCREF(sdk->sd);
    return self;
}
