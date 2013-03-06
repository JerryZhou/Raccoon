#include "stable.h"
#include "dweventdelegatetips.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwgraphicsevent.h"

DW_IMPLEMENT_CLASS(DwEventDelegateTips, 'EDTS', DwEventDelegateScene);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(tips)
{
    DwTipsEvent * tipsEvt = dwsafe_cast<DwTipsEvent>(obj);
    DW_CHECK_ASSERT_RET(tipsEvt, 0);

    tipsEvt->m_tips = _lua_pop_value_of<DwString>(L, -1);
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(tips)
{
    DwTipsEvent * tipsEvt = dwsafe_cast<DwTipsEvent>(obj);
    DW_CHECK_ASSERT_RET(tipsEvt, 0);

    return _lua_push_value_of<DwString>(L, (tipsEvt->m_tips));
}

//------------------------------------------------------------------------------
/**
*/
DwEventDelegateTips::DwEventDelegateTips()
{
    DECLARE_ADD_SET(tips);
    DECLARE_ADD_GET(tips);
}