#include "stable.h"
#include "dwitemdelegatescrollarea.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwscrollarea.h"

DW_IMPLEMENT_CLASS(DwItemDelegateScrollArea, 'DSAA', DwItemDelegate);

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_SET(content)
{
    DwScrollArea *area = dwsafe_cast<DwScrollArea>(obj);
    DW_CHECK_ASSERT_RET(area, 0);

    area->setContent(_lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L, -1));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(content)
{
    DwScrollArea *area = dwsafe_cast<DwScrollArea>(obj);
    DW_CHECK_ASSERT_RET(area, 0);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, area->content());
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(setContent)
{
    DwScrollArea *area = dwsafe_cast<DwScrollArea>(obj);
    DW_CHECK_ASSERT_RET(area, 0);

    area->setContent(_lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L, -1));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DwItemDelegateScrollArea::DwItemDelegateScrollArea()
{
    DECLARE_ADD_SET(content);
    DECLARE_ADD_GET(content);
    DECLARE_ADD_CALL(setContent);
}