
#include "stable.h"

#include "dwhttpclient.h"
#include "dwapp/dwhttprequestwriter.h"
#include "dwapp/dwhttpresponsereader.h"

DW_IMPLEMENT_CLASS(DwHttpClient, 'HTCL', DwRefCounted);

//------------------------------------------------------------------------------
/**
*/
DwHttpClient::DwHttpClient() :
    m_userAgent("Mozilla")    // NOTE: web browser are picky about user agent strings, so use something common
{
    this->m_tcpClient = DwTcpClient::create();
    this->m_tcpClient->setBlocking(true);
}

//------------------------------------------------------------------------------
/**
*/
DwHttpClient::~DwHttpClient()
{
    if (this->isConnected())
    {
        this->disconnect();
    }
    this->m_tcpClient = 0;
}

//------------------------------------------------------------------------------
/**
*/
bool DwHttpClient::connect(const DwUrl& uri)
{
    DW_ASSERT(!this->isConnected());
    DW_ASSERT(this->m_tcpClient->isBlocking());

    // not connected yet, setup connection through HTTP server
    DwIpAddress ipAddress(uri);
    if (ipAddress.port() == 0)
    {
        ipAddress.setPort(80);
    }
    this->m_tcpClient->setServerAddress(ipAddress);
    DwTcpClient::Result result = this->m_tcpClient->connect();
    DW_ASSERT(result != DwTcpClient::Connecting);
    return (DwTcpClient::Success == result);
}

//------------------------------------------------------------------------------
/**
*/
void DwHttpClient::disconnect()
{
    if (this->isConnected())
    {
        this->m_tcpClient->disconnect();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwHttpClient::isConnected() const
{
    return this->m_tcpClient->isConnected();
}

//------------------------------------------------------------------------------
class DwAutoHttpEventPort
{
public:
    DwAutoHttpEventPort(DwPtr<DwHttpClientEventPort>& p) : port(p){}
    ~DwAutoHttpEventPort(){port=NULL;}
    DwPtr<DwHttpClientEventPort>& port;
};

//------------------------------------------------------------------------------
/**
*/
DwHttpStatus::Code DwHttpClient::sendRequest(DwHttpMethod::Code requestMethod, const DwUrl& uri, const DwPtr<DwStream>& responseContentStream)
{
    DW_ASSERT(this->isConnected());
    DwAutoHttpEventPort autoPort(m_eventPort);
    DW_UNUSED(autoPort);

    // write a proper HTTP request to the send stream
    DwPtr<DwHttpRequestWriter> httpRequestWriter = DwHttpRequestWriter::create();
    httpRequestWriter->setMethod(requestMethod);
    httpRequestWriter->setURI(uri);
    httpRequestWriter->setUserAgent(this->m_userAgent);
    httpRequestWriter->setStream(this->m_tcpClient->sendStream());
    if (httpRequestWriter->open())
    {
        httpRequestWriter->writeRequestHeader();
        httpRequestWriter->close();
    }
    else
    {
        report(uri, DwHttpClientEventPort::StateChange, DwVariant((int)DwHttpStatus::ServiceUnavailable));
        return DwHttpStatus::ServiceUnavailable;
    }

    // send off the http request, and wait for the response
    if (!this->m_tcpClient->send())
    {
        // send failed
        report(uri, DwHttpClientEventPort::StateChange, DwVariant((int)DwHttpStatus::ServiceUnavailable));
        return DwHttpStatus::ServiceUnavailable;
    }

    // wait for the response (we're working in blocking mode)
    if (!this->m_tcpClient->recv())
    {
        // receive failed
        report(uri, DwHttpClientEventPort::StateChange, DwVariant((int)DwHttpStatus::ServiceUnavailable));
        return DwHttpStatus::ServiceUnavailable;
    }

    // decode the HTTP response from the receive stream
    DwPtr<DwHttpResponseReader> httpResponseReader = DwHttpResponseReader::create();
    httpResponseReader->setStream(this->m_tcpClient->recvStream());
    if (httpResponseReader->open())
    {
        httpResponseReader->readResponse();
        if (httpResponseReader->isValidHttpResponse())
        {
            if (DwHttpStatus::OK == httpResponseReader->statusCode())
            {
                size_t contentLength = httpResponseReader->contentLength();
                report(uri, DwHttpClientEventPort::HeaderResponse, DwVariant(contentLength));
                if (contentLength > 0)
                {
                    uint8_t* buf = (uint8_t*) DwMemoryAlloc(DW_SCRATCH_HEAP, contentLength);
                    size_t bufPos = 0;
                    size_t bytesToRead = contentLength;
                    bool done = false;
                    while (!done)
                    {
                        const DwPtr<DwStream>& recvStream = this->m_tcpClient->recvStream();
                        size_t bytesRead = recvStream->read(&(buf[bufPos]), bytesToRead);
                        bytesToRead -= bytesRead;
                        bufPos += bytesRead;
                        if (bytesToRead > 0)
                        {
                            recvStream->close();
                            this->m_tcpClient->recv();
                            recvStream->open();
                        }
                        else
                        {
                            done = true;
                        }
                        report(uri, DwHttpClientEventPort::ProgressChange, DwVariant(contentLength-bytesToRead));
                    }
                    if (responseContentStream->isOpen())
                    {
                        // if stream is already open, simply append the read data
                        responseContentStream->write(buf, contentLength);
                    }
                    else
                    {
                        // response content stream not open, so open -> write -> close
                        responseContentStream->setAccessMode(DwStream::WriteAccess);
                        if (responseContentStream->open())
                        {
                            responseContentStream->write(buf, contentLength);
                            responseContentStream->close();
                        }
                        else
                        {
                            DW_ASSERT( false && "HttpClient: Failed to open '%s' for writing!");
                            return DwHttpStatus::ServiceUnavailable;
                        }
                    }
                    DwMemoryFree(DW_SCRATCH_HEAP, buf);
                }
            }
        }
        httpResponseReader->close();
        return httpResponseReader->statusCode();
    }
    // fallthrough: error
    return DwHttpStatus::ServiceUnavailable;
}

//------------------------------------------------------------------------------
/**
*/
void DwHttpClient::setEventPort(const DwPtr<DwHttpClientEventPort>& port)
{
    m_eventPort = port;
}

//------------------------------------------------------------------------------
/**
*/
void DwHttpClient::report(const DwUrl& url, DwHttpClientEventPort::HttpEvent evt, const DwVariant& v)
{
    if (m_eventPort)
    {
        m_eventPort->report(url, evt, v);
    }
}