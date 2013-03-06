#include "stable.h"
#include "dweventdelegatescene.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwgraphicsview.h"
#include "export/dwexportutil_p.h"

DW_IMPLEMENT_CLASS(DwEventDelegateScene, 'EDSE', DwEventDelegate);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(doneItem)
{
    DwSceneEvent * sceneEvt = dwsafe_cast<DwSceneEvent>(obj);
    DW_CHECK_ASSERT_RET(sceneEvt, 0);

    sceneEvt->setDoneItem(_lua_itemOf(L, -1));
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(doneItem)
{
    DwSceneEvent * sceneEvt = dwsafe_cast<DwSceneEvent>(obj);
    DW_CHECK_ASSERT_RET(sceneEvt, 0);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, sceneEvt->doneItem());
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(mapToDevice)
{
    DwSceneEvent * sceneEvt = dwsafe_cast<DwSceneEvent>(obj);
    DW_CHECK_ASSERT_RET(sceneEvt, 0);
    DW_ASSERT(sceneEvt->view());

    DwGraphicsView *view = sceneEvt->view();

    int n = 0;
    DwVariant v  = _lua_pop_value_of<DwVariant>(L, 3);
    switch (v.vtype())
    {
    case DwVariant::Point:
        n = _lua_push_value_of<DwPoint>(L, view->mapToDevice(v.as<DwPoint>()));
        break;
    case DwVariant::PointF:
        n = _lua_push_value_of<DwPointF>(L, view->mapToDevice(v.as<DwPointF>()));
        break;
    case DwVariant::Rect:
        n = _lua_push_value_of<DwRect>(L, view->mapToDevice(v.as<DwRect>()));
        break;
    case DwVariant::RectF:
        n = _lua_push_value_of<DwRectF>(L, view->mapToDevice(v.as<DwRectF>()));
        break;
    default:
        DW_ASSERT(false);
        DW_UI_LOGER() << "scene event mapToDevice error type" << "\n";
    }

    return n;
}

DECLARE_DELEGATE_CALL(mapFromDevice)
{
    DwSceneEvent * sceneEvt = dwsafe_cast<DwSceneEvent>(obj);
    DW_CHECK_ASSERT_RET(sceneEvt, 0);
    DW_ASSERT(sceneEvt->view());

    DwGraphicsView *view = sceneEvt->view();

    int n = 0;
    DwVariant v  = _lua_pop_value_of<DwVariant>(L, 3);
    switch (v.vtype())
    {
    case DwVariant::Point:
        n = _lua_push_value_of<DwPoint>(L, view->mapFromDevice(v.as<DwPoint>()));
        break;
    case DwVariant::PointF:
        n = _lua_push_value_of<DwPointF>(L, view->mapFromDevice(v.as<DwPointF>()));
        break;
    case DwVariant::Rect:
        n = _lua_push_value_of<DwRect>(L, view->mapFromDevice(v.as<DwRect>()));
        break;
    case DwVariant::RectF:
        n = _lua_push_value_of<DwRectF>(L, view->mapFromDevice(v.as<DwRectF>()));
        break;
    default:
        DW_ASSERT(false);
        DW_UI_LOGER() << "scene event mapToDevice error type" << "\n";
    }

    return n;
}

DECLARE_DELEGATE_CALL(doneItem)
{
    DwSceneEvent * sceneEvt = dwsafe_cast<DwSceneEvent>(obj);
    DW_CHECK_ASSERT_RET(sceneEvt, 0);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, sceneEvt->doneItem());
}

DECLARE_DELEGATE_CALL(setDoneItem)
{
    DwSceneEvent * sceneEvt = dwsafe_cast<DwSceneEvent>(obj);
    DW_CHECK_ASSERT_RET(sceneEvt, 0);

    sceneEvt->setDoneItem(_lua_itemOf(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(isInterestedBy)
{
    DwSceneEvent * sceneEvt = dwsafe_cast<DwSceneEvent>(obj);
    DW_CHECK_ASSERT_RET(sceneEvt, 0);

    return _lua_push_value_of<bool>(L, sceneEvt->isInterestedBy(_lua_itemOf(L, 3)));
}

DECLARE_DELEGATE_CALL(setInterestItem)
{
    DwSceneEvent * sceneEvt = dwsafe_cast<DwSceneEvent>(obj);
    DW_CHECK_ASSERT_RET(sceneEvt, 0);

    sceneEvt->setInterestItem(_lua_itemOf(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(indexInterestItemFromScene)
{
    DwSceneEvent * sceneEvt = dwsafe_cast<DwSceneEvent>(obj);
    DW_CHECK_ASSERT_RET(sceneEvt, 0);
    DW_ASSERT(sceneEvt->view());
    DW_ASSERT(sceneEvt->view()->scene());

    DwGraphicsItem *item = sceneEvt->indexInterestItemFromScene(sceneEvt->view()->scene());
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, item);
}

DECLARE_DELEGATE_CALL(indexInterestItemFromView)
{
    DwSceneEvent * sceneEvt = dwsafe_cast<DwSceneEvent>(obj);
    DW_CHECK_ASSERT_RET(sceneEvt, 0);
    DW_ASSERT(sceneEvt->view());

    DwGraphicsItem *item = sceneEvt->indexInterestItemFromView(sceneEvt->view());
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, item);
}

DECLARE_DELEGATE_CALL(vars)
{
    DW_UNUSED(L);
    DwSceneEvent *evt = dwsafe_cast<DwSceneEvent>(obj);

    return _lua_push_value_of<DwVariant>(L, evt->vars(_lua_pop_value_of<DwString>(L, -1)));
}

//------------------------------------------------------------------------------
/**
*/
DwEventDelegateScene::DwEventDelegateScene()
{
    DECLARE_ADD_SET(doneItem);
    DECLARE_ADD_GET(doneItem);

    DECLARE_ADD_CALL(mapToDevice);
    DECLARE_ADD_CALL(mapFromDevice);
    DECLARE_ADD_CALL(doneItem);
    DECLARE_ADD_CALL(setDoneItem);
    DECLARE_ADD_CALL(isInterestedBy);
    DECLARE_ADD_CALL(setInterestItem);
    DECLARE_ADD_CALL(indexInterestItemFromScene);
    DECLARE_ADD_CALL(indexInterestItemFromView);
    DECLARE_ADD_CALL(vars);
}