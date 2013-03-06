#pragma once

#include "dwgui/dwurl.h"
#include "dwapp/dwrefcounted.h"
#include "dwapp/dwhttprequest.h"
#include "dwapp/dwstream.h"
#include "dwapp/dwsafequeue.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpRequestHandler : public DwRefCounted
{
    DW_DECLARE_CLASS(DwHttpRequestHandler);
public:
    /// constructor
    DwHttpRequestHandler();
    /// destructor
    virtual ~DwHttpRequestHandler();

    /// get a human readable name of the request handler
    const DwString& name() const;
    /// get a human readable description of the request handler
    const DwString& desc() const;
    /// get a resource location path which is accepted by the handler (e.g. "/display")
    const DwString& rootLocation() const;

protected:
    friend class DwHttpServer;
    friend class DwHttpServerProxy;

    /// handle a http request, overwrite this method in you subclass
    virtual void handleRequest(const DwPtr<DwHttpRequest>& request);
    /// handle all pending requests, called by local-thread's HttpServerProxy
    void handlePendingRequests();
    /// put a request to the pending queue, called by HttpServer thread
    void putRequest(const DwPtr<DwHttpRequest>& httpRequest);
    /// set human readable name of the request handler
    void setName(const DwString& n);
    /// set human readable description
    void setDesc(const DwString& d);
    /// set the root location of the request handler
    void setRootLocation(const DwString& l);

    DwString m_name;
    DwString m_desc;
    DwString m_rootLocation;
    DwSafeQueue<DwPtr<DwHttpRequest> > m_pendingRequests;
    DwVector<DwPtr<DwHttpRequest> > m_curWorkRequests;
};// end of DwHttpRequestHandler

//------------------------------------------------------------------------------
/**
*/
inline void
DwHttpRequestHandler::setName(const DwString& n)
{
    this->m_name = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwString&
DwHttpRequestHandler::name() const
{
    return this->m_name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwHttpRequestHandler::setDesc(const DwString& d)
{
    this->m_desc = d;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwString&
DwHttpRequestHandler::desc() const
{
    return this->m_desc;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwHttpRequestHandler::setRootLocation(const DwString& l)
{
    this->m_rootLocation = l;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwString&
DwHttpRequestHandler::rootLocation() const
{
    return this->m_rootLocation;
}

