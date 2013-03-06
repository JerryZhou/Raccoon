#include "stable.h"

#include "dwlocks.h"

//------------------------------------------------------------------------------
/**
*/
DwCriticalSection::DwCriticalSection()
{
    InitializeCriticalSection(&m_criticalSection);
}

//------------------------------------------------------------------------------
/**
*/
DwCriticalSection::~DwCriticalSection()
{
    DeleteCriticalSection(&m_criticalSection);
}

//------------------------------------------------------------------------------
/**
*/
DwCSLocker::DwCSLocker(const DwCriticalSection *cs)
: m_cs((DwCriticalSection *)cs)
{
    DW_ASSERT(cs);
    m_cs->lock();
}

//------------------------------------------------------------------------------
/**
*/
DwCSLocker::~DwCSLocker()
{
    m_cs->unlock();
}

