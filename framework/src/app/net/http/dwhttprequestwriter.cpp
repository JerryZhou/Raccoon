
#include "stable.h"
#include "dwhttprequestwriter.h"
#include "dwapp/dwtextwriter.h"

DW_IMPLEMENT_CLASS(DwHttpRequestWriter, 'HTRW', DwStreamWriter);

//------------------------------------------------------------------------------
/**
*/
DwHttpRequestWriter::DwHttpRequestWriter() :
    m_httpMethod(DwHttpMethod::Get),
    m_userAgent("Mozilla")    // NOTE: web browser are picky about user agent strings, so use something common
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
DwHttpRequestWriter::writeRequestHeader()
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(this->m_uri.isValid());

    // attach a text writer to our stream
    DwPtr<DwTextWriter> textWriter = DwTextWriter::create();
    textWriter->setStream(this->m_stream);
    if (textWriter->open())
    {
        // write header
        textWriter->writeFormatted("%s /%s HTTP/1.1\r\n", 
            DwHttpMethod::toString(this->m_httpMethod).toUtf8().data(),
            this->m_uri.localPath().toUtf8().data());
        textWriter->writeFormatted("Host: %s\r\n", this->m_uri.host().toUtf8().data());
        textWriter->writeFormatted("User-Agent: %s\r\n", this->m_userAgent.toUtf8().data());
        textWriter->writeString("Keep-Alive: 300\r\n");
        textWriter->writeString("Connection: keep-alive\r\n");
        textWriter->writeString("\r\n");
        textWriter->close();
        return true;
    }
    return false;
}