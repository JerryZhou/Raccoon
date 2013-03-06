#pragma once

#include "dwapp/dwstreamwriter.h"
#include "dwapp/dwhttpmethod.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpRequestWriter : public DwStreamWriter
{
    DW_DECLARE_CLASS(DwHttpRequestWriter);
public:
    /// constructor
    DwHttpRequestWriter();
    
    /// set HTTP Method
    void setMethod(DwHttpMethod::Code httpMethod);
    /// set the URI of the request
    void setURI(const DwUrl& uri);
    /// set optional User-Agent string
    void setUserAgent(const DwString& userAgent);
    /// write the request header to the stream
    bool writeRequestHeader();

private:
    DwHttpMethod::Code m_httpMethod;
    DwUrl m_uri;
    DwString m_userAgent;
};// end of DwHttpRequestWriter

//------------------------------------------------------------------------------
/**
*/
inline void
DwHttpRequestWriter::setMethod(DwHttpMethod::Code m)
{
    this->m_httpMethod = m;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwHttpRequestWriter::setURI(const DwUrl& u)
{
    this->m_uri = u;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwHttpRequestWriter::setUserAgent(const DwString& agent)
{
    this->m_userAgent = agent;
}
