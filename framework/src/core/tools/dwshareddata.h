#pragma once

#include <dwcore/dwatomicint.h>
#include <dwcore/dwmalloc.h>

template <typename T>
inline void dwqSwap(T &value1, T &value2)
{
    const T t = value1;
    value1 = value2;
    value2 = t;
}

class DW_CORE_EXPORT DwSharedData
{
public:
    mutable DwAtomicInt ref;

    inline DwSharedData() : ref(0) { }
    inline DwSharedData(const DwSharedData &) : ref(0) { }
    virtual ~DwSharedData() { };

    void *operator new(size_t s)
    {
        return dwMalloc(s);
    }
    void operator delete(void *p)
    {
        dwFree(p);
    }

private:
    // using the assignment operator would lead to corruption in the ref-counting
    DwSharedData &operator=(const DwSharedData &);
};

template <class T> class DwSharedDataPointer
{
public:
    typedef T Type;
    typedef T *pointer;

    inline void detach()
    {
        if (d && d->ref != 1) detach_helper();
    }
    inline T &operator*()
    {
        detach();
        return *d;
    }
    inline const T &operator*() const
    {
        return *d;
    }
    inline T *operator->()
    {
        detach();
        return d;
    }
    inline const T *operator->() const
    {
        return d;
    }
    inline operator T *()
    {
        detach();
        return d;
    }
    inline operator const T *() const
    {
        return d;
    }
    inline T *data()
    {
        detach();
        return d;
    }
    inline const T *data() const
    {
        return d;
    }
    inline const T *constData() const
    {
        return d;
    }

    inline bool operator==(const DwSharedDataPointer<T> &other) const
    {
        return d == other.d;
    }
    inline bool operator!=(const DwSharedDataPointer<T> &other) const
    {
        return d != other.d;
    }

    inline DwSharedDataPointer()
    {
        d = 0;
    }
    inline ~DwSharedDataPointer()
    {
        if (d && !d->ref.deref()) delete d;
    }

    explicit DwSharedDataPointer(T *data);
    inline DwSharedDataPointer(const DwSharedDataPointer<T> &o) : d(o.d)
    {
        if (d) d->ref.ref();
    }
    inline DwSharedDataPointer<T> & operator=(const DwSharedDataPointer<T> &o)
    {
        if (o.d != d)
        {
            if (o.d)
                o.d->ref.ref();
            T *old = d;
            d = o.d;
            if (old && !old->ref.deref())
                delete old;
        }
        return *this;
    }
    inline DwSharedDataPointer &operator=(T *o)
    {
        if (o != d)
        {
            if (o)
                o->ref.ref();
            T *old = d;
            d = o;
            if (old && !old->ref.deref())
                delete old;
        }
        return *this;
    }

    inline bool operator!() const
    {
        return !d;
    }

    inline void swap(DwSharedDataPointer &other)
    {
        dwqSwap(d, other.d);
    }

protected:
    T *clone()
    {
        return new T(*d);
    }

private:
    void detach_helper()
    {
        T *x = clone();
        x->ref.ref();
        if (!d->ref.deref())
            delete d;
        d = x;
    }

    T *d;
};

template <class T> class DwExplicitlySharedDataPointer
{
public:
    typedef T Type;
    typedef T *pointer;

    inline T &operator*() const
    {
        return *d;
    }
    inline T *operator->()
    {
        return d;
    }
    inline T *operator->() const
    {
        return d;
    }
    inline T *data() const
    {
        return d;
    }
    inline const T *constData() const
    {
        return d;
    }

    inline void detach()
    {
        if (d && d->ref != 1) detach_helper();
    }

    inline void reset()
    {
        if(d && !d->ref.deref())
            delete d;

        d = 0;
    }

    inline operator bool () const
    {
        return d != 0;
    }

    inline bool operator==(const DwExplicitlySharedDataPointer<T> &other) const
    {
        return d == other.d;
    }
    inline bool operator!=(const DwExplicitlySharedDataPointer<T> &other) const
    {
        return d != other.d;
    }
    inline bool operator==(const T *ptr) const
    {
        return d == ptr;
    }
    inline bool operator!=(const T *ptr) const
    {
        return d != ptr;
    }

    inline DwExplicitlySharedDataPointer()
    {
        d = 0;
    }
    inline ~DwExplicitlySharedDataPointer()
    {
        if (d && !d->ref.deref()) delete d;
    }

    explicit DwExplicitlySharedDataPointer(T *data);
    inline DwExplicitlySharedDataPointer(const DwExplicitlySharedDataPointer<T> &o) : d(o.d)
    {
        if (d) d->ref.ref();
    }

#ifndef QT_NO_MEMBER_TEMPLATES
    template<class X>
    inline DwExplicitlySharedDataPointer(const DwExplicitlySharedDataPointer<X> &o) : d(static_cast<T *>(o.data()))
    {
        if(d)
            d->ref.ref();
    }
#endif

    inline DwExplicitlySharedDataPointer<T> & operator=(const DwExplicitlySharedDataPointer<T> &o)
    {
        if (o.d != d)
        {
            if (o.d)
                o.d->ref.ref();
            T *old = d;
            d = o.d;
            if (old && !old->ref.deref())
                delete old;
        }
        return *this;
    }
    inline DwExplicitlySharedDataPointer &operator=(T *o)
    {
        if (o != d)
        {
            if (o)
                o->ref.ref();
            T *old = d;
            d = o;
            if (old && !old->ref.deref())
                delete old;
        }
        return *this;
    }

    inline bool operator!() const
    {
        return !d;
    }

    inline void swap(DwExplicitlySharedDataPointer &other)
    {
        dwSwap(d, other.d);
    }

protected:
    T *clone();

private:
    void detach_helper();

    T *d;
};


template <class T>
inline void dwqSwap(DwSharedDataPointer<T> &p1, DwSharedDataPointer<T> &p2)
{
    p1.swap(p2);
}

template <class T>
inline void dwqSwap(DwExplicitlySharedDataPointer<T> &p1, DwExplicitlySharedDataPointer<T> &p2)
{
    p1.swap(p2);
}
