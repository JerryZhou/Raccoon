#include "stable.h"
#include "dweventdelegatemousewheel.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwgraphicsevent.h"

DW_IMPLEMENT_CLASS(DwEventDelegateMouseWheel, 'EDMW', DwEventDelegateMouse);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(absDelta)
{
    DwMouseWheelEvent * mouseEvt = dwsafe_cast<DwMouseWheelEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    mouseEvt->m_delta = ( _lua_pop_value_of<DwReal>(L, -1) );
    return 0;
}

DECLARE_DELEGATE_SET(virtualKeyState)
{
    DwMouseWheelEvent * mouseEvt = dwsafe_cast<DwMouseWheelEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    mouseEvt->m_fwKeys = (Dw::MouseVirtualKeyState)( _lua_pop_value_of<int>(L, -1) );
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(isRollUp)
{
    DwMouseWheelEvent * mouseEvt = dwsafe_cast<DwMouseWheelEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    return _lua_push_value_of<bool>(L, (mouseEvt->isRollUp()));
}

DECLARE_DELEGATE_GET(isRollDown)
{
    DwMouseWheelEvent * mouseEvt = dwsafe_cast<DwMouseWheelEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    return _lua_push_value_of<bool>(L, (mouseEvt->isRollDown()));
}

DECLARE_DELEGATE_GET(absDelta)
{
    DwMouseWheelEvent * mouseEvt = dwsafe_cast<DwMouseWheelEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    return _lua_push_value_of<DwReal>(L, (mouseEvt->absDelta()));
}

DECLARE_DELEGATE_GET(virtualKeyState)
{
    DwMouseWheelEvent * mouseEvt = dwsafe_cast<DwMouseWheelEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    return _lua_push_value_of<int>(L, int(mouseEvt->m_fwKeys));
}

//------------------------------------------------------------------------------
/**
*/
DwEventDelegateMouseWheel::DwEventDelegateMouseWheel()
{
    DECLARE_ADD_SET(absDelta);
    DECLARE_ADD_SET(virtualKeyState);

    DECLARE_ADD_GET(isRollUp);
    DECLARE_ADD_GET(isRollDown);
    DECLARE_ADD_GET(absDelta);
    DECLARE_ADD_GET(virtualKeyState);
}