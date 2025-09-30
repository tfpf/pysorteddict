#ifndef SORTED_DICT_VIEW_TYPE_HH_
#define SORTED_DICT_VIEW_TYPE_HH_

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iterator>
#include <map>

#include "sorted_dict_type.hh"

using FwdIterType = std::map<PyObject*, SortedDictValue, SortedDictKeyCompare>::iterator;
using FwdIterToOb = PyObject* (*)(FwdIterType);

using RevIterType = std::reverse_iterator<FwdIterType>;
using RevIterToOb = PyObject* (*)(RevIterType);

template<typename T>
struct SortedDictViewIterType
{
public:
    PyObject_HEAD;

protected:
    SortedDictType* sd;
    T it;
    bool should_raise_stop_iteration;

    // See below for why this is required.
    PyObject* (*iterator_to_object)(T);

private:
    void track(T);
    void untrack(T);

public:
    static void Delete(PyObject* self)
    {
        SortedDictViewIterType<T>* sdvi = reinterpret_cast<SortedDictViewIterType<T>*>(self);
        if (!sdvi->should_raise_stop_iteration)
        {
            sdvi->untrack(sdvi->it);
            --sdvi->sd->known_referrers;
            Py_DECREF(sdvi->sd);
        }
        Py_TYPE(self)->tp_free(self);
    }

    PyObject* next(void)
    {
        if (this->should_raise_stop_iteration)
        {
            return nullptr;
        }

        // The 'next' key-value pair is the current one the iterator points to.
        T curr = this->it++;
        this->untrack(curr);
        this->track(this->it);
        return this->iterator_to_object(curr);
    }

    static PyObject* New(PyTypeObject*, SortedDictType*, PyObject* (*iterator_to_object)(T));
};

struct SortedDictViewType
{
public:
    PyObject_HEAD;

protected:
    SortedDictType* sd;

    // To mimic dynamic dispatch. Child classes should provide these functions,
    // both of which should convert a C++ iterator into a Python object. Using
    // C++ virtual functions would require constructing instances the C++ way,
    // but C++ constructors won't know how to initialise the Python-specific
    // members. On the other hand, the default memory allocator provided by
    // Python (which does initialise Python-specific members) does not run
    // constructors.
    FwdIterToOb forward_iterator_to_object;
    RevIterToOb reverse_iterator_to_object;

private:
    PyObject* getitem(Py_ssize_t);
    PyObject* getitem(Py_ssize_t, Py_ssize_t, Py_ssize_t);

public:
    static void Delete(PyObject*);
    static PyObject* repr(PyObject*);
    Py_ssize_t len(void);
    PyObject* getitem(PyObject*);
    PyObject* iter(PyTypeObject*);
    static PyObject* New(PyTypeObject*, SortedDictType*, FwdIterToOb, RevIterToOb);
};

#endif
