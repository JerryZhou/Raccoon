#include "stable.h"
#include "dweventdelegatefocus.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwgraphicsevent.h"

DW_IMPLEMENT_CLASS(DwEventDelegateFocus, 'EDUS', DwEventDelegateScene);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(reason)
{
    DwFocusEvent * focusEvt = dwsafe_cast<DwFocusEvent>(obj);
    DW_CHECK_ASSERT_RET(focusEvt, 0);

    focusEvt->m_reason = (Dw::FocusReason)_lua_pop_value_of<int>(L, -1);
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(reason)
{
    DwFocusEvent * focusEvt = dwsafe_cast<DwFocusEvent>(obj);
    DW_CHECK_ASSERT_RET(focusEvt, 0);

    return _lua_push_value_of<int>(L, (int)(focusEvt->m_reason));
}

//------------------------------------------------------------------------------
/**
*/
DwEventDelegateFocus::DwEventDelegateFocus()
{
    DECLARE_ADD_SET(reason);
    DECLARE_ADD_GET(reason);
}