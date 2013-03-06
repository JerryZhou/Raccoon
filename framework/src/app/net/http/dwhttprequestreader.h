#pragma once

#include "dwgui/dwurl.h"
#include "dwapp/dwstreamreader.h"
#include "dwapp/dwhttpmethod.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpRequestReader : public DwStreamReader
{
    DW_DECLARE_CLASS(DwHttpRequestReader);
public:
    /// constructor
    DwHttpRequestReader();
    /// decode the request from the stream (call first before Get methods!)
    bool readRequest();
    /// return true if the stream contains a valid HTTP request 
    bool isValidHttpRequest() const;
    /// get HTTP request method
    DwHttpMethod::Code httpMethod() const;
    /// get request URI
    const DwUrl& requestURI() const;

private:
    bool m_isValidHttpRequest;
    DwHttpMethod::Code m_httpMethod;
    DwUrl m_requestURI;
};// end of DwHttpRequestReader

//------------------------------------------------------------------------------
/**
*/
inline bool
DwHttpRequestReader::isValidHttpRequest() const
{
    return this->m_isValidHttpRequest;
}

//------------------------------------------------------------------------------
/**
*/
inline DwHttpMethod::Code
DwHttpRequestReader::httpMethod() const
{
    return this->m_httpMethod;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwUrl&
DwHttpRequestReader::requestURI() const
{
    return this->m_requestURI;
}
