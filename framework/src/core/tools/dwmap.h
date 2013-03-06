#pragma once

#include "dwshareddata.h"
#include "eastl/map.h"

DW_CORE_EXPORT void _dw_map_construct_hook();
DW_CORE_EXPORT void _dw_map_destruct_hook();

template<class Key, class T>
class DwMapData : public DwSharedData
{
public:
    eastl::map<Key, T> m;
};

template<class Key, class T>
class DwMap
{
public:
    typedef typename eastl::map<Key, T>::iterator iterator;
    typedef typename eastl::map<Key, T>::const_iterator const_iterator;

    DwMap()
    {
        _dw_map_construct_hook();

        d = new DwMapData<Key, T>();
    }

    DwMap(const DwMap &other) : d(other.d)
    {
        _dw_map_construct_hook();
    }

    ~DwMap()
    {
        _dw_map_destruct_hook();
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

    bool contains(const Key &key) const
    {
        return find(key) != end();
    }

    iterator find(const Key &key)
    {
        return d->m.find(key);
    }
    const_iterator find(const Key &key) const
    {
        return d->m.find(key);
    }

    T &operator[](const Key &key)
    {
        return d->m[key];
    }
    const T operator[](const Key &key) const
    {
        return (*find(key)).second;
    }

    DwMap<Key, T> &operator=( const DwMap<Key, T> & other )
    {
        if (d != other.d)
        {
            d = other.d;
        }
        return *this;
    }

    void insert(const Key &key, const T &value)
    {
        d->m.insert(eastl::pair<Key, T>(key, value));
    }

    void remove(const Key &key)
    {
        iterator it = find(key);
        if(it != end())
        {
            d->m.erase(it);
        }
    }

    iterator remove(iterator it)
    {
        return d->m.erase(it);
    }

    const T& value(const Key &key) const
    {
        return d->m.find(key)->second;
    }

    const T& value(const Key &key, const T &defaultValue) const
    {
        const_iterator it = d->m.find(key);
        if(it == end())
        {
            return defaultValue;
        }
        return it->second;
    }

private:
    DwSharedDataPointer< DwMapData<Key, T> > d;
};
