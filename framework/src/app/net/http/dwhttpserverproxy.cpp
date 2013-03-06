
#include "stable.h"
#include "dwhttpserverproxy.h"
#include "dwapp/dwhttpprotocol.h"
#include "dwapp/dwhttpinterface.h"
#include "dwapp/dwutil.h"

DW_IMPLEMENT_CLASS(DwHttpServerProxy, 'HTSP', DwRefCounted);

//------------------------------------------------------------------------------
/**
*/
DwHttpServerProxy::DwHttpServerProxy() :
    m_isOpen(false)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwHttpServerProxy::~DwHttpServerProxy()
{
    DW_ASSERT(!this->m_isOpen);
}

//------------------------------------------------------------------------------
/**
*/
void
DwHttpServerProxy::open()
{
    DW_ASSERT(!this->m_isOpen);
    DW_ASSERT(this->m_requestHandlers.isEmpty());
    this->m_isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
DwHttpServerProxy::close()
{
    DW_ASSERT(this->m_isOpen);
    
    // cleanup request handlers
    while (this->m_requestHandlers.size() > 0)
    {
        this->removeRequestHandler(this->m_requestHandlers.back());
    }
    this->m_isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
DwHttpServerProxy::attachRequestHandler(const DwPtr<DwHttpRequestHandler>& requestHandler)
{
    DW_ASSERT(this->m_isOpen);
    this->m_requestHandlers.append(requestHandler);

    // register request handler with HttpServer thread 
    DwPtr<DwAttachRequestHandler> msg = DwAttachRequestHandler::create();
    msg->SetRequestHandler(requestHandler);
    DwHttpInterface::Instance()->send(msg.cast<DwMessage>());
}

//------------------------------------------------------------------------------
/**
*/
void
DwHttpServerProxy::removeRequestHandler(const DwPtr<DwHttpRequestHandler>& requestHandler)
{
    DW_ASSERT(this->m_isOpen);    
    int index = this->m_requestHandlers.indexOf(requestHandler);
    DW_ASSERT(InvalidIndex != index);
    
    // unregister request handler from HttpServer thread
    DwPtr<DwRemoveRequestHandler> msg = DwRemoveRequestHandler::create();
    msg->SetRequestHandler(requestHandler);
    DwHttpInterface::Instance()->send(msg.cast<DwMessage>());

    // delete from local array
    this->m_requestHandlers.remove(index);
}

//------------------------------------------------------------------------------
/**
*/
void
DwHttpServerProxy::handlePendingRequests()
{
    DW_ASSERT(this->m_isOpen);

    int i;
    for (i = 0; i < this->m_requestHandlers.size(); i++)
    {
        this->m_requestHandlers[i]->handlePendingRequests();
    }
}