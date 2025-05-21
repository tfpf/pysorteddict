#ifndef SORTED_DICT_VIEW_TYPE_HH_
#define SORTED_DICT_VIEW_TYPE_HH_

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <map>

#include "sorted_dict_type.hh"

struct SortedDictViewType
{
public:
    PyObject_HEAD;

protected:
    SortedDictType* sd;

public:
    void deinit(void);
    Py_ssize_t len(void);
    static PyObject* New(PyTypeObject*, SortedDictType*);
};

struct SortedDictViewIteratorType
{
public:
    PyObject_HEAD;

protected:
    SortedDictType* sd;
    std::map<PyObject*, PyObject*, SortedDictKeyCompare>::iterator it;
};

#endif
