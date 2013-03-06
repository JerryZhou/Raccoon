#pragma once
#include "dwcore/dwmap.h"
#include "dwcore/dwvector.h"
#include "dwcore/dwlocks.h"
#include "dwcore/dwglobalstatic.h"

template<typename T>
class DwTHolder
{
    DwTHolder(const DwTHolder& other);
    DwTHolder& operator = (const DwTHolder& other);
public:
    DwTHolder(T* p = NULL) : ptr(NULL) , refCounted(0)
    {
        setT(p);
    }
    ~DwTHolder()
    {
        setT(NULL);
    }

    void setT(T* p)
    {
        ptr = p;
    }
    void addRef()
    {
        ++refCounted;
    }
    void release();
    int refCnt() const
    {
        return refCounted;
    }

    T* ptr;
    int refCounted;
};

template<typename T>
class DwTHolderCenter
{
public:
    typedef T* TPtr;
    typedef DwTHolder<T>* HPtr;
    typedef DwMap<TPtr, HPtr> HolderContainer;
    typedef DwVector<HPtr> HolderFreeList;

    DW_GLOBAL_STATIC(DwTHolderCenter<T>, Instance);

    DwTHolderCenter()
        : freeListCapacity(1000)
        , containerCapacity(1000)
    {
        ;
    }

    ~DwTHolderCenter()
    {
        HolderContainer::iterator ite = holderMap.begin();
        while (ite != holderMap.end())
        {
            delete ite->second;
            ite->second = NULL;
            ++ite;
        }

        while (holders.size() > 0)
        {
            HPtr h = holders.back();
            delete h;
            h = NULL;
            holders.pop_back();
        }
    }

    // in this the holder refcounted must be 0
    // but may be a null holder,
    // we can delete the holder at this place
    void recycleHolder(HPtr h)
    {
        if (h->ptr)
        {
            destroyHolder(h->ptr);
        }
    }

    HPtr requestHolder(const TPtr p)
    {
        DwCSLocker locks(&criticalSection);

        HolderContainer::iterator ite = holderMap.find(p);
        if (ite == holderMap.end())
        {
            HPtr h = allocHPtr();
            h->setT(p);
            holderMap[p] = h;
            return h;
        }
        else
        {
            return ite->second;
        }
    }

    bool destroyHolder(const TPtr p)
    {
        DwCSLocker locks(&criticalSection);

        HolderContainer::iterator ite = holderMap.find(p);
        if (ite != holderMap.end())
        {
            freeHPtr(ite->second);
            holderMap.remove(ite);
            return true;
        }
        return false;
    }

    HPtr allocHPtr()
    {
        DwCSLocker locks(&criticalSection);

        HPtr h = NULL;
        if (holders.size() > 0)
        {
            for (int i=holders.size()-1; i>=0; --i)
            {
                if(holders.at(i)->refCnt() == 0)
                {
                    h = holders.at(i);
                    holders.remove(i);
                    break;
                }
            }
        }

        if(!h)
        {
            h = new DwTHolder<T>();
        }

        return h;
    }

    void freeHPtr(HPtr h)
    {
        DwCSLocker locks(&criticalSection);

        if(!h) return;
        h->setT(NULL);
        holders.push_back(h);

        shrinkageFreeList();
    }

    void shrinkageFreeList()
    {
        DwCSLocker locks(&criticalSection);

        if (holders.size() > freeListCapacity)
        {
            bool expandFreeListCapacity = true;
            for (int i=holders.size()-1; i>=0; --i)
            {
                if(holders.at(i)->refCnt() == 0)
                {
                    expandFreeListCapacity = false;
                    delete holders.at(i);
                    holders.remove(i);
                }
            }

            if (expandFreeListCapacity)
            {
                freeListCapacity = 2 * freeListCapacity;
            }
            else
            {
                freeListCapacity = dwMax<size_t>(holders.size(), 1000);
            }
        }
    }

    void shrinkageContainer()
    {
        DwCSLocker locks(&criticalSection);

        if (holderMap.count() > containerCapacity)
        {
            bool expandCapactiy = true;

            HolderContainer::iterator ite = holderMap.begin();
            while (ite != holderMap.end())
            {
                if (ite->second->refCnt() == 0)
                {
                    expandCapactiy = false;
                    HolderContainer::iterator eraseIte = ite++;
                    eraseIte->second->setT(NULL);
                    holders.push_back(eraseIte->second);
                    holderMap.remove(eraseIte);
                }
                else
                {
                    ++ite;
                }
            }

            if (expandCapactiy)
            {
                containerCapacity = containerCapacity * 2;
            }
            else
            {
                containerCapacity = dwMax<size_t>(holderMap.count(), 1000);
            }
        }
    }

    HolderContainer holderMap;
    HolderFreeList holders;
    int freeListCapacity;
    int containerCapacity;
    DwCriticalSection criticalSection;
};

template<typename T>
void DwTHolder<T>::release()
{
    --refCounted;
    if(refCounted == 0)
    {
        DwTHolderCenter<T>::Instance()->recycleHolder(this);
    }
}

template<typename T, typename TBase = T>
class DwWeakPtr
{
public:
    DwWeakPtr() : holderPtr(NULL)
    {
        ;
    }
    DwWeakPtr(const T* ptr) : holderPtr(NULL)
    {
        if (ptr)
        {
            holderPtr = DwTHolderCenter<TBase>::Instance()->requestHolder((T*)(ptr));
            // We Can set a max holder num
            if (holderPtr)
            {
                holderPtr->addRef();
            }
        }
    }
    DwWeakPtr(const DwWeakPtr& other) : holderPtr(NULL)
    {
        *this = other;
    }
    DwWeakPtr& operator = (const DwWeakPtr& other)
    {
        if (&other != this)
        {
            release();
            // just add ref to valid object
            if(other.holderPtr && other.holderPtr->ptr)
            {
                this->holderPtr = other.holderPtr;
                this->holderPtr->addRef();
            }

        }
        return *this;
    }

    ~DwWeakPtr()
    {
        release();
    }

    /*inline T* operator->() const
    {
    	if (holderPtr) { return static_cast<T*>(holderPtr->ptr); }
    	return NULL;
    }

    inline T& operator*() const
    {
    	if (holderPtr) { return *(static_cast<T*>(holderPtr->ptr)); }
    	return *static_cast<T*>(NULL);
    }

    inline operator T*() const
    {
    	if (holderPtr) { return static_cast<T*>(holderPtr->ptr); }
    	return NULL;
    }*/

    inline void release()
    {
        if (holderPtr)
        {
            holderPtr->release();
            holderPtr = NULL;
        }
    }
    inline bool isNull()const
    {
        return !holderPtr || !(holderPtr->ptr);
    }
    inline operator bool()
    {
        return !isNull();
    }

protected:
    DwTHolder<TBase>* holderPtr;
};