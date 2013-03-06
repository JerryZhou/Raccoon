#pragma once

#include <eastl/vector.h>
#include "dwshareddata.h"

DW_CORE_EXPORT void _dw_vector_construct_hook();
DW_CORE_EXPORT void _dw_vector_destruct_hook();

template<class T>
class DwVectorData : public DwSharedData
{
public:
    eastl::vector<T> v;
};

template<class T>
class DwVector
{
public:
    typedef typename eastl::vector<T>::iterator iterator;
    typedef typename eastl::vector<T>::const_iterator const_iterator;

    DwVector()
    {
        _dw_vector_construct_hook();

        d = new DwVectorData<T>();
    }

    DwVector(const DwVector<T> &other) : d(other.d)
    {
        _dw_vector_construct_hook();
    }

    ~DwVector()
    {
        _dw_vector_destruct_hook();
    }

public:
    inline const_iterator begin() const
    {
        return d->v.begin();
    }
    inline const_iterator end() const
    {
        return d->v.end();
    }
    inline iterator begin()
    {
        return d->v.begin();
    }
    inline iterator end()
    {
        return d->v.end();
    }

    inline T &front()
    {
        return d->v.front();
    }
    inline const T &front() const
    {
        return d->front();
    }

    inline T &back()
    {
        return d->v.back();
    }
    inline const T &back() const
    {
        return d->v.back();
    }

    inline void push_back(const T &t)
    {
        append(t);
    }
    inline void push_front(const T &t)
    {
        prepend(t);
    }
    inline void append(const T &t)
    {
        d->v.push_back(t);
    }
    inline void prepend(const T &t)
    {
        d->v.push_front(t);
    }
    inline int count() const
    {
        return d->v.size();
    }
    inline int size() const
    {
        return d->v.size();
    }
    inline const T &at(int i) const
    {
        DW_ASSERT(i >= 0 && i < count());
        return d->v[i];
    }
    inline T &operator[](int i)
    {
        return d->v[i];
    }
    inline const T &operator[](int i) const
    {
        return at(i);
    }
    inline void clear()
    {
        d->v.clear();
    }
    inline bool isEmpty() const
    {
        return size() == 0;
    }
    inline void pop_front()
    {
        d->v.erase(d->v.begin());
    }
    inline void pop_back()
    {
        d->v.pop_back();
    }
    inline void insert(iterator i, const T &value)
    {
        d->v.insert(i, value);
    }
    inline void insert(int i, const T &value)
    {
        d->v.insert(begin() + i, value);
    }
    inline iterator erase(iterator it)
    {
        return d->v.erase(it);
    }
    inline void remove(int i)
    {
        d->v.erase(d->v.begin() + i);
    }
    inline void resize(int size)
    {
        d->v.resize(size);
    }
    inline void reserve(int size)
    {
        d->v.reserve(size);
    }

    int indexOf(const T &t) const
    {
        int c = count();
        for(int i = 0; i < c; i++)
        {
            if(d->v[i] == t)
            {
                return i;
            }
        }

        return -1;
    }

    bool removeOne(const T &t)
    {
        int i = indexOf(t);
        if(i != -1)
        {
            remove(i);
            return true;
        }

        return false;
    }

    int removeAll(const T &t)
    {
        int c = count();
        int removeCount = 0;
        for(int i = c-1; i >=0 ;)
        {
            if(d->v[i] == t)
            {
                remove(i);
                ++removeCount;
            }
            --i;
        }
        return removeCount;
    }



private:
    DwSharedDataPointer< DwVectorData<T> > d;
};
