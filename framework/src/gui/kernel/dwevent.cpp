#include "stable.h"
#include "dwevent.h"

DW_IMPLEMENT_ROOT_EVENTID(DwEvent);

//------------------------------------------------------------------------------
/**
*/
DwEvent::DwEvent()
    : m_accept(0)
    , m_reserved(0)
    , m_dir(0)
    , m_backtrace(0)
    , m_traced(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwEvent::~DwEvent()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
bool DwEvent::isA(const DwEventId *id) const
{
    return eventId()->isDerivedFrom(id);
}