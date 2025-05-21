#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <map>
#include <string>

#include "sorted_dict_keys_type.hh"
#include "sorted_dict_type.hh"
#include "sorted_dict_utils.hh"

PyObject* SortedDictKeysIterType::next(void)
{
    auto it = this->SortedDictViewIterType::next();
    return it == this->sd->map->end() ? nullptr : Py_NewRef(it->first);
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
