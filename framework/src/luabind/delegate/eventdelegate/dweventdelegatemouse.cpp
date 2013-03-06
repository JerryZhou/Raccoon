#include "stable.h"
#include "dweventdelegatemouse.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwgraphicsevent.h"

DW_IMPLEMENT_CLASS(DwEventDelegateMouse, 'EDME', DwEventDelegateScene);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(screenPoint)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    mouseEvt->m_screenPoint = ( _lua_pop_value_of<DwPointF>(L, -1) ).toPoint();
    return 0;
}

DECLARE_DELEGATE_SET(devicePoint)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    mouseEvt->m_devicePoint = ( _lua_pop_value_of<DwPointF>(L, -1) ).toPoint();
    return 0;
}

DECLARE_DELEGATE_SET(viewPoint)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    mouseEvt->m_viewPoint = ( _lua_pop_value_of<DwPointF>(L, -1) ).toPoint();
    return 0;
}

DECLARE_DELEGATE_SET(scenePoint)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    mouseEvt->m_scenePoint = ( _lua_pop_value_of<DwPointF>(L, -1) ).toPoint();
    return 0;
}

DECLARE_DELEGATE_SET(itemPoint)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    mouseEvt->m_itemPoint = ( _lua_pop_value_of<DwPointF>(L, -1) ).toPoint();
    return 0;
}

DECLARE_DELEGATE_SET(button)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    mouseEvt->m_button = (Dw::MouseButton)( _lua_pop_value_of<int>(L, -1) );
    return 0;
}

DECLARE_DELEGATE_SET(nc)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    mouseEvt->m_nc = (unsigned int)( _lua_pop_value_of<int>(L, -1) );
    return 0;
}

DECLARE_DELEGATE_SET(isNc)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    mouseEvt->m_isNC = ( _lua_pop_value_of<bool>(L, -1) );
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(screenPoint)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    return _lua_push_value_of<DwPointF>(L, DwPointF(mouseEvt->m_screenPoint));
}

DECLARE_DELEGATE_GET(devicePoint)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    return _lua_push_value_of<DwPointF>(L, DwPointF(mouseEvt->m_devicePoint));
}

DECLARE_DELEGATE_GET(viewPoint)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    return _lua_push_value_of<DwPointF>(L, DwPointF(mouseEvt->m_viewPoint));
}

DECLARE_DELEGATE_GET(scenePoint)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    return _lua_push_value_of<DwPointF>(L, DwPointF(mouseEvt->m_scenePoint));
}

DECLARE_DELEGATE_GET(itemPoint)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    return _lua_push_value_of<DwPointF>(L, DwPointF(mouseEvt->m_itemPoint));
}

DECLARE_DELEGATE_GET(button)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    return _lua_push_value_of<int>(L, int(mouseEvt->m_button));
}

DECLARE_DELEGATE_GET(nc)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    return _lua_push_value_of<int>(L, int(mouseEvt->m_nc));
}

DECLARE_DELEGATE_GET(isNc)
{
    DwMouseEvent * mouseEvt = dwsafe_cast<DwMouseEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    return _lua_push_value_of<bool>(L, mouseEvt->m_isNC);
}

//------------------------------------------------------------------------------
/**
*/
DwEventDelegateMouse::DwEventDelegateMouse()
{
    DECLARE_ADD_SET(screenPoint);
    DECLARE_ADD_SET(devicePoint);
    DECLARE_ADD_SET(viewPoint);
    DECLARE_ADD_SET(scenePoint);
    DECLARE_ADD_SET(itemPoint);
    DECLARE_ADD_SET(button);
    DECLARE_ADD_SET(nc);
    DECLARE_ADD_SET(isNc);

    DECLARE_ADD_GET(screenPoint);
    DECLARE_ADD_GET(devicePoint);
    DECLARE_ADD_GET(viewPoint);
    DECLARE_ADD_GET(scenePoint);
    DECLARE_ADD_GET(itemPoint);
    DECLARE_ADD_GET(button);
    DECLARE_ADD_GET(nc);
    DECLARE_ADD_GET(isNc);
}