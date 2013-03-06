#pragma once

#include "dwapp/dwstreamreader.h"
#include "dwapp/dwhttpstatus.h"
#include "dwgui/dwmediatype.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpResponseReader : public DwStreamReader
{
    DW_DECLARE_CLASS(DwHttpResponseReader);
public:
    /// constructor
    DwHttpResponseReader();

    /// read the response
    bool readResponse();
    /// return true if this was a valid response
    bool isValidHttpResponse() const;
    /// get the HTTP status code which was sent by the server
    DwHttpStatus::Code statusCode() const;
    /// get content type
    const DwMediaType& contentType() const;
    /// get content length
    size_t contentLength() const;

private:
    bool m_isValidHttpResponse;
    DwHttpStatus::Code m_httpStatus;
    DwMediaType m_contentType;
    size_t m_contentLength;
};// end of DwHttpResponseReader

//------------------------------------------------------------------------------
/**
*/
inline bool DwHttpResponseReader::isValidHttpResponse() const
{
    return this->m_isValidHttpResponse;
}

//------------------------------------------------------------------------------
/**
*/
inline DwHttpStatus::Code DwHttpResponseReader::statusCode() const
{
    return this->m_httpStatus;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwMediaType& DwHttpResponseReader::contentType() const
{
    return this->m_contentType;
}

//------------------------------------------------------------------------------
/**
*/
inline size_t DwHttpResponseReader::contentLength() const
{
    return this->m_contentLength;
}