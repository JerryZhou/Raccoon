#pragma once

#include "dwapp/dwrefcounted.h"
#include "dwapp/dwhttprequesthandler.h"
#include "dwapp/dwsinglton.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpServerProxy : public DwRefCounted, public DwSinglton<DwHttpServerProxy>
{
    DW_DECLARE_CLASS(DwHttpServerProxy);
public:
    /// constructor
    DwHttpServerProxy();
    /// destructor
    virtual ~DwHttpServerProxy();

    /// open the server proxy
    void open();
    /// close the server proxy
    void close();
    /// return true if open
    bool isOpen() const;

    /// attach a request handler to the server
    void attachRequestHandler(const DwPtr<DwHttpRequestHandler>& h);
    /// remove a request handler from the server
    void removeRequestHandler(const DwPtr<DwHttpRequestHandler>& h);
    /// handle pending http requests, call this method frequently
    void handlePendingRequests();

private:
    DwVector<DwPtr<DwHttpRequestHandler> > m_requestHandlers;
    bool m_isOpen;
};// end of DwHttpServerProxy

//------------------------------------------------------------------------------
/**
*/
inline bool
DwHttpServerProxy::isOpen() const
{
    return this->m_isOpen;
}