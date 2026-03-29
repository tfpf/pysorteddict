#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "sorted_dict_items_type.hh"
#include "sorted_dict_keys_type.hh"
#include "sorted_dict_type.hh"
#include "sorted_dict_values_type.hh"

/**
 * Deinitialise and deallocate.
 */
static void sorted_dict_items_fwd_iter_type_dealloc(PyObject* self)
{
    SortedDictItemsIterType<FwdIterType>::Delete(self);
}

/**
 * Retrieve the next element.
 */
static PyObject* sorted_dict_items_fwd_iter_type_next(PyObject* self)
{
    return reinterpret_cast<SortedDictItemsIterType<FwdIterType>*>(self)->next();
}

static PyTypeObject sorted_dict_items_fwd_iter_type = {
    // clang-format off
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0)
    .tp_name = "pysorteddict.SortedDictItemsFwdIter",
    // clang-format on
    .tp_basicsize = sizeof(SortedDictItemsIterType<FwdIterType>),
    .tp_dealloc = sorted_dict_items_fwd_iter_type_dealloc,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Forward iterator over the items in a sorted dictionary.",
    .tp_iter = PyObject_SelfIter,
    .tp_iternext = sorted_dict_items_fwd_iter_type_next,
    .tp_alloc = PyType_GenericAlloc,
    .tp_free = PyObject_Free,
};

/**
 * Deinitialise and deallocate.
 */
static void sorted_dict_items_rev_iter_type_dealloc(PyObject* self)
{
    SortedDictItemsIterType<RevIterType>::Delete(self);
}

/**
 * Retrieve the next element.
 */
static PyObject* sorted_dict_items_rev_iter_type_next(PyObject* self)
{
    return reinterpret_cast<SortedDictItemsIterType<RevIterType>*>(self)->next();
}

static PyTypeObject sorted_dict_items_rev_iter_type = {
    // clang-format off
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0)
    .tp_name = "pysorteddict.SortedDictItemsRevIter",
    // clang-format on
    .tp_basicsize = sizeof(SortedDictItemsIterType<RevIterType>),
    .tp_dealloc = sorted_dict_items_rev_iter_type_dealloc,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Reverse iterator over the items in a sorted dictionary.",
    .tp_iter = PyObject_SelfIter,
    .tp_iternext = sorted_dict_items_rev_iter_type_next,
    .tp_alloc = PyType_GenericAlloc,
    .tp_free = PyObject_Free,
};

/**
 * Deinitialise and deallocate.
 */
static void sorted_dict_items_type_dealloc(PyObject* self)
{
    SortedDictItemsType::Delete(self);
}

/**
 * Stringify.
 */
static PyObject* sorted_dict_items_type_repr(PyObject* self)
{
    return SortedDictItemsType::repr(self);
}

/**
 * Obtain the number of items.
 */
static Py_ssize_t sorted_dict_items_type_len(PyObject* self)
{
    return reinterpret_cast<SortedDictItemsType*>(self)->len();
}

/**
 * Check whether a item is present.
 */
static int sorted_dict_items_type_contains(PyObject* self, PyObject* item)
{
    return reinterpret_cast<SortedDictItemsType*>(self)->contains(item);
}

static PySequenceMethods sorted_dict_items_type_sequence = {
    .sq_length = sorted_dict_items_type_len,
    .sq_contains = sorted_dict_items_type_contains,
};

/**
 * Retrieve the item at a position or items in a slice.
 */
static PyObject* sorted_dict_items_type_getitem(PyObject* self, PyObject* idx)
{
    return reinterpret_cast<SortedDictItemsType*>(self)->getitem(idx);
}

static PyMappingMethods sorted_dict_items_type_mapping = {
    .mp_subscript = sorted_dict_items_type_getitem,
};

/**
 * Create a forward iterator.
 */
static PyObject* sorted_dict_items_type_iter(PyObject* self)
{
    return reinterpret_cast<SortedDictItemsType*>(self)->iter(&sorted_dict_items_fwd_iter_type);
}

PyDoc_STRVAR(sorted_dict_items_type_reversed_doc, "Implement reversed(self).");

static PyObject* sorted_dict_items_type_reversed(PyObject* self, PyObject* args)
{
    return reinterpret_cast<SortedDictItemsType*>(self)->reversed(&sorted_dict_items_rev_iter_type);
}

static PyMethodDef sorted_dict_items_type_methods[] = {
    {
        .ml_name = "__reversed__",
        .ml_meth = sorted_dict_items_type_reversed,
        .ml_flags = METH_NOARGS,
        .ml_doc = sorted_dict_items_type_reversed_doc,
    },
    { nullptr },
};

static PyTypeObject sorted_dict_items_type = {
    // clang-format off
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0)
    .tp_name = "pysorteddict.SortedDictItems",
    // clang-format on
    .tp_basicsize = sizeof(SortedDictItemsType),
    .tp_dealloc = sorted_dict_items_type_dealloc,
    .tp_repr = sorted_dict_items_type_repr,
    .tp_as_sequence = &sorted_dict_items_type_sequence,
    .tp_as_mapping = &sorted_dict_items_type_mapping,
    .tp_hash = PyObject_HashNotImplemented,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Dynamic view on the items in a sorted dictionary.",
    .tp_iter = sorted_dict_items_type_iter,
    .tp_methods = sorted_dict_items_type_methods,
    .tp_alloc = PyType_GenericAlloc,
    .tp_free = PyObject_Free,
};

/**
 * Deinitialise and deallocate.
 */
static void sorted_dict_keys_fwd_iter_type_dealloc(PyObject* self)
{
    SortedDictKeysIterType<FwdIterType>::Delete(self);
}

/**
 * Retrieve the next element.
 */
static PyObject* sorted_dict_keys_fwd_iter_type_next(PyObject* self)
{
    return reinterpret_cast<SortedDictKeysIterType<FwdIterType>*>(self)->next();
}

static PyTypeObject sorted_dict_keys_fwd_iter_type = {
    // clang-format off
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0)
    .tp_name = "pysorteddict.SortedDictKeysFwdIter",
    // clang-format on
    .tp_basicsize = sizeof(SortedDictKeysIterType<FwdIterType>),
    .tp_dealloc = sorted_dict_keys_fwd_iter_type_dealloc,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Forward iterator over the keys in a sorted dictionary.",
    .tp_iter = PyObject_SelfIter,
    .tp_iternext = sorted_dict_keys_fwd_iter_type_next,
    .tp_alloc = PyType_GenericAlloc,
    .tp_free = PyObject_Free,
};

/**
 * Deinitialise and deallocate.
 */
static void sorted_dict_keys_rev_iter_type_dealloc(PyObject* self)
{
    SortedDictKeysIterType<RevIterType>::Delete(self);
}

/**
 * Retrieve the next element.
 */
static PyObject* sorted_dict_keys_rev_iter_type_next(PyObject* self)
{
    return reinterpret_cast<SortedDictKeysIterType<RevIterType>*>(self)->next();
}

static PyTypeObject sorted_dict_keys_rev_iter_type = {
    // clang-format off
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0)
    .tp_name = "pysorteddict.SortedDictKeysRevIter",
    // clang-format on
    .tp_basicsize = sizeof(SortedDictKeysIterType<RevIterType>),
    .tp_dealloc = sorted_dict_keys_rev_iter_type_dealloc,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Reverse iterator over the keys in a sorted dictionary.",
    .tp_iter = PyObject_SelfIter,
    .tp_iternext = sorted_dict_keys_rev_iter_type_next,
    .tp_alloc = PyType_GenericAlloc,
    .tp_free = PyObject_Free,
};

/**
 * Deinitialise and deallocate.
 */
static void sorted_dict_keys_type_dealloc(PyObject* self)
{
    SortedDictKeysType::Delete(self);
}

/**
 * Stringify.
 */
static PyObject* sorted_dict_keys_type_repr(PyObject* self)
{
    return SortedDictKeysType::repr(self);
}

/**
 * Obtain the number of keys.
 */
static Py_ssize_t sorted_dict_keys_type_len(PyObject* self)
{
    return reinterpret_cast<SortedDictKeysType*>(self)->len();
}

/**
 * Check whether a key is present.
 */
static int sorted_dict_keys_type_contains(PyObject* self, PyObject* key)
{
    return reinterpret_cast<SortedDictKeysType*>(self)->contains(key);
}

static PySequenceMethods sorted_dict_keys_type_sequence = {
    .sq_length = sorted_dict_keys_type_len,
    .sq_contains = sorted_dict_keys_type_contains,
};

/**
 * Retrieve the key at a position or keys in a slice.
 */
static PyObject* sorted_dict_keys_type_getitem(PyObject* self, PyObject* idx)
{
    return reinterpret_cast<SortedDictKeysType*>(self)->getitem(idx);
}

static PyMappingMethods sorted_dict_keys_type_mapping = {
    .mp_subscript = sorted_dict_keys_type_getitem,
};

/**
 * Create a forward iterator.
 */
static PyObject* sorted_dict_keys_type_iter(PyObject* self)
{
    return reinterpret_cast<SortedDictKeysType*>(self)->iter(&sorted_dict_keys_fwd_iter_type);
}

PyDoc_STRVAR(sorted_dict_keys_type_reversed_doc, "Implement reversed(self).");

static PyObject* sorted_dict_keys_type_reversed(PyObject* self, PyObject* args)
{
    return reinterpret_cast<SortedDictKeysType*>(self)->reversed(&sorted_dict_keys_rev_iter_type);
}

static PyMethodDef sorted_dict_keys_type_methods[] = {
    {
        .ml_name = "__reversed__",
        .ml_meth = sorted_dict_keys_type_reversed,
        .ml_flags = METH_NOARGS,
        .ml_doc = sorted_dict_keys_type_reversed_doc,
    },
    { nullptr },
};

static PyTypeObject sorted_dict_keys_type = {
    // clang-format off
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0)
    .tp_name = "pysorteddict.SortedDictKeys",
    // clang-format on
    .tp_basicsize = sizeof(SortedDictKeysType),
    .tp_dealloc = sorted_dict_keys_type_dealloc,
    .tp_repr = sorted_dict_keys_type_repr,
    .tp_as_sequence = &sorted_dict_keys_type_sequence,
    .tp_as_mapping = &sorted_dict_keys_type_mapping,
    .tp_hash = PyObject_HashNotImplemented,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Dynamic view on the keys in a sorted dictionary.",
    .tp_iter = sorted_dict_keys_type_iter,
    .tp_methods = sorted_dict_keys_type_methods,
    .tp_alloc = PyType_GenericAlloc,
    .tp_free = PyObject_Free,
};

/**
 * Deinitialise and deallocate.
 */
static void sorted_dict_values_fwd_iter_type_dealloc(PyObject* self)
{
    SortedDictValuesIterType<FwdIterType>::Delete(self);
}

/**
 * Retrieve the next element.
 */
static PyObject* sorted_dict_values_fwd_iter_type_next(PyObject* self)
{
    return reinterpret_cast<SortedDictValuesIterType<FwdIterType>*>(self)->next();
}

static PyTypeObject sorted_dict_values_fwd_iter_type = {
    // clang-format off
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0)
    .tp_name = "pysorteddict.SortedDictValuesFwdIter",
    // clang-format on
    .tp_basicsize = sizeof(SortedDictValuesIterType<FwdIterType>),
    .tp_dealloc = sorted_dict_values_fwd_iter_type_dealloc,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Forward iterator over the values in a sorted dictionary.",
    .tp_iter = PyObject_SelfIter,
    .tp_iternext = sorted_dict_values_fwd_iter_type_next,
    .tp_alloc = PyType_GenericAlloc,
    .tp_free = PyObject_Free,
};

/**
 * Deinitialise and deallocate.
 */
static void sorted_dict_values_rev_iter_type_dealloc(PyObject* self)
{
    SortedDictValuesIterType<RevIterType>::Delete(self);
}

/**
 * Retrieve the next element.
 */
static PyObject* sorted_dict_values_rev_iter_type_next(PyObject* self)
{
    return reinterpret_cast<SortedDictValuesIterType<RevIterType>*>(self)->next();
}

static PyTypeObject sorted_dict_values_rev_iter_type = {
    // clang-format off
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0)
    .tp_name = "pysorteddict.SortedDictValuesRevIter",
    // clang-format on
    .tp_basicsize = sizeof(SortedDictValuesIterType<RevIterType>),
    .tp_dealloc = sorted_dict_values_rev_iter_type_dealloc,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Reverse iterator over the values in a sorted dictionary.",
    .tp_iter = PyObject_SelfIter,
    .tp_iternext = sorted_dict_values_rev_iter_type_next,
    .tp_alloc = PyType_GenericAlloc,
    .tp_free = PyObject_Free,
};

/**
 * Deinitialise and deallocate.
 */
static void sorted_dict_values_type_dealloc(PyObject* self)
{
    SortedDictValuesType::Delete(self);
}

/**
 * Stringify.
 */
static PyObject* sorted_dict_values_type_repr(PyObject* self)
{
    return SortedDictValuesType::repr(self);
}

/**
 * Obtain the number of values.
 */
static Py_ssize_t sorted_dict_values_type_len(PyObject* self)
{
    return reinterpret_cast<SortedDictValuesType*>(self)->len();
}

static PySequenceMethods sorted_dict_values_type_sequence = {
    .sq_length = sorted_dict_values_type_len,
};

/**
 * Retrieve the value at a position or values in a slice.
 */
static PyObject* sorted_dict_values_type_getitem(PyObject* self, PyObject* idx)
{
    return reinterpret_cast<SortedDictValuesType*>(self)->getitem(idx);
}

static PyMappingMethods sorted_dict_values_type_mapping = {
    .mp_subscript = sorted_dict_values_type_getitem,
};

/**
 * Create a forward iterator.
 */
static PyObject* sorted_dict_values_type_iter(PyObject* self)
{
    return reinterpret_cast<SortedDictValuesType*>(self)->iter(&sorted_dict_values_fwd_iter_type);
}

PyDoc_STRVAR(sorted_dict_values_type_reversed_doc, "Implement reversed(self).");

static PyObject* sorted_dict_values_type_reversed(PyObject* self, PyObject* args)
{
    return reinterpret_cast<SortedDictValuesType*>(self)->reversed(&sorted_dict_values_rev_iter_type);
}

static PyMethodDef sorted_dict_values_type_methods[] = {
    {
        .ml_name = "__reversed__",
        .ml_meth = sorted_dict_values_type_reversed,
        .ml_flags = METH_NOARGS,
        .ml_doc = sorted_dict_values_type_reversed_doc,
    },
    { nullptr },
};

static PyTypeObject sorted_dict_values_type = {
    // clang-format off
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0)
    .tp_name = "pysorteddict.SortedDictValues",
    // clang-format on
    .tp_basicsize = sizeof(SortedDictValuesType),
    .tp_dealloc = sorted_dict_values_type_dealloc,
    .tp_repr = sorted_dict_values_type_repr,
    .tp_as_sequence = &sorted_dict_values_type_sequence,
    .tp_as_mapping = &sorted_dict_values_type_mapping,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Dynamic view on the values in a sorted dictionary.",
    .tp_iter = sorted_dict_values_type_iter,
    .tp_methods = sorted_dict_values_type_methods,
    .tp_alloc = PyType_GenericAlloc,
    .tp_free = PyObject_Free,
};

/**
 * Deinitialise and deallocate.
 */
static void sorted_dict_type_dealloc(PyObject* self)
{
    // I could have separated deinitialisation and deallocation the way I
    // separated allocation and initialisation using the Python type methods,
    // but it looks like the deinitialisation (finalisation) function is not
    // automatically called prior to deallocation. Further, the documentation
    // recommends doing both here.
    SortedDictType::Delete(self);
}

/**
 * Stringify.
 */
static PyObject* sorted_dict_type_repr(PyObject* self)
{
    return reinterpret_cast<SortedDictType*>(self)->repr();
}

/**
 * Check whether a key is present.
 */
static int sorted_dict_type_contains(PyObject* self, PyObject* key)
{
    return reinterpret_cast<SortedDictType*>(self)->contains(key);
}

static PySequenceMethods sorted_dict_type_sequence = {
    .sq_contains = sorted_dict_type_contains,
};

/**
 * Obtain the number of key-value pairs.
 */
static Py_ssize_t sorted_dict_type_len(PyObject* self)
{
    return reinterpret_cast<SortedDictType*>(self)->len();
}

/**
 * Find the value mapped to a key.
 */
static PyObject* sorted_dict_type_getitem(PyObject* self, PyObject* key)
{
    return reinterpret_cast<SortedDictType*>(self)->getitem(key);
}

/**
 * Map a value to a key or remove a key-value pair.
 */
static int sorted_dict_type_setitem(PyObject* self, PyObject* key, PyObject* value)
{
    return reinterpret_cast<SortedDictType*>(self)->setitem(key, value);
}

static PyMappingMethods sorted_dict_type_mapping = {
    .mp_length = sorted_dict_type_len,
    .mp_subscript = sorted_dict_type_getitem,
    .mp_ass_subscript = sorted_dict_type_setitem,
};

/**
 * Create a forward iterator.
 */
static PyObject* sorted_dict_type_iter(PyObject* self)
{
    return reinterpret_cast<SortedDictType*>(self)->iter(&sorted_dict_keys_fwd_iter_type);
}

PyDoc_STRVAR(sorted_dict_type_reversed_doc, "Implement reversed(self).");

static PyObject* sorted_dict_type_reversed(PyObject* self, PyObject* args)
{
    return reinterpret_cast<SortedDictType*>(self)->reversed(&sorted_dict_keys_rev_iter_type);
}

PyDoc_STRVAR(
    sorted_dict_type_clear_doc,
    "d.clear()\n"
    "Remove all key-value pairs in the sorted dictionary ``d``."
);

static PyObject* sorted_dict_type_clear(PyObject* self, PyObject* args)
{
    return reinterpret_cast<SortedDictType*>(self)->clear();
}

PyDoc_STRVAR(
    sorted_dict_type_copy_doc,
    "d.copy() -> SortedDict\n"
    "Return a shallow copy of the sorted dictionary ``d``."
);

static PyObject* sorted_dict_type_copy(PyObject* self, PyObject* args)
{
    return reinterpret_cast<SortedDictType*>(self)->copy();
}

PyDoc_STRVAR(
    sorted_dict_type_get_doc,
    "d.get(key: Any, default: Any = None, /) -> Any\n"
    "Return the value mapped to ``key`` in the sorted dictionary ``d``, or ``default`` if ``key`` isn't in ``d``."
);

static PyObject* sorted_dict_type_get(PyObject* self, PyObject* const* args, Py_ssize_t nargs)
{
    return reinterpret_cast<SortedDictType*>(self)->get(args, nargs);
}

PyDoc_STRVAR(
    sorted_dict_type_items_doc,
    "d.items() -> SortedDictItems\n"
    "Return a dynamic view on the items in the sorted dictionary ``d``."
);

static PyObject* sorted_dict_type_items(PyObject* self, PyObject* args)
{
    return reinterpret_cast<SortedDictType*>(self)->items(&sorted_dict_items_type);
}

PyDoc_STRVAR(
    sorted_dict_type_keys_doc,
    "d.keys() -> SortedDictKeys\n"
    "Return a dynamic view on the keys in the sorted dictionary ``d``."
);

static PyObject* sorted_dict_type_keys(PyObject* self, PyObject* args)
{
    return reinterpret_cast<SortedDictType*>(self)->keys(&sorted_dict_keys_type);
}

PyDoc_STRVAR(
    sorted_dict_type_setdefault_doc,
    "d.setdefault(key: Any, default: Any = None, /) -> Any\n"
    "Return ``d.get(key, default)``, and map ``default`` to ``key`` if ``key`` isn't in the sorted dictionary ``d``."
);

static PyObject* sorted_dict_type_setdefault(PyObject* self, PyObject* const* args, Py_ssize_t nargs)
{
    return reinterpret_cast<SortedDictType*>(self)->setdefault(args, nargs);
}

PyDoc_STRVAR(
    sorted_dict_type_update_doc,
    "d.update(*args, **kwargs)\n"
    "Update the sorted dictionary ``d`` with the keys and values from ``args``."
);

static PyObject* sorted_dict_type_update(PyObject* self, PyObject* const* args, Py_ssize_t nargs, PyObject* kwnames)
{
    return reinterpret_cast<SortedDictType*>(self)->update(args, nargs, kwnames);
}

PyDoc_STRVAR(
    sorted_dict_type_values_doc,
    "d.values() -> SortedDictValues\n"
    "Return a dynamic view on the values in the sorted dictionary ``d``."
);

static PyObject* sorted_dict_type_values(PyObject* self, PyObject* args)
{
    return reinterpret_cast<SortedDictType*>(self)->values(&sorted_dict_values_type);
}

static PyMethodDef sorted_dict_type_methods[] = {
    {
        .ml_name = "__class_getitem__",
        .ml_meth = Py_GenericAlias,
        .ml_flags = METH_O | METH_CLASS,
        .ml_doc = "See PEP 585.",
    },
    {
        .ml_name = "__reversed__",
        .ml_meth = sorted_dict_type_reversed,
        .ml_flags = METH_NOARGS,
        .ml_doc = sorted_dict_type_reversed_doc,
    },
    {
        .ml_name = "clear",
        .ml_meth = sorted_dict_type_clear,
        .ml_flags = METH_NOARGS,
        .ml_doc = sorted_dict_type_clear_doc,
    },
    {
        .ml_name = "copy",
        .ml_meth = sorted_dict_type_copy,
        .ml_flags = METH_NOARGS,
        .ml_doc = sorted_dict_type_copy_doc,
    },
    {
        .ml_name = "get",
        .ml_meth = reinterpret_cast<PyCFunction>(sorted_dict_type_get),
        .ml_flags = METH_FASTCALL,
        .ml_doc = sorted_dict_type_get_doc,
    },
    {
        .ml_name = "items",
        .ml_meth = sorted_dict_type_items,
        .ml_flags = METH_NOARGS,
        .ml_doc = sorted_dict_type_items_doc,
    },
    {
        .ml_name = "keys",
        .ml_meth = sorted_dict_type_keys,
        .ml_flags = METH_NOARGS,
        .ml_doc = sorted_dict_type_keys_doc,
    },
    {
        .ml_name = "setdefault",
        .ml_meth = reinterpret_cast<PyCFunction>(sorted_dict_type_setdefault),
        .ml_flags = METH_FASTCALL,
        .ml_doc = sorted_dict_type_setdefault_doc,
    },
    {
        // Using the fast calling convention speeds up the common case but
        // slows down the rare case (that of unpacking a dictionary into
        // keyword arguments) in CPython. See
        // https://github.com/python/cpython/pull/14589#issuecomment-509356084.
        // Since I ignore keyword arguments, the rare case is irrelevant.
        .ml_name = "update",
        .ml_meth = reinterpret_cast<PyCFunction>(sorted_dict_type_update),
        .ml_flags = METH_FASTCALL | METH_KEYWORDS,
        .ml_doc = sorted_dict_type_update_doc,
    },
    {
        .ml_name = "values",
        .ml_meth = sorted_dict_type_values,
        .ml_flags = METH_NOARGS,
        .ml_doc = sorted_dict_type_values_doc,
    },
    { nullptr },
};

PyDoc_STRVAR(
    sorted_dict_type_key_type_doc,
    "d.key_type: type | None\n"
    "The key type of the sorted dictionary ``d``, or ``None`` if no key-value pairs have been inserted in it."
);

static PyObject* sorted_dict_type_get_key_type(PyObject* self, void* closure)
{
    return reinterpret_cast<SortedDictType*>(self)->get_key_type();
}

static int sorted_dict_type_set_key_type(PyObject* self, PyObject* key_type, void* closure)
{
    return reinterpret_cast<SortedDictType*>(self)->set_key_type(key_type);
}

static PyGetSetDef sorted_dict_type_getset[] = {
    {
        .name = "key_type",
        .get = sorted_dict_type_get_key_type,
        .set = sorted_dict_type_set_key_type,
        .doc = sorted_dict_type_key_type_doc,
    },
    { nullptr },
};

/**
 * Initialise.
 */
static int sorted_dict_type_init(PyObject* self, PyObject* args, PyObject* kwargs)
{
    return reinterpret_cast<SortedDictType*>(self)->init(args, kwargs);
}

/**
 * Allocate.
 */
static PyObject* sorted_dict_type_new(PyTypeObject* type, PyObject* args, PyObject* kwargs)
{
    // In C++, the operator with the same name is used to allocate memory. I
    // could override it to call Python's memory allocator, but the latter
    // requires the Python type object, while the former receives only the
    // requested amount of memory. Hence, I am forced to use a custom method.
    return SortedDictType::New(type, args, kwargs);
}

static PyTypeObject sorted_dict_type = {
    // clang-format off
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0)
    .tp_name = "pysorteddict.SortedDict",
    // clang-format on
    .tp_basicsize = sizeof(SortedDictType),
    .tp_dealloc = sorted_dict_type_dealloc,
    .tp_repr = sorted_dict_type_repr,
    .tp_as_sequence = &sorted_dict_type_sequence,
    .tp_as_mapping = &sorted_dict_type_mapping,
    .tp_hash = PyObject_HashNotImplemented,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_flags = Py_TPFLAGS_BASETYPE | Py_TPFLAGS_DEFAULT | Py_TPFLAGS_DICT_SUBCLASS,
    .tp_doc = "Sorted dictionary: a dictionary in which the keys are always in ascending order.\n\n"
              "See https://tfpf.github.io/pysorteddict/documentation.html.",
    .tp_iter = sorted_dict_type_iter,
    .tp_methods = sorted_dict_type_methods,
    .tp_getset = sorted_dict_type_getset,
    .tp_init = sorted_dict_type_init,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = sorted_dict_type_new,
    .tp_free = PyObject_Free,
};

static int sorted_dict_module_exec(PyObject* mod)
{
    if (PyType_Ready(&sorted_dict_items_fwd_iter_type) < 0 || PyType_Ready(&sorted_dict_items_rev_iter_type) < 0
        || PyType_Ready(&sorted_dict_items_type) < 0 || PyType_Ready(&sorted_dict_keys_fwd_iter_type) < 0
        || PyType_Ready(&sorted_dict_keys_rev_iter_type) < 0 || PyType_Ready(&sorted_dict_keys_type) < 0
        || PyType_Ready(&sorted_dict_values_fwd_iter_type) < 0 || PyType_Ready(&sorted_dict_values_rev_iter_type) < 0
        || PyType_Ready(&sorted_dict_values_type) < 0 || PyType_Ready(&sorted_dict_type) < 0)
    {
        return -1;
    }
    if (PyModule_AddObjectRef(mod, "SortedDict", reinterpret_cast<PyObject*>(&sorted_dict_type)) < 0)  // 🆕
    {
        return -1;
    }
    return 0;
}

static PyModuleDef_Slot sorted_dict_module_slots[] = {
    { Py_mod_exec, reinterpret_cast<void*>(sorted_dict_module_exec) },
#if PY_VERSION_HEX >= 0x030C0000
    { Py_mod_multiple_interpreters, Py_MOD_MULTIPLE_INTERPRETERS_NOT_SUPPORTED },
#endif
#if PY_VERSION_HEX >= 0x030D0000
    { Py_mod_gil, Py_MOD_GIL_USED },
#endif
    { 0, nullptr },
};

static PyModuleDef sorted_dict_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "pysorteddict",
    .m_doc = "enriches Python with a sorted dictionary\n\n"
             "See https://tfpf.github.io/pysorteddict/.",
    .m_size = 0,
    .m_slots = sorted_dict_module_slots,
};

PyMODINIT_FUNC PyInit_pysorteddict(void)
{
    return PyModuleDef_Init(&sorted_dict_module);
}
