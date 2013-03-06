#pragma once

#include "dwglobal.h"
#include "dwshareddata.h"
#include "eastl/set.h"

DW_CORE_EXPORT void _dw_set_construct_hook();
DW_CORE_EXPORT void _dw_set_destruct_hook();

template<class T>
class DwSetData : public DwSharedData
{
public:
    eastl::set<T> m;
};

template<class T>
class DwSet
{
public:
    typedef typename eastl::set<T>::iterator iterator;
    typedef typename eastl::set<T>::const_iterator const_iterator;

    DwSet()
    {
        _dw_set_construct_hook();

        d = new DwSetData<T>();
    }

    DwSet(const DwSet &other) : d(other.d)
    {
        _dw_set_construct_hook();
    }

    ~DwSet()
    {
        _dw_set_destruct_hook();
    }

    inline iterator begin()
    {
        return d->m.begin();
    }
    inline const_iterator begin() const
    {
        return d->m.begin();
    }
    inline iterator end()
    {
        return d->m.end();
    }
    inline const_iterator end() const
    {
        return d->m.end();
    }
    inline int count() const
    {
        return d->m.size();
    }
    inline bool isEmpty() const
    {
        return d->m.size() == 0;
    }
    inline void clear()
    {
        d->m.clear();
    }
    bool contains(const T &value) const
    {
        return find(value) != end();
    }

    iterator find(const T &value)
    {
        return d->m.find(value);
    }
    const_iterator find(const T &value) const
    {
        return d->m.find(value);
    }

    DwSet<T> &operator=( const DwSet<T> & other )
    {
        if (d != other.d)
        {
            d = other.d;
        }
        return *this;
    }

    void insert(const T &value)
    {
        d->m.insert(value);
    }

    void remove(const T &value)
    {
        iterator it = find(value);
        if(it != end())
        {
            d->m.erase(it);
        }
    }

    inline void erase(iterator it)
    {
        d->m.erase(it);
    }

private:
    DwSharedDataPointer< DwSetData<T> > d;
};
