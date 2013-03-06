#pragma once

#include "dwshareddata.h"
#include "eastl/list.h"

DW_CORE_EXPORT void _dw_linkedlist_construct_hook();
DW_CORE_EXPORT void _dw_linkedlist_destruct_hook();

template<class T>
class DwLinkedListData : public DwSharedData
{
public:
    eastl::list<T> l;
};

template<class T>
class DwLinkedList
{
public:
    typedef typename eastl::list<T>::iterator iterator;
    typedef typename eastl::list<T>::const_iterator const_iterator;

    DwLinkedList()
    {
        _dw_linkedlist_construct_hook();

        d = new DwLinkedListData<T>();
    }

    DwLinkedList(const DwLinkedList<T> &other) : d(other.d)
    {
        _dw_linkedlist_construct_hook();
    }

    ~DwLinkedList()
    {
        _dw_linkedlist_destruct_hook();
    }

    inline const_iterator begin() const
    {
        return d->l.begin();
    }
    inline const_iterator end() const
    {
        return d->l.end();
    }
    inline iterator begin()
    {
        return d->l.begin();
    }
    inline iterator end()
    {
        return d->l.end();
    }

    inline T &front()
    {
        return d->l.front();
    }
    inline const T &front() const
    {
        return d->front();
    }

    inline T &back()
    {
        return d->l.back();
    }
    inline const T &back() const
    {
        return d->back();
    }

    inline void push_back(const T &t)
    {
        append(t);
    }
    inline void push_front(const T &t)
    {
        prepend(t);
    }
    inline void pop_front()
    {
        removeFirst();
    }
    inline void pop_back()
    {
        removeLast();
    }
    inline void append(const T &t)
    {
        d->l.push_back(t);
    }
    inline void prepend(const T &t)
    {
        d->l.push_front(t);
    }
    inline int count() const
    {
        return d->l.size();
    }
    inline int size() const
    {
        return d->l.size();
    }
    inline void clear()
    {
        d->l.clear();
    }
    inline bool isEmpty() const
    {
        return size() == 0;
    }

    inline void removeFirst()
    {
        d->l.pop_front();
    }
    inline void removeLast()
    {
        d->l.pop_back();
    }
    inline iterator erase(iterator it)
    {
        return d->l.erase(it);
    }
    inline iterator erase(iterator itBegin, iterator itEnd)
    {
        return d->l.erase(itBegin, itEnd);
    }
    inline iterator insert(iterator before, const T &value)
    {
        return d->l.insert(before, value);
    }

    inline bool removeOne(const T &value)
    {
        d->l.remove(value);
        return true;
    }

private:
    DwSharedDataPointer< DwLinkedListData<T> > d;
};