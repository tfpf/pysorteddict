#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <map>

#include "sorted_dict_keys_type.hh"
#include "sorted_dict_type.hh"
#include "sorted_dict_view_type.hh"

static PyObject* forward_iterator_to_object(FwdIterType it)
{
    return Py_NewRef(it->first);  // 🆕
}

template <> PyObject* SortedDictKeysIterType<FwdIterType>::New(PyTypeObject* type, SortedDictType* sd)
{
    return SortedDictViewIterType<FwdIterType>::New(type, sd, ::forward_iterator_to_object);
}

int SortedDictKeysType::contains(PyObject* key)
{
    return this->sd->contains(key);
}

PyObject* SortedDictKeysType::New(PyTypeObject* type, SortedDictType* sd)
{
    return SortedDictViewType::New(type, sd, ::forward_iterator_to_object, nullptr);
}

template struct SortedDictKeysIterType<FwdIterType>;
