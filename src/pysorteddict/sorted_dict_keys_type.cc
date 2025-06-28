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
    Py_RETURN_TRUE;
}

PyObject* SortedDictKeysType::getitem(Py_ssize_t start, Py_ssize_t stop, Py_ssize_t step)
{
    Py_RETURN_FALSE;
}

int SortedDictKeysType::contains(PyObject* key)
{
    return this->sd->contains(key);
}

PyObject*SortedDictKeysType::getitem(PyObject* idx)
{
    Py_ssize_t position = PyNumber_AsSsize_t(idx, PyExc_IndexError);
    if (position != -1 || PyErr_Occurred() == nullptr){
        return this->getitem(position);
    }
    PyErr_Clear();
Py_ssize_t start, stop, step;
if (PySlice_Unpack(idx, &start, &stop, &step) == 0){
return this->getitem(start, stop, step);
}
PyErr_Format(PyExc_TypeError, "got index %R, want position (integer) or slice", idx);
return nullptr;
}
