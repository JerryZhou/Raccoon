
#include "stable.h"
#include "dwhttprequesthandler.h"

DW_IMPLEMENT_CLASS(DwHttpRequestHandler, 'HRHD', DwRefCounted);

//------------------------------------------------------------------------------
/**
*/
DwHttpRequestHandler::DwHttpRequestHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwHttpRequestHandler::~DwHttpRequestHandler()
{
    this->m_pendingRequests.setSignalOnEnqueueEnabled(false);
    // empty
}

//------------------------------------------------------------------------------
/**
    Put a http request into the request handlers message queue. This method
    is meant to be called from another thread.
*/
void
DwHttpRequestHandler::putRequest(const DwPtr<DwHttpRequest>& httpRequest)
{
    this->m_pendingRequests.enqueue(httpRequest);
}

//------------------------------------------------------------------------------
/**
    Handle all pending http requests in the pending queue. This method
    must be called frequently from the thread which created this
    request handler.
*/
void
DwHttpRequestHandler::handlePendingRequests()
{
    this->m_pendingRequests.dequeueAll(this->m_curWorkRequests);
    int i;
    for (i = 0; i < this->m_curWorkRequests.size(); i++)
    {
        this->handleRequest(this->m_curWorkRequests[i]);
        this->m_curWorkRequests[i]->setHandled(true);
    }
}

//------------------------------------------------------------------------------
/**
    Overwrite this method in your subclass. This method will be called by the
    HttpServer if AcceptsRequest() returned true. The request handler should
    properly process the request by filling the responseContentStream with
    data (for instance a HTML page), set the MediaType on the 
    responseContentStream (for instance "text/html") and return with a
    HttpStatus code (usually HttpStatus::OK).
*/
void
DwHttpRequestHandler::handleRequest(const DwPtr<DwHttpRequest>& request)
{
    request->setStatus(DwHttpStatus::NotFound);
}
