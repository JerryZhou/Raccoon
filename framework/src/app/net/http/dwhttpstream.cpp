
#include "stable.h"
#include "dwhttpstream.h"
#include "dwapp/dwhttpclientregistry.h"
#include "dwgui/dwringlogcase.h"

DW_IMPLEMENT_CLASS(DwHttpStream, 'HTST', DwMemoryStream);

//------------------------------------------------------------------------------
/**
*/
DwHttpStream::DwHttpStream()
: eventPort(NULL)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwHttpStream::~DwHttpStream()
{
    if (this->isOpen())
    {
        this->close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
DwHttpStream::open()
{
    DW_ASSERT(!this->isOpen());
    bool retval = false;

    if (DwMemoryStream::open())
    {
        DW_LOGR() << "DwHttpStream: Opening " << this->m_uri.asString() << "...";

        // create a HTTP client and open connection
        DwPtr<DwHttpClient> httpClient = DwHttpClientRegistry::Instance()->obtainConnection(this->m_uri);
        if (httpClient->isConnected())
        {
            // port event
            if (eventPort)
            {
                DwPtr<DwHttpClientEventPort> httpEventPort = dwsafe_cast<DwHttpClientEventPort>(eventPort->create());
                httpClient->setEventPort(httpEventPort);
            }

            AccessMode oldAccessMode = this->m_accessMode;
            this->m_accessMode = WriteAccess;
            DwHttpStatus::Code res = httpClient->sendRequest(DwHttpMethod::Get, this->m_uri, this);
            this->m_accessMode = oldAccessMode;
            this->seek(0, DwStream::Begin);
            if (DwHttpStatus::OK == res)
            {
                DW_LOGR() << ("ok!\n");
                retval = true;
            }
            else
            {
                DW_LOGR() << ("failed!\n");
            }
            DwHttpClientRegistry::Instance()->releaseConnection(this->m_uri);
        }
    }
    return retval;
}

void DwHttpStream::setEventPort( const DwRtti* rti )
{
    eventPort = (DwRtti*)rti;
}
