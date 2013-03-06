
#include "stable.h"
#include "dwhttprequestreader.h"
#include "dwapp/dwtextreader.h"
#include "dwapp/dwutil.h"

DW_IMPLEMENT_CLASS(DwHttpRequestReader, 'HRQR', DwStreamReader);

//------------------------------------------------------------------------------
/**
*/
DwHttpRequestReader::DwHttpRequestReader() :
    m_isValidHttpRequest(false),
    m_httpMethod(DwHttpMethod::InvalidHttpMethod)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
DwHttpRequestReader::readRequest()
{
    this->m_isValidHttpRequest = false;
        
    // attach a text reader to our stream and parse the request header
    DwPtr<DwTextReader> textReader = DwTextReader::create();
    textReader->setStream(this->m_stream);
    if (textReader->open())
    {
        // read the first line of the request
        // should be "METHOD PATH HTTP/1.1[CRLF]
        DwString headLine =  textReader->readLine();
        DwVector<DwString> headTokens = DwStringUtil::tokenize(headLine, " ");
        if (headTokens.size() != 3)
        {
            // malformed request header
            textReader->close();
            return false;
        }

        // check if the HTTP version string looks alright
        if (!DwStringUtil::matchPattern(headTokens[2], "HTTP/*"))
        {
            // malformed request header
            textReader->close();
            return false;
        }

        // decode the HTTP method
        this->m_httpMethod = DwHttpMethod::fromString(headTokens[0]);

        // decode the remaining request header lines
        DwString host;
        bool endOfHeader = false;
        while (!textReader->eof() && !endOfHeader)
        {
            DwString curLine = textReader->readLine();
            if (!curLine.isNull())
            {
                if (DwStringUtil::matchPattern(curLine, "Host: *"))
                {
                    host = curLine;
                    host.replace("Host: ", "");
                }
            }
            else
            {
                endOfHeader = true;
            }
        }

        // build URI
        DwString uriString;
        DwStringUtil::stringFormat(uriString, L"http://%s%s", host.constString(), headTokens[1].constString());
        this->m_requestURI = uriString;

        this->m_isValidHttpRequest = true;
        textReader->close();
        return true;
    }
    return false;
}