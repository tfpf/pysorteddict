#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include "sorted_dict_items_type.hh"
#include "sorted_dict_keys_type.hh"
#include "sorted_dict_type.hh"
#include "sorted_dict_utils.hh"
#include "sorted_dict_values_type.hh"
#include "sorted_dict_view_type.hh"

/**
 * Import a Python type.
 *
 * The caller should ensure that the module and type names actually name a
 * Python type.
 *
 * @param module_name Name of the module to import the type from.
 * @param type_name Name of the type to import.
 *
 * @return Python type object if successful, else `nullptr`.
 */
static PyTypeObject* import_python_type(char const* module_name, char const* type_name)
{
    PyErrorClearer _;
    PyObjectWrapper module_ob(PyImport_ImportModule(module_name));  // 🆕
    if (module_ob == nullptr)
    {
        return nullptr;
    }
    PyObject* type_ob = PyObject_GetAttrString(module_ob.get(), type_name);  // 🆕
    if (type_ob == nullptr)
    {
        return nullptr;
    }
    if (PyObject_IsInstance(type_ob, reinterpret_cast<PyObject*>(&PyType_Type)) != 1)
    {
        return nullptr;
    }
    return reinterpret_cast<PyTypeObject*>(type_ob);
}

// Key types which have to be imported explicitly using the above function.
static PyTypeObject* PyDate_Type;
static PyTypeObject* PyTimeDelta_Type;
static PyTypeObject* PyDecimal_Type;
static PyTypeObject* PyFraction_Type;
static PyTypeObject* PyIPv4Address_Type;
static PyTypeObject* PyIPv4Interface_Type;
static PyTypeObject* PyIPv4Network_Type;
static PyTypeObject* PyIPv6Address_Type;
static PyTypeObject* PyIPv6Interface_Type;
static PyTypeObject* PyIPv6Network_Type;
static PyTypeObject* PyPosixPath_Type;
static PyTypeObject* PyPurePosixPath_Type;
static PyTypeObject* PyPureWindowsPath_Type;
static PyTypeObject* PyWindowsPath_Type;
static PyTypeObject* PyStructTime_Type;
static PyTypeObject* PyUUID_Type;

/**
 * Check whether the given key can be inserted into this sorted dictionary. For
 * instance, NaN cannot be compared with other floating-point numbers, so it
 * cannot be inserted.
 *
 * The caller should ensure that the key type is set and that it matches the
 * type of the given key prior to calling this method.
 *
 * @param key Key.
 *
 * @return `true` if the check succeeds, else `false`.
 */
bool SortedDictType::is_key_good(PyObject* key)
{
    if (this->key_type == &PyFloat_Type)
    {
        return !std::isnan(PyFloat_AS_DOUBLE(key));
    }
    if (this->key_type == PyDecimal_Type)
    {
        PyErrorClearer _;
        PyObjectWrapper key_is_nan(PyObject_CallMethod(key, "is_nan", nullptr));  // 🆕
        if (key_is_nan == nullptr)
        {
            return false;
        }
        return Py_IsFalse(key_is_nan.get());
    }
    return true;
}

/**
 * Check whether the key type and the given key-value pair satisfy one of the
 * following conditions.
 *
 * 1. The key type is not set; the given key is of a supported type and has a
 *    good value (defined above); and the given value is not null.
 *
 * 2. The key type is set; and the given key is of that type and has a good
 *    value.
 *
 * On success, set the key type to the type of the given key if it was not set.
 * On failure, set a Python exception.
 *
 * @param key Key.
 * @param value Value.
 *
 * @return `true` if the check succeeds, else `false`.
 */
bool SortedDictType::are_key_type_and_key_value_pair_good(PyObject* key, PyObject* value)
{
    bool key_type_set_here = false;
    if (this->key_type == nullptr)
    {
        if (value == nullptr)
        {
            // No key-value pairs have been inserted, so can't get or delete
            // anything.
            PyErr_SetString(PyExc_RuntimeError, "key type not set: insert at least one item first");
            return false;
        }

        // The first key-value pair is being inserted.
        if (this->set_key_type(reinterpret_cast<PyObject*>(Py_TYPE(key)), key) == -1)
        {
            return false;
        }
        key_type_set_here = true;
    }

    // At this point, the key type is guaranteed to be non-null.
    if (!key_type_set_here && !Py_IS_TYPE(key, this->key_type))
    {
        PyErr_Format(PyExc_TypeError, "got key %R of type %R, want key of type %R", key, Py_TYPE(key), this->key_type);
        return false;
    }

    // At this point, the key is guaranteed to be of the correct type. Hence,
    // it is safe to call this method.
    if (!this->is_key_good(key))
    {
        PyErr_Format(PyExc_ValueError, "got bad key %R of type %R", key, Py_TYPE(key));
        if (key_type_set_here)
        {
            // Insertion of the key-value pair was unsuccessful, so clear the
            // key type.
            this->key_type = nullptr;
        }
        return false;
    }
    return true;
}

/**
 * Check whether every key-value pair in this sorted dictionary can be deleted.
 *
 * @return `true` if the check succeeds, else `false`.
 */
bool SortedDictType::is_deletion_allowed(void)
{
    if (this->known_referrers != 0)
    {
        PyErr_Format(
            PyExc_RuntimeError, "operation not permitted: sorted dictionary locked by %zd iterator(s)",
            this->known_referrers
        );
        return false;
    }
    return true;
}

/**
 * Check whether a key-value pair in this sorted dictionary can be deleted.
 *
 * @param kv_known_referrers Known referrers of the key-value pair.
 *
 * @return `true` if the check succeeds, else `false`.
 */
bool SortedDictType::is_deletion_allowed(Py_ssize_t kv_known_referrers)
{
    if (kv_known_referrers != 0)
    {
        PyErr_Format(
            PyExc_RuntimeError, "operation not permitted: key-value pair locked by %zd iterator(s)", kv_known_referrers
        );
        return false;
    }
    return true;
}

/**
 * Check whether the number of arguments falls within the specified range.
 *
 * @param caller Caller requesting the check.
 * @param nargs Number of arguments.
 * @param at_least Minimum number of arguments.
 * @param at_most Maximum number of arguments.
 *
 * @return `true` if the check succeeds, else `false`.
 */
bool SortedDictType::is_nargs_good(char const* caller, Py_ssize_t nargs, int at_least, int at_most)
{
    if (nargs < at_least || at_most < nargs)
    {
        PyErr_Format(
            PyExc_TypeError, "%s() takes %d to %d positional arguments (%zd given)", caller, at_least, at_most, nargs
        );
        return false;
    }
    return true;
}

/**
 * Update the sorted dictionary with the keys and values from the given
 * mapping.
 *
 * @param mp Mapping.
 *
 * @return `true` if successful, else `false`.
 */
bool SortedDictType::update_from_mapping(PyObject* mp)
{
    return true;
}

/**
 * Update the sorted dictionary with the keys and values from the given
 * sequence.
 *
 * @param sq Sequence.
 *
 * @return `true` if successful, else `false`.
 */
bool SortedDictType::update_from_sequence(PyObject* sq)
{
    return true;
}

/**
 * Update the sorted dictionary with the keys and values from the given object.
 *
 * @param ob Object.
 *
 * @return `true` if successful, else `false`.
 */
bool SortedDictType::update_from_object(PyObject* ob)
{
    return PyObject_HasAttrString(ob, "keys") ? this->update_from_mapping(ob) : this->update_from_sequence(ob);
}

/**
 * Update the sorted dictionary with the keys and values provided.
 *
 * This method currently does nothing and always succeeds.
 *
 * @param keys Python tuple of keys.
 * @param values C array of values.
 *
 * @return `true` if successful, else `false`.
 */
bool SortedDictType::update_from_key_value_pairs(PyObject* keys, PyObject* const* values)
{
    return true;
}

void SortedDictType::Delete(PyObject* self)
{
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
    for (auto& item : *sd->map)
    {
        Py_DECREF(item.first);
        Py_DECREF(item.second.value);
    }
    delete sd->map;
    Py_TYPE(self)->tp_free(self);
}

PyObject* SortedDictType::repr(void)
{
    char const* delimiter = "";
    char const* actual_delimiter = ", ";
    std::string this_repr_utf8 = "SortedDict" LEFT_PARENTHESIS LEFT_CURLY_BRACKET;
    for (auto& item : *this->map)
    {
        PyObjectWrapper key_repr(PyObject_Repr(item.first));  // 🆕
        if (key_repr == nullptr)
        {
            return nullptr;
        }
        PyObjectWrapper value_repr(PyObject_Repr(item.second.value));  // 🆕
        if (value_repr == nullptr)
        {
            return nullptr;
        }
        Py_ssize_t key_repr_size, value_repr_size;
        char const* key_repr_utf8 = PyUnicode_AsUTF8AndSize(key_repr.get(), &key_repr_size);
        char const* value_repr_utf8 = PyUnicode_AsUTF8AndSize(value_repr.get(), &value_repr_size);
        this_repr_utf8.append(delimiter)
            .append(key_repr_utf8, key_repr_size)
            .append(": ")
            .append(value_repr_utf8, value_repr_size);
        delimiter = actual_delimiter;
    }
    this_repr_utf8.append(RIGHT_CURLY_BRACKET RIGHT_PARENTHESIS);
    return PyUnicode_FromStringAndSize(this_repr_utf8.data(), this_repr_utf8.size());  // 🆕
}

/**
 * Check whether a key is present. Also check whether it is mapped to the given
 * value if it is provided.
 *
 * @param key Key.
 * @param value Value.
 *
 * @return -1 on error. 1 if it is present and mapped, else 0.
 */
int SortedDictType::contains(PyObject* key, PyObject* value)
{
    if (!this->are_key_type_and_key_value_pair_good(key))
    {
        return -1;
    }
    auto it = this->map->find(key);
    if (it == this->map->end())
    {
        return 0;
    }
    return value == nullptr ? 1 : PyObject_RichCompareBool(it->second.value, value, Py_EQ);
}

Py_ssize_t SortedDictType::len(void)
{
    auto sz = this->map->size();
    if (std::cmp_greater(sz, PY_SSIZE_T_MAX))
    {
        PyErr_Format(
            PyExc_OverflowError, "sorted dictionary length is %zu which exceeds PY_SSIZE_T_MAX = %zd", sz,
            PY_SSIZE_T_MAX
        );
        return -1;
    }
    return sz;
}

/**
 * Find the value mapped to a key. On failure, set a Python exception.
 *
 * @param key Key.
 *
 * @return Value if present, else `nullptr`.
 */
PyObject* SortedDictType::getitem(PyObject* key)
{
    if (!this->are_key_type_and_key_value_pair_good(key))
    {
        return nullptr;
    }
    auto it = this->map->find(key);
    if (it == this->map->end())
    {
        PyErr_SetObject(PyExc_KeyError, key);
        return nullptr;
    }
    return Py_NewRef(it->second.value);  // 🆕
}

/**
 * Map a value to a key or remove a key-value pair. On failure, set a Python
 * exception.
 *
 * @param key Key.
 * @param value Value.
 *
 * @return 0 if a key-value pair was inserted or removed, else -1.
 */
int SortedDictType::setitem(PyObject* key, PyObject* value)
{
    if (!this->are_key_type_and_key_value_pair_good(key, value))
    {
        return -1;
    }

    // Insertion will be faster if the approximate location is known. Hence,
    // look for the nearest match.
    auto it = this->map->lower_bound(key);
    bool found = it != this->map->end() && !this->map->key_comp()(key, it->first);

    if (value == nullptr)
    {
        // Remove the key-value pair.
        if (!found)
        {
            PyErr_SetObject(PyExc_KeyError, key);
            return -1;
        }
        if (!this->is_deletion_allowed(it->second.known_referrers))
        {
            return -1;
        }
        Py_DECREF(it->first);
        Py_DECREF(it->second.value);
        this->map->erase(it);
        return 0;
    }

    // Map the value to the key. This merely stores additional references to
    // the key (if applicable) and the value. If I ever plan to allow mutable
    // types as keys, I should store references to their copies instead. Like
    // the C++ standard library containers do.
    if (!found)
    {
        // Insert a new key-value pair. The hint is correct; the key will get
        // inserted just before it.
        this->map->emplace_hint(it, Py_NewRef(key), value);  // 🆕
    }
    else
    {
        // Replace the previously-mapped value.
        Py_DECREF(it->second.value);
        it->second.value = value;
    }
    Py_INCREF(value);  // 🆕
    return 0;
}

PyObject* SortedDictType::iter(PyTypeObject* type)
{
    return SortedDictKeysIterType<FwdIterType>::New(type, this);
}

PyObject* SortedDictType::reversed(PyTypeObject* type)
{
    return SortedDictKeysIterType<RevIterType>::New(type, this);
}

PyObject* SortedDictType::clear(void)
{
    if (!this->is_deletion_allowed())
    {
        return nullptr;
    }
    for (auto& item : *this->map)
    {
        Py_DECREF(item.first);
        Py_DECREF(item.second.value);
    }
    this->map->clear();
    Py_RETURN_NONE;
}

PyObject* SortedDictType::copy(void)
{
    PyTypeObject* type = Py_TYPE(this);
    PyObject* sd_copy = type->tp_alloc(type, 0);  // 🆕
    if (sd_copy == nullptr)
    {
        return nullptr;
    }
    SortedDictType* this_copy = reinterpret_cast<SortedDictType*>(sd_copy);
    this_copy->map = new std::map<PyObject*, SortedDictValue, SortedDictKeyCompare>(*this->map);
    for (auto& item : *this_copy->map)
    {
        Py_INCREF(item.first);  // 🆕
        Py_INCREF(item.second.value);  // 🆕
        item.second.known_referrers = 0;
    }
    this_copy->key_type = this->key_type;
    this_copy->known_referrers = 0;
    return sd_copy;
}

PyObject* SortedDictType::get(PyObject* const* args, Py_ssize_t nargs)
{
    if (!this->is_nargs_good(__func__, nargs, 1, 2))
    {
        return nullptr;
    }
    PyObject* key = args[0];
    if (!this->are_key_type_and_key_value_pair_good(key))
    {
        return nullptr;
    }
    auto it = this->map->find(key);
    if (it != this->map->end())
    {
        return Py_NewRef(it->second.value);  // 🆕
    }
    PyObject* Default = nargs > 1 ? args[1] : Py_None;
    return Py_NewRef(Default);  // 🆕
}

PyObject* SortedDictType::items(PyTypeObject* type)
{
    return SortedDictItemsType::New(type, this);
}

PyObject* SortedDictType::keys(PyTypeObject* type)
{
    return SortedDictKeysType::New(type, this);
}

PyObject* SortedDictType::setdefault(PyObject* const* args, Py_ssize_t nargs)
{
    if (!this->is_nargs_good(__func__, nargs, 1, 2))
    {
        return nullptr;
    }
    PyObject* key = args[0];
    if (!this->are_key_type_and_key_value_pair_good(key))
    {
        return nullptr;
    }
    auto it = this->map->lower_bound(key);
    bool found = it != this->map->end() && !this->map->key_comp()(key, it->first);
    if (found)
    {
        return Py_NewRef(it->second.value);  // 🆕
    }
    PyObject* Default = nargs > 1 ? args[1] : Py_None;
    this->map->emplace_hint(it, Py_NewRef(key), Py_NewRef(Default));  // 🆕
    return Py_NewRef(Default);  // 🆕
}

PyObject* SortedDictType::update(PyObject* const* args, Py_ssize_t nargs, PyObject* kwnames)
{
    if (!this->is_nargs_good(__func__, nargs, 0, 1))
    {
        return nullptr;
    }
    if (nargs == 1 && !this->update_from_object(args[0]))
    {
        return nullptr;
    }
    if (kwnames != nullptr && !this->update_from_key_value_pairs(kwnames, args + nargs))
    {
        return nullptr;
    }
    Py_RETURN_NONE;
}

PyObject* SortedDictType::values(PyTypeObject* type)
{
    return SortedDictValuesType::New(type, this);
}

PyObject* SortedDictType::get_key_type(void)
{
    if (this->key_type == nullptr)
    {
        Py_RETURN_NONE;
    }
    return Py_NewRef(this->key_type);  // 🆕
}

int SortedDictType::set_key_type(PyObject* key_type, PyObject* key)
{
    if (key_type == nullptr)
    {
        PyErr_SetString(PyExc_AttributeError, "cannot delete attribute");
        return -1;
    }

    if (this->key_type != nullptr)
    {
        if (Py_Is(key_type, reinterpret_cast<PyObject*>(this->key_type)))
        {
            return 0;
        }
        PyErr_Format(PyExc_AttributeError, "cannot change key type from %R to %R", this->key_type, key_type);
        return -1;
    }

    static PyTypeObject* allowed_key_types[] = {
        &PyBool_Type,
        &PyBytes_Type,
        &PyFloat_Type,
        &PyLong_Type,
        &PyUnicode_Type,
        // The following types are not built-in.
        PyDate_Type = import_python_type("datetime", "date"),
        PyTimeDelta_Type = import_python_type("datetime", "timedelta"),
        PyDecimal_Type = import_python_type("decimal", "Decimal"),
        PyFraction_Type = import_python_type("fractions", "Fraction"),
        PyIPv4Address_Type = import_python_type("ipaddress", "IPv4Address"),
        PyIPv4Interface_Type = import_python_type("ipaddress", "IPv4Interface"),
        PyIPv4Network_Type = import_python_type("ipaddress", "IPv4Network"),
        PyIPv6Address_Type = import_python_type("ipaddress", "IPv6Address"),
        PyIPv6Interface_Type = import_python_type("ipaddress", "IPv6Interface"),
        PyIPv6Network_Type = import_python_type("ipaddress", "IPv6Network"),
        PyPosixPath_Type = import_python_type("pathlib", "PosixPath"),
        PyPurePosixPath_Type = import_python_type("pathlib", "PurePosixPath"),
        PyPureWindowsPath_Type = import_python_type("pathlib", "PureWindowsPath"),
        PyWindowsPath_Type = import_python_type("pathlib", "WindowsPath"),
        PyStructTime_Type = import_python_type("time", "struct_time"),
        PyUUID_Type = import_python_type("uuid", "UUID"),
    };
    for (PyTypeObject* allowed_key_type : allowed_key_types)
    {
        if (allowed_key_type != nullptr && Py_Is(key_type, reinterpret_cast<PyObject*>(allowed_key_type)))
        {
            this->key_type = allowed_key_type;
            return 0;
        }
    }

    if (key != nullptr)
    {
        // The user supplied a key of the wrong type.
        PyErr_Format(PyExc_TypeError, "got key %R of unsupported type %R", key, key_type);
    }
    else
    {
        // The user supplied a wrong value (which should have been a supported
        // type).
        PyErr_Format(PyExc_ValueError, "got %R, want a supported key type", key_type);
    }
    return -1;
}

int SortedDictType::init(PyObject* args, PyObject* kwargs)
{
    // All initialisation is done immediately after allocation in order to
    // avoid leaving essential members uninitialised. This method is kept to
    // allow adding functionality in the future.
    return 0;
}

PyObject* SortedDictType::New(PyTypeObject* type, PyObject* args, PyObject* kwargs)
{
    PyObject* self = type->tp_alloc(type, 0);  // 🆕
    if (self == nullptr)
    {
        return nullptr;
    }

    // Python's default allocator claims to initialise the contents of the
    // allocated memory to null, but actually writes zeros to it. Hence,
    // explicitly initialise them.
    SortedDictType* sd = reinterpret_cast<SortedDictType*>(self);
    sd->map = new std::map<PyObject*, SortedDictValue, SortedDictKeyCompare>;
    sd->key_type = nullptr;
    sd->known_referrers = 0;
    return self;
}
