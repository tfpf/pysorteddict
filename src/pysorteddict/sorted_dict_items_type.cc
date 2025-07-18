#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <map>

#include "sorted_dict_items_type.hh"
#include "sorted_dict_type.hh"
#include "sorted_dict_view_type.hh"

static PyObject* iterator_to_object(std::map<PyObject*, SortedDictValue, SortedDictKeyCompare>::iterator it)
{
    return PyTuple_Pack(2, it->first, it->second.value);  // 🆕
}

int SortedDictItemsType::contains(PyObject* item)
{
    return 0;
}

PyObject* SortedDictItemsType::New(PyTypeObject* type, SortedDictType* sd)
{
    return SortedDictViewType::New(type, sd, ::iterator_to_object);
}
