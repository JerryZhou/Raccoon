
#include "stable.h"
#include "dwhandler.h"

DW_IMPLEMENT_CLASS(DwHandler, 'MHDL', DwRefCounted);

//------------------------------------------------------------------------------
/**
*/
DwHandler::DwHandler() :
    m_isOpen(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwHandler::~DwHandler()
{
    DW_ASSERT(!this->isOpen());
}

//------------------------------------------------------------------------------
/**
    Open the handler. This method is called once after the handler has been
    attached to a port and before the first call to HandleMessage().
*/
void
DwHandler::open()
{
    DW_ASSERT(!this->isOpen());
    this->m_isOpen = true;
}

//------------------------------------------------------------------------------
/**
    Close the handler. This method is called once before the handler
    is detached from the port.
*/
void
DwHandler::close()
{
    DW_ASSERT(this->isOpen());
    this->m_isOpen = false;
}

//------------------------------------------------------------------------------
/**
    Derive this method in a subclass to handle specific messages. The method
    should return true only if a message has been handled.
*/
bool
DwHandler::handleMessage(const DwPtr<DwMessage>& msg)
{
    DW_UNUSED(msg);
    DW_ASSERT(msg.isvalid());
    DW_ASSERT(this->m_isOpen);
    return false;
}

//------------------------------------------------------------------------------
/**
    This is an optional method for handlers which need to do continuous
    work (like a render thread message handler). This message will
    be called after messages have been handled.
*/
void
DwHandler::doWork()
{
    // empty, override in subclass
}
