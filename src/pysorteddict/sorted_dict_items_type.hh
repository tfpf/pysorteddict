#ifndef SORTED_DICT_ITEMS_TYPE_HH_
#define SORTED_DICT_ITEMS_TYPE_HH_

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "sorted_dict_view_type.hh"

template<typename T>
struct SortedDictItemsIterType : public SortedDictViewIterType<T>
{
};

struct SortedDictItemsType : public SortedDictViewType
{
public:
    int contains(PyObject*);
    static PyObject* New(PyTypeObject*, SortedDictType*);
};

#endif
