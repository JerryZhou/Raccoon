#include "stable.h"
#include "dweventport.h"

//------------------------------------------------------------------------------
/**
*/
DwEventPort::DwEventPort()
{

}

//------------------------------------------------------------------------------
/**
*/
DwEventPort::~DwEventPort()
{

}

//------------------------------------------------------------------------------
/**
*/
void DwEventPort::sendEvent(DwEvent *evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwEventPort::attachHandler(DwEventHandler* handler)
{
    DW_UNUSED(handler);
}

//------------------------------------------------------------------------------
/**
*/
void DwEventPort::detachHandler(DwEventHandler* handler)
{
    DW_UNUSED(handler);
}

//------------------------------------------------------------------------------
/**
*/
bool DwEventPort::isAcceptEvent(DwEvent *evt) const
{
    DW_UNUSED(evt);
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwEventPort::registerEvent(DwEventId* id, DwEventHandler* handler)
{
    DW_UNUSED(id);
    DW_UNUSED(handler);
}

//------------------------------------------------------------------------------
/**
*/
void DwEventPort::unRegisterEvent(DwEventId* id, DwEventHandler* handler)
{
    DW_UNUSED(id);
    DW_UNUSED(handler);
}