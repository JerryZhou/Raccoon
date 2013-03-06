#pragma once

#include "dwapp/dwrefcounted.h"
#include "dwapp/dwhttpclient.h"
#include "dwgui/dwurl.h"
#include "dwapp/dwsinglton.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpClientRegistry : public DwRefCounted, public DwSinglton<DwHttpClientRegistry>
{
    DW_DECLARE_CLASS(DwHttpClientRegistry);
public:
    /// constructor
    DwHttpClientRegistry();
    /// destructor
    virtual ~DwHttpClientRegistry();

    /// setup the client registry
    void setup();
    /// discard the client registry, closes all open connections
    void discard();
    /// return true if the object has been setup
    bool isValid() const;

    /// open a connection to a HTTP server, reuse connection if already connected
    DwPtr<DwHttpClient> obtainConnection(const DwUrl& uri);
    /// give up connection to a HTTP server, this will NEVER disconnect!
    void releaseConnection(const DwUrl& uri);
    /// check if a connection to an HTTP server exists
    bool isConnected(const DwUrl& uri) const;
    /// disconnect all connections with a zero use count
    void disconnectIdle();

private:
    struct Connection
    {
        DwPtr<DwHttpClient> httpClient;
        size_t useCount;
    };

    DwMap<DwString, Connection> m_connections;
    bool m_isValid;
};// end of DwHttpClientRegistry

//------------------------------------------------------------------------------
/**
*/
inline bool
DwHttpClientRegistry::isValid() const
{
    return this->m_isValid;
}