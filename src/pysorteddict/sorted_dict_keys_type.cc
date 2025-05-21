#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <map>

#include "sorted_dict_keys_type.hh"
#include "sorted_dict_type.hh"

PyObject* SortedDictKeysIterType::next(void)
{
    auto it = this->SortedDictViewIterType::next();
    return it == this->sd->map->end() ? nullptr : Py_NewRef(it->first);
}
