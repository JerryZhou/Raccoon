
#include "stable.h"
#include "dwhttpserver.h"
#include "dwapp/dwutil.h"
#include "dwapp/dwhttprequestreader.h"
#include "dwapp/dwhttprequestwriter.h"
#include "dwapp/dwhttpresponsewriter.h"
#include "dwapp/dwhttpresponsereader.h"
#include "dwapp/dwmemorystream.h"
#include "dwapp/dwtextwriter.h"
#include "dwapp/dwtextreader.h"

DW_IMPLEMENT_CLASS(DwHttpServer, 'HTPS', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwHttpServer::DwHttpServer() :
    m_isOpen(false),
    m_isSingleThreadMode(false)
{
    this->m_ipAddress.setHostName("any");
    this->m_ipAddress.setPort(2100);
}

//------------------------------------------------------------------------------
/**
*/
DwHttpServer::~DwHttpServer()
{
    DW_ASSERT(!this->isOpen());
    this->m_tcpServer = 0;
    this->m_defaultRequestHandler = 0;
}

//------------------------------------------------------------------------------
/**
*/
bool DwHttpServer::open()
{
    DW_ASSERT(!this->m_isOpen);
    this->m_isOpen = true;
    
    // setup a new TcpServer object
    this->m_tcpServer = DwTcpServer::create();
    this->m_tcpServer->setAddress(this->m_ipAddress);
    bool success = this->m_tcpServer->open();

    // create the default http request handler
    this->m_defaultRequestHandler = DwDefaultHttpRequestHandler::create();
    return success;
}

//------------------------------------------------------------------------------
/**
*/
void DwHttpServer::close()
{
    DW_ASSERT(this->m_isOpen);

    // clear pending requests
    this->m_pendingRequests.clear();

    // destroy the default http request handler
    this->m_defaultRequestHandler = 0;

    // remove request handlers
    this->m_requestHandlers.clear();

    // shutdown TcpServer
    this->m_tcpServer->close();
    this->m_tcpServer = 0;
    
    this->m_isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void DwHttpServer::attachRequestHandler(const DwPtr<DwHttpRequestHandler>& requestHandler)
{
    DW_ASSERT(requestHandler);
    DW_ASSERT(this->m_isOpen);
    DW_ASSERT(!this->m_requestHandlers.contains(requestHandler->rootLocation()));
    this->m_requestHandlers.insert(requestHandler->rootLocation(), requestHandler);
}

//------------------------------------------------------------------------------
/**
*/
void DwHttpServer::removeRequestHandler(const DwPtr<DwHttpRequestHandler>& requestHandler)
{
    DW_ASSERT(requestHandler);
    DW_ASSERT(this->m_isOpen);
    this->m_requestHandlers.remove(requestHandler->rootLocation());
}

//------------------------------------------------------------------------------
/**
*/
void DwHttpServer::onFrame()
{
    DW_ASSERT(this->m_isOpen);

    // handle pending client requests
    DwVector<DwPtr<DwTcpClientConnection>> recvConns = this->m_tcpServer->recv();
    int i;
    for (i = 0; i < recvConns.size(); i++)
    {
        if (!this->handleHttpRequest(recvConns[i]))
        {
            recvConns[i]->shutdown();
        }
    }

    // handle processed http requests
    for (i = 0; i < this->m_pendingRequests.size();)
    {
        const DwPtr<DwHttpRequest>& httpRequest = this->m_pendingRequests[i].httpRequest;
        if (httpRequest->handled())
        {
            const DwPtr<DwTcpClientConnection>& conn = this->m_pendingRequests[i].clientConnection;
            if (this->buildHttpResponse(conn, httpRequest))
            {
                conn->send();
            }
            this->m_pendingRequests.remove(i);
        }
        else
        {
            i++;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwHttpServer::handleHttpRequest(const DwPtr<DwTcpClientConnection>& clientConnection)  
{
    // decode the request
    DwPtr<DwHttpRequestReader> httpRequestReader = DwHttpRequestReader::create();
    httpRequestReader->setStream(clientConnection->recvStream());
    if (httpRequestReader->open())
    {
        httpRequestReader->readRequest();
        httpRequestReader->close();
    }
    if (httpRequestReader->isValidHttpRequest())
    {
        DwUrl requestURI = httpRequestReader->requestURI();
    
        // create a content stream for the response
        DwPtr<DwStream> responseContentStream = DwMemoryStream::create();
        
        // build a HttpRequest object
        DwPtr<DwHttpRequest> httpRequest = DwHttpRequest::create();
        httpRequest->setMethod(httpRequestReader->httpMethod());
        httpRequest->setURI(httpRequestReader->requestURI());
        httpRequest->setResponseContentStream(responseContentStream);
        httpRequest->setStatus(DwHttpStatus::NotFound);

        // find a request handler which accepts the request
        DwPtr<DwHttpRequestHandler> requestHandler = NULL;
        DwVector<DwString> tokens = DwStringUtil::tokenize(requestURI.localPath(), ("/"));
        if (tokens.size() > 0)
        {
            if (this->m_requestHandlers.contains(tokens[0]))
            {
                requestHandler = this->m_requestHandlers[tokens[0]];
            }
        }
        if (requestHandler)
        {
            // handle the request, default is asynchronous handling 
            // (request is added to request handler with PutRequest()
            // and processed when the thread where the request handler
            // lives calls HandlePendingRequests()
            // in SingleThread mode, the request will be processed immediately,
            // but this is a death-receipt if request handlers live on
            // different threads!!!
            if (this->isSingleThreadMode())
            {
                // handle request immediately
                requestHandler->handleRequest(httpRequest);
                httpRequest->setHandled(true);
            }
            else
            {
                // asynchronously handle the request
                requestHandler->putRequest(httpRequest);
            }
        }
        else
        {
            // no request handler accepts the request, let the default
            // request handler handle the request
            this->m_defaultRequestHandler->handleRequest(httpRequest);
            httpRequest->setHandled(true);
        }

        // append request to pending queue
        PendingRequest pendingRequest;
        pendingRequest.clientConnection = clientConnection;
        pendingRequest.httpRequest = httpRequest;
        this->m_pendingRequests.append(pendingRequest);

        return true;
    }
    else
    {
        // the received data was not a valid HTTP request
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwHttpServer::buildHttpResponse(const DwPtr<DwTcpClientConnection>& conn, const DwPtr<DwHttpRequest>& httpRequest)
{
    DwPtr<DwHttpResponseWriter> responseWriter = DwHttpResponseWriter::create();
    responseWriter->setStream(conn->sendStream());
    responseWriter->setStatusCode(httpRequest->status());
    if (DwHttpStatus::OK != httpRequest->status())
    {
        // an error occured, need to write an error message to the response stream
        DwPtr<DwTextWriter> textWriter = DwTextWriter::create();
        textWriter->setStream(httpRequest->responseContentStream());
        textWriter->open();
        textWriter->writeFormatted("%s %s", DwHttpStatus::toString(httpRequest->status()).toUtf8().data(), DwHttpStatus::toHumanReadableString(httpRequest->status()).toCap().data());
        textWriter->close();
        httpRequest->responseContentStream()->setMediaType(DwMediaType("text/plain"));
    }
    if (httpRequest->responseContentStream()->size() > 0)
    {
        httpRequest->responseContentStream()->mediaType().isValid();
        responseWriter->setContent(httpRequest->responseContentStream());
    }
    // do nothing, if stream isn't valid
    if (responseWriter->stream().isvalid())
    {
        responseWriter->open();
        responseWriter->writeResponse();
        responseWriter->close();                 
        return true;
    }    
    return false;
}

