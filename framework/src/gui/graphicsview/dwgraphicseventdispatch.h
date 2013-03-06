#pragma once
#include "dwgui/dweventdispatch.h"

//------------------------------------------------------------------------------
class DwSceneEvent;
class DwItemEventHandler;
class DwGraphicsItem;
class DwGraphicsView;
class DwGraphicsScene;
class DwRttiObject;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsEventDispatch : public DwEventDispatch
{
public:
    // item event dispatch
    void dispatchEvent(const DwGraphicsItem* item, DwSceneEvent* evt)const ;

    void dispatchEventDown(const DwGraphicsItem* item, DwSceneEvent* evt)const ;

    void dispatchEventUp(const DwGraphicsItem* item, DwSceneEvent* evt)const ;

    // view event dispatch
    void dispatchEvent(const DwGraphicsView* view, DwSceneEvent* evt)const ;

    // scene event dispatch
    void dispatchEvent(const DwGraphicsScene* scene, DwSceneEvent* evt)const ;

    // rtti object event
    void dispatch(const DwRttiObject *obj, DwEvent *evt)const ;

    DwGraphicsEventDispatch();
    virtual ~DwGraphicsEventDispatch();
};// end of DwGraphicsEventDispatch

DW_GUI_EXPORT DwGraphicsEventDispatch *dwGraphicsEventDispatch();