#include "stable.h"
#include "dweventdelegateitemchange.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwgraphicsevent.h"

DW_IMPLEMENT_CLASS(DwEventDelegateItemChange, 'EDIC', DwEventDelegateGraphicsItem);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(change)
{
    DwItemChangeEvent * changeEvt = dwsafe_cast<DwItemChangeEvent>(obj);
    DW_CHECK_ASSERT_RET(changeEvt, 0);

    changeEvt->m_change = Dw::GraphicsItemChange(_lua_pop_value_of<int>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(oldOne)
{
    DwItemChangeEvent * changeEvt = dwsafe_cast<DwItemChangeEvent>(obj);
    DW_CHECK_ASSERT_RET(changeEvt, 0);

    changeEvt->m_oldOne = _lua_pop_value_of<DwVariant>(L, -1);
    return 0;
}

DECLARE_DELEGATE_SET(newOne)
{
    DwItemChangeEvent * changeEvt = dwsafe_cast<DwItemChangeEvent>(obj);
    DW_CHECK_ASSERT_RET(changeEvt, 0);

    changeEvt->m_newOne = _lua_pop_value_of<DwVariant>(L, -1);
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(change)
{
    DwItemChangeEvent * changeEvt = dwsafe_cast<DwItemChangeEvent>(obj);
    DW_CHECK_ASSERT_RET(changeEvt, 0);

    return _lua_push_value_of<int>(L, (int)(changeEvt->m_change));
}

DECLARE_DELEGATE_GET(oldOne)
{
    DwItemChangeEvent * changeEvt = dwsafe_cast<DwItemChangeEvent>(obj);
    DW_CHECK_ASSERT_RET(changeEvt, 0);

    return _lua_push_value_of<DwVariant>(L, (changeEvt->m_oldOne));
}

DECLARE_DELEGATE_GET(newOne)
{
    DwItemChangeEvent * changeEvt = dwsafe_cast<DwItemChangeEvent>(obj);
    DW_CHECK_ASSERT_RET(changeEvt, 0);

    return _lua_push_value_of<DwVariant>(L, (changeEvt->m_newOne));
}

//------------------------------------------------------------------------------
/**
*/
DwEventDelegateItemChange::DwEventDelegateItemChange()
{
    DECLARE_ADD_SET(change);
    DECLARE_ADD_SET(oldOne);
    DECLARE_ADD_SET(newOne);

    DECLARE_ADD_GET(change);
    DECLARE_ADD_GET(oldOne);
    DECLARE_ADD_GET(newOne);
}