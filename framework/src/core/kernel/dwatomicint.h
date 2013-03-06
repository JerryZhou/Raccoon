#pragma once
#include <intrin.h>

class DW_CORE_EXPORT DwAtomicInt
{
public:
    DwAtomicInt() : m_value(0)
    {
    }

    DwAtomicInt(int v) : m_value(v)
    {
    }

    inline bool ref()
    {
        return _InterlockedIncrement((volatile LONG*)&m_value) != 0;
    }

    inline int deref()
    {
        return _InterlockedDecrement((volatile LONG*)&m_value);
    }

    inline bool operator!=(int value)
    {
        return m_value != value;
    }

    inline operator int() const
    {
        return m_value;
    }

private:
    volatile int m_value;
};
