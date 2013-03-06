
#include "stable.h"
#include "dwhttpresponsereader.h"
#include "dwapp/dwtextreader.h"
#include "dwapp/dwmemorystream.h"
#include "dwapp/dwutil.h"

DW_IMPLEMENT_CLASS(DwHttpResponseReader, 'HTRR', DwStreamReader);

//------------------------------------------------------------------------------
/**
*/
DwHttpResponseReader::DwHttpResponseReader() :
    m_isValidHttpResponse(false),
    m_httpStatus(DwHttpStatus::InvalidHttpStatus),
    m_contentLength(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool DwHttpResponseReader::readResponse()
{
    this->m_isValidHttpResponse = false;
    this->m_httpStatus = DwHttpStatus::InvalidHttpStatus;
    this->m_contentLength = 0;
    this->m_contentType.clear();
    bool retval = false;
        
    // attach a text reader to our stream and parse the response header
    DwPtr<DwTextReader> textReader = DwTextReader::create();
    textReader->setStream(this->m_stream);
    if (textReader->open())
    {
        // read first line, should be "HTTP/x.x STATUSCODE STATUSSTRING"
        DwString headLine = textReader->readLine();
        DwVector<DwString> headTokens = DwStringUtil::tokenize(headLine, " ");
        if (headTokens.size() < 3)
        {
            // malformed response header
            return false;
        }

        // check if the HTTP version string looks alright
        if (!DwStringUtil::matchPattern(headTokens[0], "HTTP/*"))
        {
            // malformed request header
            return false;
        }

        // decode the status code
        this->m_httpStatus = DwHttpStatus::fromString(headTokens[1]);
        if (DwHttpStatus::OK == this->m_httpStatus)
        {
            // decode the remaining request header lines
            bool endOfHeader = false;
            while (!textReader->eof() && !endOfHeader)
            {
                DwString curLine = textReader->readLine();
                DwVector<DwString> tokens;
                DwStringUtil::trimRight(curLine, '\r');
                if (!curLine.isEmpty())
                {
                    if (DwStringUtil::matchPattern(curLine, "Content-Length: *"))
                    {
                        // read content length
                        tokens = DwStringUtil::tokenize(curLine, " :");
                        DW_ASSERT(tokens.size() == 2);
                        this->m_contentLength = tokens[1].toInt32();
                    }
                    else if (DwStringUtil::matchPattern(curLine, "Content-Type: *"))
                    {
                        // read content MIME type
                        tokens = DwStringUtil::tokenize(curLine, " :;");
                        DW_ASSERT(tokens.size() >= 2);
                        this->m_contentType.set(tokens[1]);
                    }
                }
                else
                {
                    endOfHeader = true;
                }
            }
            retval = true;
        }
        this->m_isValidHttpResponse = true;
        textReader->close();
    }
    return retval;
}