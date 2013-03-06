#include "stable.h"
#include "dwelapsedtimer.h"

//------------------------------------------------------------------------------
/**
*/
typedef ULONGLONG (WINAPI *PtrGetTickCount64)(void);
static PtrGetTickCount64 ptrGetTickCount64 = 0;
static const int64_t invalidData = DW_INT64_C(0x8000000000000000);

//------------------------------------------------------------------------------
/**
*/
void DwElapsedTimer::invalidate()
{
    t1 = t2 = invalidData;
}

//------------------------------------------------------------------------------
/**
*/
bool DwElapsedTimer::isValid() const
{
    return t1 != invalidData && t2 != invalidData;
}

//------------------------------------------------------------------------------
/**
*/
bool DwElapsedTimer::hasExpired(int64_t timeout) const
{
    // if timeout is -1, int64_t(timeout) is LLINT_MAX, so this will be
    // considered as never expired
    return int64_t(elapsed()) > int64_t(timeout);
}

//------------------------------------------------------------------------------
/**
*/
static void resolveLibs()
{
    static bool done = false;
    if (done)
        return;

    // try to get GetTickCount64 from the system
    HMODULE kernel32 = ::GetModuleHandleW(L"kernel32");
    if (!kernel32)
        return;

    ptrGetTickCount64 = (PtrGetTickCount64)GetProcAddress(kernel32, "GetTickCount64");

    done = true;
}

//------------------------------------------------------------------------------
/**
*/
static uint64_t getTickCount()
{
    resolveLibs();
    if (ptrGetTickCount64)
        return ptrGetTickCount64();

    static uint32_t highdword = 0;
    static uint32_t lastval = 0;
    uint32_t val = ::GetTickCount();
    if (val < lastval)
        ++highdword;
    lastval = val;
    return val | (uint64_t(highdword) << 32);
}

//------------------------------------------------------------------------------
/**
*/
Dw::ClockType DwElapsedTimer::clockType()
{
    return Dw::TickCounter;
}

//------------------------------------------------------------------------------
/**
*/
bool DwElapsedTimer::isMonotonic()
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void DwElapsedTimer::start()
{
    t1 = getTickCount();
    t2 = 0;
}

//------------------------------------------------------------------------------
/**
*/
int64_t DwElapsedTimer::restart()
{
    int64_t oldt1 = t1;
    t1 = getTickCount();
    t2 = 0;
    return t1 - oldt1;
}

//------------------------------------------------------------------------------
/**
*/
int64_t DwElapsedTimer::elapsed() const
{
    return getTickCount() - t1;
}

//------------------------------------------------------------------------------
/**
*/
int64_t DwElapsedTimer::msecsSinceReference() const
{
    return t1;
}

//------------------------------------------------------------------------------
/**
*/
int64_t DwElapsedTimer::msecsTo(const DwElapsedTimer &other) const
{
    return other.t1 - t1;
}

//------------------------------------------------------------------------------
/**
*/
int64_t DwElapsedTimer::secsTo(const DwElapsedTimer &other) const
{
    return msecsTo(other) / 1000;
}

//------------------------------------------------------------------------------
/**
*/
bool operator<(const DwElapsedTimer &v1, const DwElapsedTimer &v2)
{
    return (v1.t1 - v2.t1) < 0;
}