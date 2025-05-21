#ifndef SORTED_DICT_KEYS_TYPE_HH_
#define SORTED_DICT_KEYS_TYPE_HH_

#include "sorted_dict_view_type.hh"

struct SortedDictKeysType: public SortedDictViewType
{
PyObject* repr(void);
};

struct SortedDictKeysIteratorType: public SortedDictViewIteratorType
{
};

#endif
