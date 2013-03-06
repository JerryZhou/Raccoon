
#include "stable.h"
#include "dwhttpresponsewriter.h"
#include "dwapp/dwtextwriter.h"


DW_IMPLEMENT_CLASS(DwHttpResponseWriter, 'HRSW', DwStreamWriter);

//------------------------------------------------------------------------------
/**
*/
void
DwHttpResponseWriter::writeResponse()
{
    DwPtr<DwTextWriter> textWriter = DwTextWriter::create();
    textWriter->setStream(this->m_stream);
    if (textWriter->open())
    {
        textWriter->writeFormatted("HTTP/1.1 %s %s\r\n", 
            DwHttpStatus::toString(this->m_statusCode).toCap().data(),
            DwHttpStatus::toHumanReadableString(this->m_statusCode).toCap().data());
        if (this->m_contentStream.isvalid())
        {
            textWriter->writeFormatted("Content-Length: %d\r\n", 
                this->m_contentStream->size());
            if (!this->m_contentStream->mediaType().asString().isNull())
            {
                textWriter->writeFormatted("Content-Type: %s;  charset=utf-8\r\n", 
                    this->m_contentStream->mediaType().asString().toCap().data());
            }
        }
        else
        {
            textWriter->writeString("Content-Length: 0\r\n");
        }
        textWriter->writeString("\r\n");
        textWriter->close();
    }

    // append content string
    if (this->m_contentStream.isvalid())
    {
        DW_ASSERT(this->m_contentStream->canBeMapped());
        this->m_contentStream->setAccessMode(DwStream::ReadAccess);
        if (this->m_contentStream->open())
        {                 
            void* ptr = this->m_contentStream->map();
            this->m_stream->write(ptr, this->m_contentStream->size());
            this->m_contentStream->unmap();                                                          
            this->m_contentStream->close();
        }             
    }
}