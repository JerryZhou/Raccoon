#pragma once

#include "dwgui/dwurl.h"
#include "dwapp/dwrefcounted.h"
#include "dwapp/dwtcpclient.h"
#include "dwapp/dwhttpstatus.h"
#include "dwapp/dwhttpmethod.h"
#include "dwapp/dwstream.h"
#include "dwapp/dwhttpclienteventport.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpClient : public DwRefCounted
{
    DW_DECLARE_CLASS(DwHttpClient);
public:
    /// constructor
    DwHttpClient();
    /// destructor
    virtual ~DwHttpClient();
    
    /// set the user-agent to use for HTTP requests
    void setUserAgent(const DwString& u);
    /// get the user-agent
    const DwString& userAgent() const;

    /// establish a connection to a HTTP server
    bool connect(const DwUrl& uri);
    /// disconnect from the server
    void disconnect();
    /// return true if currently connected
    bool isConnected() const;

    /// send request and write result to provided response content stream
    DwHttpStatus::Code sendRequest(DwHttpMethod::Code requestMethod, const DwUrl& uri, const DwPtr<DwStream>& responseContentStream);

    /// set current event port
    void setEventPort(const DwPtr<DwHttpClientEventPort>& port);
    /// report event to outside
    void report(const DwUrl& url, DwHttpClientEventPort::HttpEvent evt, const DwVariant& v);
private:
    DwString m_userAgent;
    DwPtr<DwTcpClient> m_tcpClient;
    DwPtr<DwHttpClientEventPort> m_eventPort;
};// end of DwHttpClient

//------------------------------------------------------------------------------
/**
*/
inline void DwHttpClient::setUserAgent(const DwString& agent)
{
    this->m_userAgent = agent;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwString& DwHttpClient::userAgent() const
{
    return this->m_userAgent;
}