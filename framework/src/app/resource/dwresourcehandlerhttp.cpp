#include "stable.h"
#include "dwresourcehandlerhttp.h"
#include "dwapp/dwhttpstream.h"
#include "dwapp/dwresourcemappinghttp.h"
#include "dwapp/dwresourcehttpeventport.h"

DW_IMPLEMENT_CLASS(DwResourceHandlerHttp, 'rhhp', DwInterfaceHandlerBase);

//------------------------------------------------------------------------------
/**
*/
DwResourceHandlerHttp::DwResourceHandlerHttp()
: httpMapping(NULL)
{

}

//------------------------------------------------------------------------------
/**
*/
DwResourceHandlerHttp::~DwResourceHandlerHttp()
{

}

//------------------------------------------------------------------------------
/**
*/
void DwResourceHandlerHttp::open()
{
    DW_ASSERT(!this->isOpen());
    DwInterfaceHandlerBase::open();
}

//------------------------------------------------------------------------------
/**
*/
void DwResourceHandlerHttp::close()
{
    DW_ASSERT(this->isOpen());
    pendingRequests.clear();

    DwInterfaceHandlerBase::close();
}

//------------------------------------------------------------------------------
/**
*/
bool DwResourceHandlerHttp::handleMessage( const DwPtr<DwMessage>& msg )
{
    DW_ASSERT(msg.isvalid());

    if (msg->checkId(DwRequestResource::Id))
    {
        pendingRequests.enqueue(msg.cast<DwRequestResource>());
    }
    else
    {
        // unknown message
        return false;
    }
    // fallthrough: message was handled
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void DwResourceHandlerHttp::doWork()
{
    if (!pendingRequests.isEmpty())
    {
        DwVector<DwPtr<DwMessage> > loadeds;
        DwVector<DwPtr<DwRequestResource> > requests;
        pendingRequests.dequeueAll(requests);
        for (int i=0; i<requests.size(); ++i)
        {
            DwPtr<DwRequestResource> requestRes = requests.at(i);
            DwPtr<DwResourceLoaded> loaded = DwResourceLoaded::create();
            loaded->url = requestRes->url;

            DwPtr<DwHttpStream> httpStream = DwHttpStream::create();
            //DwPtr<DwStream> httpStream = DwHttpStream::create();
            httpStream->setEventPort(DwResourceHttpEventPort::RTTI());
            httpStream->setURI(requestRes->url);
            if ( httpStream->open() )
            {
                if (httpStream->size() > 0)
                {
                    void * data = httpStream->map();
                    loaded->data = DwByteArray((const char*)data, httpStream->size());
                    httpStream->unmap();
                    httpStream->close();
                }
                
                loaded->result = true;
            }
            else
            {
                loaded->result = false;
                DW_LOGR() << "http download: " << requestRes->url.asString() << ("failed!\n");
            }

            loadeds.append(loaded.upcast<DwMessage>());
        }

        if (httpMapping && !loadeds.isEmpty())
        {
            httpMapping->appendResource(loadeds);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwResourceHandlerHttp::setHttpMapping(DwResourceMappingHttp *mapping)
{
    httpMapping = mapping;
    DwResourceHttpEventPort::setHttpMapping(mapping);
}