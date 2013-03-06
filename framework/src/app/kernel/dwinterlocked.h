#pragma once
#include <intrin.h>

class DW_APP_EXPORT DwInterlocked
{
public:
    /// interlocked increment, return result
    static int increment(int volatile& var);
    /// interlocked decrement, return result
    static int decrement(int volatile& var);
    /// interlocked add
    static int add(int volatile& var, int a);
    /// interlocked exchange
    static int exchange(int volatile* dest, int value);
    /// interlocked compare-exchange
    static int compareExchange(int volatile* dest, int e, int comparand);
};

//------------------------------------------------------------------------------
/**
*/
__forceinline int
DwInterlocked::increment(int volatile& var)
{
    return _InterlockedIncrement((volatile LONG*)&var);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
DwInterlocked::decrement(int volatile& var)
{
    return _InterlockedDecrement((volatile LONG*)&var);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
DwInterlocked::add(int volatile& var, int a)
{
    return _InterlockedExchangeAdd((volatile LONG*)&var, a);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
DwInterlocked::exchange(int volatile* dest, int value)
{
    return _InterlockedExchange((volatile LONG*)dest, value);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
DwInterlocked::compareExchange(int volatile* dest, int e, int comparand)
{
    return _InterlockedCompareExchange((volatile LONG*)dest, e, comparand);
}