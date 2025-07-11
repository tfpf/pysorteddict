#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "sorted_dict_values_type.hh"
#include "sorted_dict_view_type.hh"

static PyObject* iterator_to_object(std::map<PyObject*, SortedDictValue, SortedDictKeyCompare>::iterator it)
{
    return Py_NewRef(it->second.value);  // 🆕
}

PyObject* SortedDictValuesIterType::New(PyTypeObject* type, SortedDictType* sd)
{
    return SortedDictViewIterType::New(type, sd, ::iterator_to_object);
}

PyObject* SortedDictValuesType::New(PyTypeObject* type, SortedDictType* sd)
{
    return SortedDictViewType::New(type, sd, ::iterator_to_object);
}
