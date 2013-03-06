#pragma once

#include "dwapp/dwrefcounted.h"
#include "dwapp/dwhttprequesthandler.h"
#include "dwapp/dwhttprequest.h"
#include "dwapp/dwtcpconnection.h"
#include "dwapp/dwdefaulthttprequesthandler.h"
#include "dwapp/dwtcpserver.h"
#include "dwapp/dwptr.h"
#include "dwapp/dwsinglton.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpServer : public DwRefCounted, public DwSinglton<DwHttpServer>
{
    DW_DECLARE_CLASS(DwHttpServer);
public:
    /// constructor
    DwHttpServer();
    /// destructor
    virtual ~DwHttpServer();
    
    /// set port number for http service
    void setPort(uint16_t p);
    /// get port number of http service
    uint16_t port() const;
    /// turn single-thread mode on/off (useful for debugging), default is off
    void setSingleThreadMode(bool b);
    /// get single-thread mode
    bool isSingleThreadMode() const;
    /// open the http server
    bool open();
    /// close the http server
    void close();
    /// return true if server is open
    bool isOpen() const;
    /// attach a request handler to the server
    void attachRequestHandler(const DwPtr<DwHttpRequestHandler>& h);
    /// remove a request handler from the server
    void removeRequestHandler(const DwPtr<DwHttpRequestHandler>& h);
    /// get registered request handlers
    DwVector<DwPtr<DwHttpRequestHandler> > requestHandlers() const;
    /// call this method frequently to serve http connections
    void onFrame();

private:
    /// handle an HttpRequest
    bool handleHttpRequest(const DwPtr<DwTcpClientConnection>& clientConnection);
    /// build an HttpResponse for a handled http request
    bool buildHttpResponse(const DwPtr<DwTcpClientConnection>& clientConnection, const DwPtr<DwHttpRequest>& httpRequest);

    struct PendingRequest
    {
        DwPtr<DwTcpClientConnection> clientConnection;
        DwPtr<DwHttpRequest> httpRequest;
        PendingRequest()
            : clientConnection(NULL)
            , httpRequest(NULL){}
    };

    DwMap<DwString, DwPtr<DwHttpRequestHandler> > m_requestHandlers;
    DwPtr<DwDefaultHttpRequestHandler> m_defaultRequestHandler;    
    DwIpAddress m_ipAddress;
    DwPtr<DwTcpServer> m_tcpServer;
    DwVector<PendingRequest> m_pendingRequests;
    bool m_isOpen;
    bool m_isSingleThreadMode;
};// end of DwHttpServer

//------------------------------------------------------------------------------
/**
*/
inline bool DwHttpServer::isOpen() const
{
    return this->m_isOpen;
}

//------------------------------------------------------------------------------
/**
    Switch on single thread mode, default is off. In single thread mode,
    http requests will be processed immediately in the OnFrame() method,
    not added to the request handler for asynchronous processing. This
    may be useful for debugging, but is dangerous/impossible if HTTP
    request handlers live in different threads!!!
*/
inline void DwHttpServer::setSingleThreadMode(bool b)
{
    DW_ASSERT(!this->m_isOpen);
    this->m_isSingleThreadMode = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwHttpServer::isSingleThreadMode() const
{
    return this->m_isSingleThreadMode;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwHttpServer::setPort(uint16_t p)
{
    this->m_ipAddress.setPort(p);
}

//------------------------------------------------------------------------------
/**
*/
inline uint16_t DwHttpServer::port() const
{
    return this->m_ipAddress.port();
}

//------------------------------------------------------------------------------
/**
*/
inline DwVector<DwPtr<DwHttpRequestHandler> > DwHttpServer::requestHandlers() const
{
    DwVector<DwPtr<DwHttpRequestHandler> > handlers;

    DwMap<DwString, DwPtr<DwHttpRequestHandler> >::const_iterator ite = this->m_requestHandlers.begin();
    while(ite != this->m_requestHandlers.end())
    {
        handlers.append(ite->second);
        ++ite;
    }
    return handlers;
}