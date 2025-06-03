#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <map>
#include <utility>

#include "sorted_dict_type.hh"
#include "sorted_dict_utils.hh"
#include "sorted_dict_view_type.hh"

/**
 * Make the number of known referrers of the sorted dictionary and/or its
 * relevant key-value pair consistent.
 *
 * The caller should ensure that this function is called immediately after the
 * iterator is updated, and should adjust the number of known referrers of
 * other key-value pairs itself.
 */
void SortedDictViewIterType::make_consistent(void){
    if(this->it == this->sd->map->begin()){
        Py_INCREF(this->sd);
        ++this->sd->known_referrers;
        this->should_raise_stop_iteration = false;
    }
    if(this->it != this->sd->map->end())
    {
        ++this->it->second.known_referrers;
    }else{
        this->should_raise_stop_iteration = true;
        --this->sd->known_referrers;
        Py_DECREF(this->sd);
    }
}

void SortedDictViewIterType::deinit(void)
{
    if(this->should_raise_stop_iteration){
        return;
    }
    if(this->it != this->sd->map->end()){
        --this->it->second.known_referrers;
    }
    --this->sd->known_referrers;
}

std::pair<PyObject*, PyObject*> SortedDictViewIterType::next(void)
{
    static std::pair<PyObject* const, PyObject*> sentinel = { nullptr, nullptr };
    if(this->stop_iteration_raised || this->it == this->sd->map->end()){
        if(!this->stop_iteration_raised){
            // Match the behaviour of built-in iterators: drop the held
            // reference when raising `StopIteration` for the first time.
            --this->sd->known_referrers;
            Py_DECREF(this->sd);
            this->stop_iteration_raised = true;
        }
        return sentinel;
    }

    // The 'next' key-value pair is the current one the iterator points to.
    auto curr = this->it++;
    --curr->second.known_referrers;
    if(this->it != this->sd->map->end()){
        ++this->it->second.known_referrers;
    }
    return {curr->first, curr->second.value};
}

PyObject* SortedDictViewIterType::New(PyTypeObject* type, SortedDictType* sd)
{
    PyObject* self = type->tp_alloc(type, 0);  // 🆕
    if (self == nullptr)
    {
        return nullptr;
    }

    SortedDictViewIterType* sdvi = reinterpret_cast<SortedDictViewIterType*>(self);
    sdvi->sd = sd;
    sdvi->it = sdvi->sd->map->begin();
    sdvi->make_consistent();
    return self;
}

void SortedDictViewType::deinit(void)
{
    Py_DECREF(this->sd);
}

PyObject* SortedDictViewType::repr(char const* name, PyObject* ob)
{
    PyObjectWrapper ob_list(PySequence_List(ob));  // 🆕
    if (ob_list == nullptr)
    {
        return nullptr;
    }
    return PyUnicode_FromFormat("%s(%R)", name, ob_list.get());
}

Py_ssize_t SortedDictViewType::len(void)
{
    return this->sd->map->size();
}

PyObject* SortedDictViewType::iter(PyTypeObject* type)
{
    return SortedDictViewIterType::New(type, this->sd);
}

PyObject* SortedDictViewType::New(PyTypeObject* type, SortedDictType* sd)
{
    PyObject* self = type->tp_alloc(type, 0);  // 🆕
    if (self == nullptr)
    {
        return nullptr;
    }

    SortedDictViewType* sdv = reinterpret_cast<SortedDictViewType*>(self);
    sdv->sd = sd;
    Py_INCREF(sdv->sd);
    return self;
}
