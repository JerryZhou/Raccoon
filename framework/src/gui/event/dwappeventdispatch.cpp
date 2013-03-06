#include "stable.h"
#include "dwappeventdispatch.h"
#include "dwappeventdispatch_p.h"
#include "dwgui/dwgraphicseventdispatch.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwanimationevent.h"
#include "dwgui/dwpluginevent.h"
#include "dwgui/dwplugineventdispatch.h"
#include "dwgui/dwpluginlib.h"
#include "dwgui/dwgraphicsitem.h"
#include "dwgui/dwgraphicsscene.h"
#include "dwgui/dwgraphicsview.h"

//------------------------------------------------------------------------------
/**
*/
DwAppEventDispatch::DwAppEventDispatch()
    : d(new DwAppEventDispatchData())
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwAppEventDispatch::~DwAppEventDispatch()
{
    DW_SAFE_DELETE(d);
}

//------------------------------------------------------------------------------
/**
*/
void DwAppEventDispatch::dispatchEvent(DwEvent *evt)
{
    if (evt->isA(DwSceneEvent::EventId()))
    {
        DwSceneEvent *sceneEvt = dwsafe_cast<DwSceneEvent>(evt);
        DwGraphicsView *view = sceneEvt->view();
        if (view)
        {
            dwGraphicsEventDispatch()->dispatchEvent(view, sceneEvt);
        }
        else
        {
            DwGraphicsItem *item = sceneEvt->indexInterestItemFromView(NULL);
            DwGraphicsScene *scene = item ? item->scene() : NULL;
            if (scene)
            {
                dwGraphicsEventDispatch()->dispatchEvent(scene, sceneEvt);
            }
            else if (item)
            {
                dwGraphicsEventDispatch()->dispatchEvent(item, sceneEvt);
            }
            else
            {
                DW_UI_LOGER() << "DwAppEventDispatch::dispatchEvent : failed to dispatch event "
                              << sceneEvt->eventId()->name() << "\n";
            }
        }
    }
    else if (evt->isA(DwPluginEvent::EventId()))
    {
        DwPluginEvent *pluginEvt = dwsafe_cast<DwPluginEvent>(evt);
        dwPluginEventDispatch()->dispatchEvent(dwPluginLib(), pluginEvt);
    }
    else
    {
        DW_UI_LOGER() << "DwAppEventDispatch::dispatchEvent : we do nont know how to dispatch event "
                      << evt->eventId()->name() << "\n";
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAppEventDispatch::dispatchEventAsyncHelper(DwEvent *evt, int sec/* = 0*/)
{
    d->dispatch(evt, sec);
}

//------------------------------------------------------------------------------
/**
*/
DW_GLOBAL_STATIC(DwAppEventDispatch, _dw_AppEventDispatch);
DwAppEventDispatch *dwAppEventDispatch()
{
    return _dw_AppEventDispatch();
}