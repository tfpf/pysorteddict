#ifndef SORTED_DICT_KEYS_TYPE_HH_
#define SORTED_DICT_KEYS_TYPE_HH_

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "sorted_dict_type.hh"

struct SortedDictKeysType
{
public:
    PyObject_HEAD;

private:
    SortedDictType* sd;

public:
    void deinit(void);
    PyObject* repr(void);
    Py_ssize_t len(void);
    static PyObject* New(PyTypeObject*, SortedDictType*);
};

#endif
