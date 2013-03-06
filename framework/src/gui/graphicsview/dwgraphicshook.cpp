#include "stable.h"
#include "dwgraphicshook.h"

DW_IMPLEMENT_CLASS(DwGraphicsHook, 'GHOK', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsHook::DwGraphicsHook(DwGraphicsHook *next/* = NULL*/)
    : m_next(next)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsHook::~DwGraphicsHook()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsHook::append(DwGraphicsHook *next)
{
    if (next)
    {
        next->m_next = m_next;
    }
    m_next = next;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsHook::setNext(DwGraphicsHook *next)
{
    m_next = next;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsHook* DwGraphicsHook::last() const
{
    DwGraphicsHook * l = (DwGraphicsHook *)(this);
    DwGraphicsHook * n = m_next;
    while(n)
    {
        l = n;
        n = l->next();
    }
    return l;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsHook* DwGraphicsHook::next() const
{
    return m_next;
}