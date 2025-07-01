#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <algorithm>

#include "sorted_dict_keys_type.hh"
#include "sorted_dict_view_type.hh"

PyObject* SortedDictKeysIterType::next(void)
{
    return Py_XNewRef(this->SortedDictViewIterType::next().first);  // 🆕
}

PyObject* SortedDictKeysType::getitem(Py_ssize_t position)
{
    Py_ssize_t sz = this->sd->len();
    if (sz == -1)
    {
        return nullptr;
    }
    Py_ssize_t positive_position = position >= 0 ? position : position + sz;
    if (positive_position < 0 || sz <= positive_position)
    {
        PyErr_Format(PyExc_IndexError, "got invalid index %zd for sorted dictionary of length %zd", position, sz);
        return nullptr;
    }
    if (positive_position < sz / 2)
    {
        auto it = this->sd->map->begin();
        std::advance(it, positive_position);
        return Py_NewRef(it->first);  // 🆕
    }
    auto it = this->sd->map->rbegin();
    std::advance(it, sz - 1 - positive_position);
    return Py_NewRef(it->first);  // 🆕
}

PyObject* SortedDictKeysType::getitem(Py_ssize_t start, Py_ssize_t stop, Py_ssize_t step)
{
    Py_ssize_t sz = this->sd->len();
    if (sz == -1)
    {
        return nullptr;
    }
    Py_ssize_t slice_len = PySlice_AdjustIndices(sz, &start, &stop, step);
fprintf(stderr, "(%zd, %zd, %zd): %zd\n", start, stop, step, slice_len);
    if (slice_len == 0)
    {
        return PyList_New(0);  // 🆕
    }
    return Py_NotImplemented;
}

int SortedDictKeysType::contains(PyObject* key)
{
    return this->sd->contains(key);
}

PyObject* SortedDictKeysType::getitem(PyObject* idx)
{
    Py_ssize_t position = PyNumber_AsSsize_t(idx, PyExc_IndexError);
    if (position != -1 || PyErr_Occurred() == nullptr)
    {
        return this->getitem(position);
    }
    PyErr_Clear();
    Py_ssize_t start, stop, step;
    if (PySlice_Unpack(idx, &start, &stop, &step) == 0)
    {
        return this->getitem(start, stop, step);
    }
    PyErr_Format(
        PyExc_TypeError, "got index %R of type %R, want index of type %R or %R with non-zero step", idx, Py_TYPE(idx),
        &PyLong_Type, &PySlice_Type
    );
    return nullptr;
}
