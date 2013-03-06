
#include "stable.h"
#include "dwapp/dwhttpmessagehandler.h"
#include "dwapp/dwthread.h"

DW_IMPLEMENT_CLASS(DwHttpMessageHandler, 'HTMH', DwInterfaceHandlerBase);

//------------------------------------------------------------------------------
/**
*/
DwHttpMessageHandler::DwHttpMessageHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwHttpMessageHandler::~DwHttpMessageHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
DwHttpMessageHandler::open()
{   
    DW_ASSERT(!this->isOpen());
    DwInterfaceHandlerBase::open();

    // setup core runtime and central http server
    this->m_httpServer = DwHttpServer::create();
    this->m_httpServer->setPort(DefaultTcpPort);
    this->m_httpServer->open();
}

//------------------------------------------------------------------------------
/**
*/
void
DwHttpMessageHandler::close()
{
    DW_ASSERT(this->isOpen());

    this->m_httpServer->close();
    this->m_httpServer = 0;

    DwInterfaceHandlerBase::close();
}

//------------------------------------------------------------------------------
/**
    Triggers the http server from time to time.
*/
void
DwHttpMessageHandler::doWork()
{
    DW_ASSERT(this->isOpen());

    // only process http requests once in a while
    this->m_httpServer->onFrame();
    ::Sleep(100);   
}

//------------------------------------------------------------------------------
/**
*/
bool
DwHttpMessageHandler::handleMessage(const DwPtr<DwMessage>& msg)
{
    DW_ASSERT(msg.isvalid());

    if (msg->checkId(DwAttachRequestHandler::Id))
    {
        this->onAttachRequestHandler(msg.cast<DwAttachRequestHandler>());
    }
    else if (msg->checkId(DwRemoveRequestHandler::Id))
    {
        this->onRemoveRequestHandler(msg.cast<DwRemoveRequestHandler>());
    }
    else
    {
        // unknown message
        return false;
    }
    // fallthrough: message was handled
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
DwHttpMessageHandler::onAttachRequestHandler(const DwPtr<DwAttachRequestHandler>& msg)
{
    this->m_httpServer->attachRequestHandler(msg->GetRequestHandler());
}

//------------------------------------------------------------------------------
/**
*/
void
DwHttpMessageHandler::onRemoveRequestHandler(const DwPtr<DwRemoveRequestHandler>& msg)
{
    this->m_httpServer->removeRequestHandler(msg->GetRequestHandler());
}
