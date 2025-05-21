#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <map>

#include "sorted_dict_type.hh"
#include "sorted_dict_view_type.hh"

void SortedDictViewIterType::deinit(void)
{
    Py_DECREF(this->sd);
}

std::map<PyObject*, PyObject*, SortedDictKeyCompare>::iterator SortedDictViewIterType::next(void)
{
    return this->it == this->sd->map->end() ? this->it : this->it++;
}

PyObject* SortedDictViewIterType::New(PyTypeObject* type, SortedDictType* sd)
{
    PyObject* self = type->tp_alloc(type, 0);  // 🆕
    if (self == nullptr)
    {
        return nullptr;
    }

    SortedDictViewIterType* sdvi = reinterpret_cast<SortedDictViewIterType*>(self);
    sdvi->sd = sd;
    sdvi->it = sdvi->sd->map->begin();
    Py_INCREF(sdvi->sd);
    return self;
}

void SortedDictViewType::deinit(void)
{
    Py_DECREF(this->sd);
}

Py_ssize_t SortedDictViewType::len(void)
{
    return this->sd->map->size();
}

PyObject* SortedDictViewType::iter(PyTypeObject* type)
{
    return SortedDictViewIterType::New(type, this->sd);
}

PyObject* SortedDictViewType::New(PyTypeObject* type, SortedDictType* sd)
{
    PyObject* self = type->tp_alloc(type, 0);  // 🆕
    if (self == nullptr)
    {
        return nullptr;
    }

    SortedDictViewType* sdv = reinterpret_cast<SortedDictViewType*>(self);
    sdv->sd = sd;
    Py_INCREF(sdv->sd);
    return self;
}
