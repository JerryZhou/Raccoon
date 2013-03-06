#pragma once
#include "dwgui/dwevent.h"
#include "dwgui/dweventid.h"
#include "dwgui/dwrttiobjectptr.h"
#include "dwgui/dwurl.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwcore/dwvector.h"
#include "dwgraphics/dwimage.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwResourceEvent : public DwEvent
{
    DW_DECLARE_EVENTID(DwResourceEvent);
public:
    DwResourceEvent();
    virtual ~DwResourceEvent();

    DwUrl url;
};// end of DwResourceEvent
DW_REGISTER_CLASS(DwResourceEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwResourceLoadFinishedEvent : public DwResourceEvent
{
    DW_DECLARE_EVENTID(DwResourceLoadFinishedEvent);
public:
    DwResourceLoadFinishedEvent();
    virtual ~DwResourceLoadFinishedEvent();

    DwByteArray data;
};// end of DwResourceLoadFinishedEvent
DW_REGISTER_CLASS(DwResourceLoadFinishedEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwResourceLoadProgressEvent : public DwResourceEvent
{
    DW_DECLARE_EVENTID(DwResourceLoadProgressEvent);
public:
    DwResourceLoadProgressEvent();
    virtual ~DwResourceLoadProgressEvent();

    int size;
    int progress;
};// end of DwResourceLoadProgressEvent
DW_REGISTER_CLASS(DwResourceLoadProgressEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwSceneResourceLoadProgressEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwSceneResourceLoadProgressEvent);
public:
    DwSceneResourceLoadProgressEvent();
    virtual ~DwSceneResourceLoadProgressEvent();

    // will ported to script
    virtual DwVariant vars(const DwString& key) const;

    DwUrl url;
    int size;
    int progress;
};// end of DwSceneResourceLoadProgressEvent
DW_REGISTER_CLASS(DwSceneResourceLoadProgressEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwImageLoadFinisedEvent : public DwResourceEvent
{
    DW_DECLARE_EVENTID(DwImageLoadFinisedEvent);
public:
    DwImageLoadFinisedEvent();
    virtual ~DwImageLoadFinisedEvent();

    DwUrl url;
    DwImage img;
};// end of DwImageLoadFinisedEvent
DW_REGISTER_CLASS(DwImageLoadFinisedEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwSceneImageLoadFinisedEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwSceneImageLoadFinisedEvent);
public:
    DwSceneImageLoadFinisedEvent();
    virtual ~DwSceneImageLoadFinisedEvent();

    DwUrl url;
    DwImage img;
};// end of DwSceneImageLoadFinisedEvent
DW_REGISTER_CLASS(DwSceneImageLoadFinisedEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwDataLoadFinisedEvent : public DwResourceEvent
{
    DW_DECLARE_EVENTID(DwDataLoadFinisedEvent);
public:
    DwDataLoadFinisedEvent();
    virtual ~DwDataLoadFinisedEvent();

    DwUrl url;
    DwByteArray data;
};// end of DwDataLoadFinisedEvent
DW_REGISTER_CLASS(DwDataLoadFinisedEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwSceneDataLoadFinisedEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwSceneDataLoadFinisedEvent);
public:
    DwSceneDataLoadFinisedEvent();
    virtual ~DwSceneDataLoadFinisedEvent();

    DwUrl url;
    DwByteArray data;
};// end of DwSceneDataLoadFinisedEvent
DW_REGISTER_CLASS(DwSceneDataLoadFinisedEvent);