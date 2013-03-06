#pragma once

template <class T>
class DwObjectPtr
{
    DwObject *o;

public:
    inline DwObjectPtr() : o(0) {}
    inline DwObjectPtr(T *p) : o(p)
    {
        DwObject::addGuard(&o);
    }
    inline DwObjectPtr(const DwObjectPtr<T> &p) : o(p.o)
    {
        DwObject::addGuard(&o);
    }
    inline ~DwObjectPtr()
    {
        if(o) DwObject::removeGuard(&o);
        o = NULL;
    }
    inline DwObjectPtr<T> &operator=(const DwObjectPtr<T> &p)
    {
        if (this != &p) DwObject::changeGuard(&o, p.o);
        o = p.o;
        return *this;
    }
    inline DwObjectPtr<T> &operator=(T* p)
    {
        if (o != p) DwObject::changeGuard(&o, p);
        o = p;
        return *this;
    }

    inline bool isNull() const
    {
        return !o;
    }

    inline operator bool()
    {
        return o != NULL;
    }

    inline T* operator->() const
    {
        return static_cast<T*>(const_cast<DwObject*>(o));
    }
    inline T& operator*() const
    {
        return *static_cast<T*>(const_cast<DwObject*>(o));
    }
    inline operator T*() const
    {
        return static_cast<T*>(const_cast<DwObject*>(o));
    }
    inline T* data() const
    {
        return static_cast<T*>(const_cast<DwObject*>(o));
    }
};
/*
//it doesn't work, since the old object could be deleted and new another in the same
address, but there are still some pointer pointed to the old one.
a single map cann't solve the problem in that way.
template <class T>
class DwObjectPointer
{
    DwObjectHolder *h;

public:
    inline DwObjectPointer() : h(0) {}
    inline DwObjectPointer(T *p) : h(0)
    { DwObject::addHolder(p, h); }
    inline DwObjectPointer(const DwObjectPointer<T> &p)
    { DwObject::addHolder(p.h->objectPtr, h); }
    inline ~DwObjectPointer()
    { if(h) DwObject::removeHolder(h->objectPtr); h = NULL; }
    inline DwObjectPointer<T> &operator=(const DwObjectPointer<T> &p)
    {
        if(this != &p)
        {
            if(h)
            {
                DwObject::removeHolder(h->objectPtr);
            }
            else
            {
                ++h->refCount;
            }
        }
        h = p.h;
        return *this;
    }
    inline DwObjectPointer<T> &operator=(T* p)
    {
        if (h)
        {
            DwObject::removeHolder(h->objectPtr);
        }
        DwObject::addHolder(p, h);
        return *this;
    }

    inline bool isNull() const
    { return h->objectPtr == NULL; }

    inline operator bool() { return h->objectPtr != NULL; }

    inline T* operator->() const
    { return static_cast<T*>(const_cast<DwObject*>(h->objectPtr)); }
    inline T& operator*() const
    { return *static_cast<T*>(const_cast<DwObject*>(h->objectPtr)); }
    inline operator T*() const
    { return static_cast<T*>(const_cast<DwObject*>(h->objectPtr)); }
    inline T* data() const
    { return static_cast<T*>(const_cast<DwObject*>(h->objectPtr)); }
};*/