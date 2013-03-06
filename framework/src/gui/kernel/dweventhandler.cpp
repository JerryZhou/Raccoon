#include "stable.h"
#include "dweventhandler.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwEventHandler, 'EHER', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwEventHandler::DwEventHandler()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwEventHandler::~DwEventHandler()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwEventHandler::onEvent(DwEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
	register interest event id in port
*/
void DwEventHandler::onAttachd(DwEventPort* port)
{
    DW_UNUSED(port);
}

//------------------------------------------------------------------------------
/**
	unregister interest event id in port
*/
void DwEventHandler::onDetach(DwEventPort* port)
{
    DW_UNUSED(port);
}