#pragma once

#include "dwapp/dwstreamwriter.h"
#include "dwapp/dwhttpstatus.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpResponseWriter : public DwStreamWriter
{
    DW_DECLARE_CLASS(DwHttpResponseWriter);
public:
    /// set status code
    void setStatusCode(DwHttpStatus::Code c);
    /// set optional content stream (needs valid media type!)
    void setContent(const DwPtr<DwStream>& s);
    /// write http response to the stream
    void writeResponse();

private:
    DwHttpStatus::Code m_statusCode;
    DwPtr<DwStream> m_contentStream;
};// end of DwHttpResponseWriter

//------------------------------------------------------------------------------
/**
*/
inline void
DwHttpResponseWriter::setStatusCode(DwHttpStatus::Code c)
{
    this->m_statusCode = c;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwHttpResponseWriter::setContent(const DwPtr<DwStream>& s)
{
    this->m_contentStream = s;
}
