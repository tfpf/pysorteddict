#ifndef SORTED_DICT_VIEW_TYPE_HH_
#define SORTED_DICT_VIEW_TYPE_HH_

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <map>

#include "sorted_dict_type.hh"

struct SortedDictViewIterType
{
public:
    PyObject_HEAD;

protected:
    SortedDictType* sd;
    std::map<PyObject*, PyObject*, SortedDictKeyCompare>::iterator it;

public:
    void deinit(void);
    std::map<PyObject*, PyObject*, SortedDictKeyCompare>::iterator next(void);
    static PyObject* New(PyTypeObject*, SortedDictType*);
};

struct SortedDictViewType
{
public:
    PyObject_HEAD;

protected:
    SortedDictType* sd;

public:
    void deinit(void);
    Py_ssize_t len(void);
    PyObject* iter(PyTypeObject*);
    static PyObject* New(PyTypeObject*, SortedDictType*);
};

#endif
