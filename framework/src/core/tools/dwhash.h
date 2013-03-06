#pragma once

#include "dwshareddata.h"
#include "eastl/hash_map.h"
#include "dwcore/dwstring.h"
#include "dwcore/dwbytearray.h"
#include "dwcore/dwatomicint.h"

DW_CORE_EXPORT void _dw_hash_construct_hook();
DW_CORE_EXPORT void _dw_hash_destruct_hook();

template<class Key, class T>
class DwHashData : public DwSharedData
{
public:
    eastl::hash_map<Key, T> m;
};

template<class Key, class T>
class DwHash
{
public:
    typedef typename eastl::hash_map<Key, T>::iterator iterator;
    typedef typename eastl::hash_map<Key, T>::const_iterator const_iterator;

    DwHash()
    {
        _dw_hash_construct_hook();

        d = new DwHashData<Key, T>();
    }

    DwHash(const DwHash &other) : d(other.d)
    {
        _dw_hash_construct_hook();
    }

    ~DwHash()
    {
        _dw_hash_destruct_hook();
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
        return d->m[key];
    }

    DwHash<Key, T> &operator=( const DwHash<Key, T> & other )
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

    const T value(const Key &key) const
    {
        return d->m.find(key)->second;
    }

    const T value(const Key &key, const T &defaultValue) const
    {
        const_iterator it = d->m.find(key);
        if(it == end())
        {
            return defaultValue;
        }
        return it->second;
    }
    inline void clear()
    {
        d->m.clear();
    }

private:
    DwSharedDataPointer< DwHashData<Key, T> > d;
};

//------------------------------------------------------------------------------
namespace eastl
{
    template <>
    struct DW_CORE_EXPORT hash<DwString>
    {
        size_t operator()(DwString val) const;
    };
    template <>
    struct DW_CORE_EXPORT hash<DwByteArray>
    {
        size_t operator()(DwByteArray val) const;
    };
    template <>
    struct DW_CORE_EXPORT hash<DwAtomicInt>
    {
        size_t operator()(DwAtomicInt val) const;
    };
}