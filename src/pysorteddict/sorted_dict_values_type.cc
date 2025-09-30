#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <map>

#include "sorted_dict_type.hh"
#include "sorted_dict_values_type.hh"
#include "sorted_dict_view_type.hh"

static PyObject* forward_iterator_to_object(FwdIterType it)
{
    return Py_NewRef(it->second.value);  // 🆕
}

PyObject* SortedDictValuesType::New(PyTypeObject* type, SortedDictType* sd)
{
    return SortedDictViewType::New(type, sd, ::forward_iterator_to_object, nullptr);
}
