#include "dwresourceevent.h"

DW_IMPLEMENT_EVENTID(DwResourceEvent, DwEvent);
//------------------------------------------------------------------------------
/**
*/
DwResourceEvent::DwResourceEvent()
{

}

//------------------------------------------------------------------------------
/**
*/
DwResourceEvent::~DwResourceEvent()
{

}

DW_IMPLEMENT_EVENTID(DwResourceLoadFinishedEvent, DwResourceEvent);
//------------------------------------------------------------------------------
/**
*/
DwResourceLoadFinishedEvent::DwResourceLoadFinishedEvent()
{

}

//------------------------------------------------------------------------------
/**
*/
DwResourceLoadFinishedEvent::~DwResourceLoadFinishedEvent()
{

}

DW_IMPLEMENT_EVENTID(DwResourceLoadProgressEvent, DwResourceEvent);
//------------------------------------------------------------------------------
/**
*/
DwResourceLoadProgressEvent::DwResourceLoadProgressEvent()
: size(0)
, progress(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwResourceLoadProgressEvent::~DwResourceLoadProgressEvent()
{

}

DW_IMPLEMENT_EVENTID(DwSceneResourceLoadProgressEvent, DwSceneEvent);
//------------------------------------------------------------------------------
/**
*/
DwSceneResourceLoadProgressEvent::DwSceneResourceLoadProgressEvent()
: size(0)
, progress(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwSceneResourceLoadProgressEvent::~DwSceneResourceLoadProgressEvent()
{

}

//------------------------------------------------------------------------------
/**
*/
DwVariant DwSceneResourceLoadProgressEvent::vars(const DwString& key) const
{
    if (key == "size")
    {
        return DwVariant(this->size);
    }
    else if (key == "progress")
    {
        return DwVariant(this->progress);
    }
    else if (key == "percent")
    {
        return DwVariant( size ? DwReal(1.0*(progress)/(size)) : 1.0 );
    }
    else if (key == "url")
    {
        return DwVariant(this->url.asString());
    }

    return DwSceneEvent::vars(key);
}


DW_IMPLEMENT_EVENTID(DwImageLoadFinisedEvent, DwResourceEvent);
//------------------------------------------------------------------------------
/**
*/
DwImageLoadFinisedEvent::DwImageLoadFinisedEvent()
{

}

//------------------------------------------------------------------------------
/**
*/
DwImageLoadFinisedEvent::~DwImageLoadFinisedEvent()
{

}

DW_IMPLEMENT_EVENTID(DwSceneImageLoadFinisedEvent, DwSceneEvent);
//------------------------------------------------------------------------------
/**
*/
DwSceneImageLoadFinisedEvent::DwSceneImageLoadFinisedEvent()
{

}

//------------------------------------------------------------------------------
/**
*/
DwSceneImageLoadFinisedEvent::~DwSceneImageLoadFinisedEvent()
{

}

DW_IMPLEMENT_EVENTID(DwDataLoadFinisedEvent, DwResourceEvent);
//------------------------------------------------------------------------------
/**
*/
DwDataLoadFinisedEvent::DwDataLoadFinisedEvent()
{

}

//------------------------------------------------------------------------------
/**
*/
DwDataLoadFinisedEvent::~DwDataLoadFinisedEvent()
{

}

DW_IMPLEMENT_EVENTID(DwSceneDataLoadFinisedEvent, DwSceneEvent);
//------------------------------------------------------------------------------
/**
*/
DwSceneDataLoadFinisedEvent::DwSceneDataLoadFinisedEvent()
{

}

//------------------------------------------------------------------------------
/**
*/
DwSceneDataLoadFinisedEvent::~DwSceneDataLoadFinisedEvent()
{

}
