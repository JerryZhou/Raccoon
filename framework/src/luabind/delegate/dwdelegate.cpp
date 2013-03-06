#include "stable.h"
#include "dwdelegate.h"

DW_IMPLEMENT_CLASS(DwDelegate, 'DETE', DwRttiObject);

DwDelegate::DwDelegate()
    : m_key(NULL)
    , m_parent(NULL)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
bool DwDelegate::addSet(const DwString &s, fn_delegate_set t)
{
    m_sets[s] = t;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwDelegate::addGet(const DwString &s, fn_delegate_get t)
{
    m_gets[s] = t;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwDelegate::addCall(const DwString &s, fn_delegate_get t)
{
    m_calls[s] = t;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
DwDelegate::fn_delegate_set DwDelegate::entrySet(const DwString & s)
{
    return m_sets.value(s, NULL);
}

//------------------------------------------------------------------------------
/**
*/
DwDelegate::fn_delegate_get DwDelegate::entryGet(const DwString & s)
{
    return m_gets.value(s, NULL);
}

//------------------------------------------------------------------------------
/**
*/
DwDelegate::fn_delegate_get DwDelegate::entryCall(const DwString & s)
{
    return m_calls.value(s, NULL);
}

//------------------------------------------------------------------------------
/**
*/
DwRtti *DwDelegate::key() const
{
    return m_key;
}

//------------------------------------------------------------------------------
/**
*/
DwDelegate *DwDelegate::parent() const
{
    return m_parent;
}

//------------------------------------------------------------------------------
/**
*/
void DwDelegate::setKey(DwRtti *k)
{
    m_key = k;
}

//------------------------------------------------------------------------------
/**
*/
void DwDelegate::setParent(DwDelegate *p)
{
    m_parent = p;
}