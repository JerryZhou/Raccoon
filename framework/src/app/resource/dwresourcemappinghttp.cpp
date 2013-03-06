#include "stable.h"
#include "dwresourcemappinghttp.h"
#include "dwgui/dwresourcelib.h"
#include "dwgui/dwresourceevent.h"
#include "dwapp/dwhttpinterface.h"

DW_IMPLEMENT_CLASS(DwResourceMappingHttp, 'RTTP', DwResourceMapping);

//------------------------------------------------------------------------------
/**
*/
DwByteArray DwResourceMappingHttp::lookupRes(const DwUrl &url)const
{
    if(hasRes(url))
    {
        return DwResourceMapping::lookupRes(url);
    }

    const_cast<DwResourceMappingHttp*>(this)->acquireLink(url);
    return DwByteArray();
}

//------------------------------------------------------------------------------
/**
*/
bool DwResourceMappingHttp::isAsyncResource(const DwUrl &url) const
{
    if( hasRes(url) )
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void DwResourceMappingHttp::update()
{
    handlePendingResources();
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
bool DwResourceMappingHttp::acquireLink(const DwUrl &url)
{
    DW_UNUSED(url);
    DwString key = url.asString();
    if ( !m_pendingRequireLinks.contains(key) )
    {
        m_pendingRequireLinks[key] = true;

        // send request to http thread
        DwPtr<DwRequestResource> msg = DwRequestResource::create();
        msg->setURL(url);
        DwHttpInterface::Instance()->send(msg.cast<DwMessage>());
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwResourceMappingHttp::appendResource(const DwPtr<DwMessage> &res)
{
    m_pendingResources.enqueue(res);
}

//------------------------------------------------------------------------------
/**
*/
void DwResourceMappingHttp::appendResource(const DwVector<DwPtr<DwMessage> > &ress)
{
    m_pendingResources.enqueueArray(ress);
}

// run in ui thread
void DwResourceMappingHttp::handlePendingResources()
{
    if (!m_pendingResources.isEmpty())
    {
        DwVector<DwPtr<DwMessage> > resources;
        m_pendingResources.dequeueAll(resources);
        if (!resources.isEmpty())
        {
            for (int i=0; i<resources.count(); ++i)
            {
                const DwPtr<DwMessage>& res = resources.at(i);

                if (res->isA(DwResourceLoaded::RTTI()))
                {
                    const DwPtr<DwResourceLoaded>& loaded = res.downcast<DwResourceLoaded>();

                    // remove request link key
                    DwString key = loaded->url.asString();
                    m_pendingRequireLinks.remove(key);

                    // send event
                    DwResourceLoadFinishedEvent evt;
                    evt.url = loaded->url;
                    evt.data = loaded->data;
                    dwResourceLib()->sendEvent(&evt);
                }
                else if (res->isA(DwResourceFailed::RTTI()))
                {
                    const DwPtr<DwResourceFailed>& loaded = res.downcast<DwResourceFailed>();

                    // remove request link key
                    DwString key = loaded->url.asString();
                    m_pendingRequireLinks.remove(key);

                    // send event
                    DwResourceLoadFinishedEvent evt;
                    evt.url = loaded->url;
                    dwResourceLib()->sendEvent(&evt);
                }
                else if (res->isA(DwResourceProgress::RTTI()))
                {
                    const DwPtr<DwResourceProgress>& loaded = res.downcast<DwResourceProgress>();

                    // send event
                    DwResourceLoadProgressEvent evt;
                    evt.url = loaded->url;
                    evt.size = loaded->contentLength;
                    evt.progress = loaded->progress;
                    dwResourceLib()->sendEvent(&evt);
                }
            }
        }
    }
}