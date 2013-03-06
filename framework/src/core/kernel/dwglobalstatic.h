#pragma once

template <typename T>
class DwGlobalStatic
{
public:
    DwGlobalStatic()
        : pointer(0)
        , destroyed(false)
    {
    }

    virtual ~DwGlobalStatic()
    {
        ;
    }

    T* volatile pointer;
    bool destroyed;
};

template <typename T>
class DwGlobalStaticDeleter
{
public:
    DwGlobalStatic<T> &globalStatic;
    DwGlobalStaticDeleter(DwGlobalStatic<T> &_globalStatic)
        : globalStatic(_globalStatic)
    {
    }

    inline ~DwGlobalStaticDeleter()
    {
        delete globalStatic.pointer;
        globalStatic.pointer = 0;
        globalStatic.destroyed = true;
    }
};

#define DW_GLOBAL_STATIC(TYPE, NAME) \
    static TYPE *NAME() \
    { \
        static DwGlobalStatic<TYPE> this_##NAME;\
        if (!this_##NAME.pointer && !this_##NAME.destroyed) \
        { \
           TYPE *x = new TYPE; \
           if(InterlockedCompareExchangePointer((volatile PVOID *)&this_##NAME.pointer, x, 0)) \
           { \
               delete x; \
           } \
           else \
           { \
               static DwGlobalStaticDeleter<TYPE > cleanup(this_##NAME); \
           } \
        } \
        return this_##NAME.pointer; \
    }

