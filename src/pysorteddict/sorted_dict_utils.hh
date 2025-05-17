#ifndef SORTED_DICT_UTILS_HH_
#define SORTED_DICT_UTILS_HH_

#include <memory>

/**
 * C++-style clean-up implementation for Python objects.
 */
struct PyObject_Delete
{
    void operator()(PyObject* ob)
    {
        Py_XDECREF(ob);
    }
};

using PyObjectWrapper = std::unique_ptr<PyObject, PyObject_Delete>;

#endif
