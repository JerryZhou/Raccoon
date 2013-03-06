#include "stable.h"
#include "dwresourceinterface.h"
#include "dwresourceevent.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwrttiobjectptr.h"
#include "dwgui/dwgraphicsitem.h"
#include "dwgui/dweventsignaldispatch.h"
#include "dwgui/dwgraphicspixmapitem.h"
#include "dwgui/dwgraphicsninepatchitem.h"

DW_DECLARE_DLLRESOURCE(gui, theme);

//------------------------------------------------------------------------------
/**
    DEAL WITH THE ASYNC RESOURCE EVENT
    support async acquire image
*/
struct DwAsyncImageContainer : public sigslot::has_slots
{
    typedef DwHash<DwString, DwImage> Container;
    typedef DwMap<DwRttiObjectPtr<DwRttiObject>, bool > RttiObjectContainer;
    typedef DwHash<DwString, RttiObjectContainer > ListenerContainer;
    Container images;
    ListenerContainer listeners;

    DwImage acquire(const DwUrl &url, DwRttiObject *obj )
    {
        // try in cached container
        DwString key = url.asString();
        Container::const_iterator ite = images.find(key);
        if (ite != images.end())
        {
            if (listeners.contains(key))
            {
                addListener(key, obj);
            }
            return (*ite).second;
        }

        // try in image cached
        if( DwImage::hasCachedOf(key) )
        {
            return DwImage(key, DwByteArray());
        }

        // create the takeship of resource
        DwByteArray data = dwResourceLib()->lookupRes(url);
        DwImage source(key, data);
        images[key] = source;

        // listener
        addListener(key, obj);
        return source;
    }

    void addListener(DwString key, DwRttiObject* obj)
    {
        if (obj)
        {
            ListenerContainer::iterator ite = listeners.find(key);
            if (ite != listeners.end())
            {
                RttiObjectContainer& objs = (*ite).second;
                objs[obj] = true;
            }
            else
            {
                RttiObjectContainer objs;
                objs[obj] = true;
                listeners[key] = objs;
            }
        }
    }

    DwImage freeAcquire(const DwUrl &url , DwByteArray data)
    {
        // load from data
        DwImage source;
        DwString key = url.asString();
        Container::iterator iteImg = images.find(key);
        if (iteImg != images.end())
        {
            source = (*iteImg).second;
            source.loadFromData(data);
            images.remove(iteImg);

            // send resource event
            DwImageLoadFinisedEvent imgEvt;
            imgEvt.url = url;
            imgEvt.img = source;
            dwResourceLib()->sendEvent(&imgEvt);
        }

        // send event
        ListenerContainer::iterator ite = listeners.find(key);
        if (ite != listeners.end())
        {
            RttiObjectContainer objs = (*ite).second;
            listeners.remove(ite);

            RttiObjectContainer::const_iterator iteObj = objs.begin();
            while (iteObj != objs.end())
            {
                // send scene event
                DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>((*iteObj).first);
                if (item)
                {
                    // optimize for pixmap item
                    DwGraphicsPixmapItem *pixmap = dwsafe_cast<DwGraphicsPixmapItem>(item);
                    if (pixmap)
                    {
                        pixmap->setPixmap(source, true);
                    }
                    else
                    {
                        DwGraphicsNinePatchItem * ninePatchs = dwsafe_cast<DwGraphicsNinePatchItem>(item);
                        if (ninePatchs)
                        {
                            ninePatchs->setPixmap(source, true);
                        }

                        DwSceneImageLoadFinisedEvent evt;
                        evt.url = url;
                        evt.img = source;
                        item->sendEvent(&evt);
                    }
                }
                ++iteObj;
            }
        }

        return source;
    }

    // accept the resource raw event
    void onResourceMapEvent(DwRttiObject* obj, DwEvent* evt)
    {
        DW_UNUSED(obj);

        DwResourceLoadFinishedEvent *resEvt = dwsafe_cast<DwResourceLoadFinishedEvent>(evt);
        DwImage img = freeAcquire(resEvt->url, resEvt->data);
    }

    // accept the resource progress raw event
    void onResourceMapProgressEvent(DwRttiObject* obj, DwEvent* evt)
    {
        DW_UNUSED(obj);
        DwResourceLoadProgressEvent *resEvt = dwsafe_cast<DwResourceLoadProgressEvent>(evt);
        DW_ASSERT(resEvt);
        DwString key = resEvt->url.asString();
        // send event
        ListenerContainer::iterator ite = listeners.find(key);
        if (ite != listeners.end())
        {
            RttiObjectContainer objs = (*ite).second;

            RttiObjectContainer::const_iterator iteObj = objs.begin();
            while (iteObj != objs.end())
            {
                // send scene event
                DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>((*iteObj).first);
                if (item)
                {
                    DwSceneResourceLoadProgressEvent evt;
                    evt.url = resEvt->url;
                    evt.size = resEvt->size;
                    evt.progress = resEvt->progress;
                    item->sendEvent(&evt);
                }
                ++iteObj;
            }
        }
    }
};
DW_GLOBAL_STATIC(DwAsyncImageContainer, dwImgAsyncContainer);

//------------------------------------------------------------------------------
/**
    DEAL WITH THE ASYNC RESOURCE EVENT
    support async acquire data
*/
struct DwAsyncDataContainer : public sigslot::has_slots
{
    typedef DwHash<DwString, DwByteArray> Container;
    typedef DwMap<DwRttiObjectPtr<DwRttiObject>, bool > RttiObjectContainer;
    typedef DwHash<DwString, RttiObjectContainer > ListenerContainer;
    Container datas;
    ListenerContainer listeners;

    DwByteArray acquire(const DwUrl &url, DwRttiObject *obj, bool cached )
    {
        // try in cached container
        DwString key = url.asString();
        Container::const_iterator ite = datas.find(key);
        if (ite != datas.end())
        {
            if (listeners.contains(key))
            {
                addListener(key, obj, cached);
            }
            return (*ite).second;
        }

        // create the takeship of resource
        DwByteArray data = dwResourceLib()->lookupRes(url);
        datas[key] = data;

        // listener
        addListener(key, obj, cached);
        return data;
    }

    void addListener(DwString key, DwRttiObject* obj, bool cached )
    {
        if (obj)
        {
            ListenerContainer::iterator ite = listeners.find(key);
            if (ite != listeners.end())
            {
                RttiObjectContainer& objs = (*ite).second;
                objs[obj] = cached;
            }
            else
            {
                RttiObjectContainer objs;
                objs[obj] = cached;
                listeners[key] = objs;
            }
        }
    }

    DwByteArray freeAcquire(const DwUrl &url , const DwByteArray &data)
    {
        // load from data
        DwByteArray source = data;
        DwString key = url.asString();
        Container::iterator iteImg = datas.find(key);
        if (iteImg != datas.end())
        {
            datas.remove(iteImg);

            // send resource event
            DwDataLoadFinisedEvent imgEvt;
            imgEvt.url = url;
            imgEvt.data = source;
            dwResourceLib()->sendEvent(&imgEvt);
        }

        // send event
        ListenerContainer::iterator ite = listeners.find(key);
        if (ite != listeners.end())
        {
            RttiObjectContainer objs = (*ite).second;
            listeners.remove(ite);

            bool saved = false;
            RttiObjectContainer::const_iterator iteObj = objs.begin();
            while (iteObj != objs.end())
            {
                // deal with the cacled flag
                bool cached = (*iteObj).second;
                if (!saved && cached)
                {
                    dwResourceLib()->appendRes(url, source);
                    saved = true;
                }

                // send scene event
                DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>((*iteObj).first);
                if (item)
                {

                    DwSceneDataLoadFinisedEvent evt;
                    evt.url = url;
                    evt.data = source;
                    item->sendEvent(&evt);
                }
                ++iteObj;
            }
        }

        return source;
    }

    // accept the resource raw event
    void onResourceMapEvent(DwRttiObject* obj, DwEvent* evt)
    {
        DW_UNUSED(obj);

        DwResourceLoadFinishedEvent *resEvt = dwsafe_cast<DwResourceLoadFinishedEvent>(evt);
        freeAcquire(resEvt->url, resEvt->data);
    }

    // accept the resource progress raw event
    void onResourceMapProgressEvent(DwRttiObject* obj, DwEvent* evt)
    {
        DW_UNUSED(obj);
        DwResourceLoadProgressEvent *resEvt = dwsafe_cast<DwResourceLoadProgressEvent>(evt);
        DW_ASSERT(resEvt);
        DwString key = resEvt->url.asString();
        ListenerContainer::iterator ite = listeners.find(key);
        if (ite != listeners.end())
        {
            RttiObjectContainer objs = (*ite).second;
            RttiObjectContainer::const_iterator iteObj = objs.begin();
            while (iteObj != objs.end())
            {
                // send scene event
                DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>((*iteObj).first);
                if (item)
                {
                    DwSceneResourceLoadProgressEvent evt;
                    evt.url = resEvt->url;
                    evt.size = resEvt->size;
                    evt.progress = resEvt->progress;
                    item->sendEvent(&evt);
                }
                ++iteObj;
            }
        }
    }
};
DW_GLOBAL_STATIC(DwAsyncDataContainer, dwDataAsyncContainer);

//------------------------------------------------------------------------------
/**
*/
static bool initTheGUIThemeData()
{
    static bool hasInitTheThemeDataInv = false;
    if (!hasInitTheThemeDataInv)
    {
        DW_MOUNTED_DLLRESOURCE(gui, theme);
        hasInitTheThemeDataInv = true;

        // listen to resource finished event
        dwResourceLib()->eventDispatch()->connect(DwResourceLoadFinishedEvent::EventId(), 
            dwImgAsyncContainer(), &DwAsyncImageContainer::onResourceMapEvent);

        // listen to resource progress event
        dwResourceLib()->eventDispatch()->connect(DwResourceLoadProgressEvent::EventId(), 
            dwImgAsyncContainer(), &DwAsyncImageContainer::onResourceMapProgressEvent);

        // listen to resource finished event
        dwResourceLib()->eventDispatch()->connect(DwResourceLoadFinishedEvent::EventId(), 
            dwDataAsyncContainer(), &DwAsyncDataContainer::onResourceMapEvent);

        // listen to resource progress event
        dwResourceLib()->eventDispatch()->connect(DwResourceLoadProgressEvent::EventId(), 
            dwDataAsyncContainer(), &DwAsyncDataContainer::onResourceMapProgressEvent);
    }
    return hasInitTheThemeDataInv;
}

//------------------------------------------------------------------------------
/**
*/
DwImage DwResourceInterface::acquireImage( const DwUrl &url )
{
    initTheGUIThemeData();

    DwString key = url.asString();
    if ( DwImage::hasCachedOf(key) )
    {
        return DwImage(key);
    }
    else
    {
        DwByteArray data = dwResourceLib()->lookupRes(url);
        if (data.size() > 0)
        {
            return DwImage(key, data);
        }

        if (!url.isEmpty() && url.localPath().length() > 0)
        {
            DW_ASSERT(false && "resource do not exits");
        }
    }
    return DwImage();
}

//------------------------------------------------------------------------------
/**
*/
DwImage DwResourceInterface::acquireImage(const DwUrl &url, DwRttiObject *obj)
{
    initTheGUIThemeData();

    bool isAsync = dwResourceLib()->isAsyncResource(url);
    if (isAsync)
    {
        return dwImgAsyncContainer()->acquire(url, obj);
    }
    else
    {
        return acquireImage(url);
    }
}

//------------------------------------------------------------------------------
/**
*/
DwByteArray DwResourceInterface::acquireData(const DwUrl &url, DwRttiObject *obj, bool cached/* = true*/)
{
    initTheGUIThemeData();

    bool isAsync = dwResourceLib()->isAsyncResource(url);
    if (isAsync)
    {
        return dwDataAsyncContainer()->acquire(url, obj, cached);
    }
    else
    {
        return acquireData(url);
    }
}

//------------------------------------------------------------------------------
/**
*/
DwByteArray DwResourceInterface::acquireData( const DwUrl &url )
{
    initTheGUIThemeData();

    return dwResourceLib()->lookupRes(url);
}

//------------------------------------------------------------------------------
/**
*/
bool DwResourceInterface::hasExist(const DwUrl &url)
{
    initTheGUIThemeData();

    return dwResourceLib()->hasRes(url);
}