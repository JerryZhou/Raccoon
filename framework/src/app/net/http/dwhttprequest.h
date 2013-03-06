#pragma once

#include "dwapp/dwmessage.h"
#include "dwapp/dwhttpmethod.h"
#include "dwapp/dwhttpstatus.h"
#include "dwapp/dwstream.h"
#include "dwapp/dwptr.h"
#include "dwgui/dwurl.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpRequest : public DwMessage
{
    DW_DECLARE_CLASS(DwHttpRequest);
    DW_DECLARE_MESSAGEID;
public:
    /// constructor
    DwHttpRequest();
    /// destructor
    virtual ~DwHttpRequest();
    
    /// set the Http method (GET, PUT, etc...)
    void setMethod(DwHttpMethod::Code requestMethod);
    /// get the Http method
    DwHttpMethod::Code method() const;
    /// set the request URI
    void setURI(const DwUrl& requestUri);
    /// get the request URI
    const DwUrl& URI() const;
    /// set the response content stream
    void setResponseContentStream(const DwPtr<DwStream>& s);
    /// get the response content stream
    const DwPtr<DwStream>& responseContentStream() const;
    /// set the http status (set by HttpRequestHandler)
    void setStatus(DwHttpStatus::Code s);
    /// get the http status 
    DwHttpStatus::Code status() const;

private:
    DwHttpMethod::Code m_method;
    DwUrl m_uri;
    DwPtr<DwStream> m_responseContentStream;
    DwHttpStatus::Code m_status;
};// end of DwHttpRequest

//------------------------------------------------------------------------------
/**
*/
inline void DwHttpRequest::setMethod(DwHttpMethod::Code m)
{
    this->m_method = m;
}

//------------------------------------------------------------------------------
/**
*/
inline DwHttpMethod::Code DwHttpRequest::method() const
{
    return this->m_method;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwHttpRequest::setURI(const DwUrl& u)
{
    this->m_uri = u;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwUrl& DwHttpRequest::URI() const
{
    return this->m_uri;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwHttpRequest::setResponseContentStream(const DwPtr<DwStream>& s)
{
    this->m_responseContentStream = s;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwPtr<DwStream>& DwHttpRequest::responseContentStream() const
{
    return this->m_responseContentStream;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwHttpRequest::setStatus(DwHttpStatus::Code s)
{
    this->m_status = s;
}

//------------------------------------------------------------------------------
/**
*/
inline DwHttpStatus::Code DwHttpRequest::status() const
{
    return this->m_status;
}