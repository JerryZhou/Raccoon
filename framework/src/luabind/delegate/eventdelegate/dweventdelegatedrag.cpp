#include "stable.h"
#include "dweventdelegatedrag.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwgraphicsevent.h"

DW_IMPLEMENT_CLASS(DwEventDelegateDrag, 'EDDG', DwEventDelegateScene);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(screenPoint)
{
    DwSceneDragEvent * dragEvt = dwsafe_cast<DwSceneDragEvent>(obj);
    DW_CHECK_ASSERT_RET(dragEvt, 0);

    dragEvt->m_screenPoint = _lua_pop_value_of<DwPointF>(L, -1).toPoint();
    return 0;
}

DECLARE_DELEGATE_SET(devicePoint)
{
    DwSceneDragEvent * dragEvt = dwsafe_cast<DwSceneDragEvent>(obj);
    DW_CHECK_ASSERT_RET(dragEvt, 0);

    dragEvt->m_devicePoint = _lua_pop_value_of<DwPointF>(L, -1).toPoint();
    return 0;
}

DECLARE_DELEGATE_SET(viewPoint)
{
    DwSceneDragEvent * dragEvt = dwsafe_cast<DwSceneDragEvent>(obj);
    DW_CHECK_ASSERT_RET(dragEvt, 0);

    dragEvt->m_viewPoint = _lua_pop_value_of<DwPointF>(L, -1).toPoint();
    return 0;
}

DECLARE_DELEGATE_SET(scenePoint)
{
    DwSceneDragEvent * dragEvt = dwsafe_cast<DwSceneDragEvent>(obj);
    DW_CHECK_ASSERT_RET(dragEvt, 0);

    dragEvt->m_scenePoint = _lua_pop_value_of<DwPointF>(L, -1).toPoint();
    return 0;
}

DECLARE_DELEGATE_SET(button)
{
    DwSceneDragEvent * dragEvt = dwsafe_cast<DwSceneDragEvent>(obj);
    DW_CHECK_ASSERT_RET(dragEvt, 0);

    dragEvt->m_button = (Dw::MouseButton)_lua_pop_value_of<int>(L, -1);
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(screenPoint)
{
    DwSceneDragEvent * dragEvt = dwsafe_cast<DwSceneDragEvent>(obj);
    DW_CHECK_ASSERT_RET(dragEvt, 0);

    return _lua_push_value_of<DwPointF>(L, (DwPointF)(dragEvt->m_screenPoint));
}

DECLARE_DELEGATE_GET(devicePoint)
{
    DwSceneDragEvent * dragEvt = dwsafe_cast<DwSceneDragEvent>(obj);
    DW_CHECK_ASSERT_RET(dragEvt, 0);

    return _lua_push_value_of<DwPointF>(L, (DwPointF)(dragEvt->m_devicePoint));
}

DECLARE_DELEGATE_GET(viewPoint)
{
    DwSceneDragEvent * dragEvt = dwsafe_cast<DwSceneDragEvent>(obj);
    DW_CHECK_ASSERT_RET(dragEvt, 0);

    return _lua_push_value_of<DwPointF>(L, (DwPointF)(dragEvt->m_viewPoint));
}

DECLARE_DELEGATE_GET(scenePoint)
{
    DwSceneDragEvent * dragEvt = dwsafe_cast<DwSceneDragEvent>(obj);
    DW_CHECK_ASSERT_RET(dragEvt, 0);

    return _lua_push_value_of<DwPointF>(L, (DwPointF)(dragEvt->m_scenePoint));
}

DECLARE_DELEGATE_GET(button)
{
    DwSceneDragEvent * dragEvt = dwsafe_cast<DwSceneDragEvent>(obj);
    DW_CHECK_ASSERT_RET(dragEvt, 0);

    return _lua_push_value_of<int>(L, (int)(dragEvt->m_button));
}

//------------------------------------------------------------------------------
/**
*/
DwEventDelegateDrag::DwEventDelegateDrag()
{
    DECLARE_ADD_SET(screenPoint);
    DECLARE_ADD_SET(devicePoint);
    DECLARE_ADD_SET(viewPoint);
    DECLARE_ADD_SET(scenePoint);
    DECLARE_ADD_SET(button);

    DECLARE_ADD_GET(screenPoint);
    DECLARE_ADD_GET(devicePoint);
    DECLARE_ADD_GET(viewPoint);
    DECLARE_ADD_GET(scenePoint);
    DECLARE_ADD_GET(button);
}