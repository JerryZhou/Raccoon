#pragma once

#include "dwmalloc.h"

template<class _Ty>
class DwAllocator
{
public:
    typedef size_t     size_type;
    typedef ptrdiff_t  difference_type;
    typedef _Ty*       pointer;
    typedef const _Ty* const_pointer;
    typedef _Ty&       reference;
    typedef const _Ty& const_reference;
    typedef _Ty        value_type;

    template <class _Other> struct rebind
    {
        typedef DwAllocator<_Other> other;
    };

    DwAllocator() { }
    DwAllocator(const DwAllocator<_Ty>&) {}
    template <class _Other> DwAllocator(const DwAllocator<_Other>&) { }
    ~DwAllocator() { }

    _Ty* allocate(size_type _Count, const void* = 0)
    {
        return _Count != 0 ? static_cast<_Ty*>(dwMalloc(_Count * sizeof(_Ty))) : 0;
    }

    void deallocate(pointer _Ptr, size_type)
    {
        dwFree(_Ptr);
    }

    size_type max_size() const
    {
        return size_t(-1) / sizeof(_Ty);
    }

    void construct(pointer _Ptr, const _Ty& _Val)
    {
        ::new(_Ptr) _Ty(_Val);
    }

    void destroy(pointer _Ptr)
    {
        _Ptr->~_Ty();
    }
};
