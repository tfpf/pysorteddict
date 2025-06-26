#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "sorted_dict_keys_type.hh"
#include "sorted_dict_view_type.hh"

PyObject* SortedDictKeysIterType::next(void)
{
    return Py_XNewRef(this->SortedDictViewIterType::next().first);  // 🆕
}

PyObject* SortedDictKeysType::getitem(Py_ssize_t position)
{
    Py_RETURN_NONE;
}

PyObject* SortedDictKeysType::getitem(Py_ssize_t slice_len, Py_ssize_t start, Py_ssize_t stop, Py_ssize_t step)
{
    Py_RETURN_NONE;
}

int SortedDictKeysType::contains(PyObject* key)
{
    return this->sd->contains(key);
}

PyObject* SortedDictKeysType::getitem(PyObject* idx)
{
    return this->SortedDictViewType::getitem(idx);
}
