#include "stable.h"
#include "dwitemdelegatescrollbar.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwscrollbar.h"
#include "dwgui/dwscrollarea.h"
#include "dwgui/dwpushbutton.h"

DW_IMPLEMENT_CLASS(DwItemDelegateScrollBar, 'DSBR', DwItemDelegate);

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_SET(area)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->setArea(dwsafe_cast<DwScrollArea>(_lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L, -1)));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_SET(flow)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->setFlow(DwWidget::Flow(_lua_pop_value_of<int>(L, -1)));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_SET(range)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    LUA_3Real c = _lua_pop_value_of<LUA_3Real>(L, -1);
    bar->setRange(c.x, c.y, c.z);
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_SET(cur)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->setCur(_lua_pop_value_of<DwReal>(L, -1));
    return 0;
}
//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(area)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, bar->area());
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(minHit)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, bar->minHit());
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(maxHit)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, bar->maxHit());
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(thumb)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, bar->thumb());
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(flow)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    return _lua_push_value_of<int>(L, (int)bar->flow());
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(cur)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    return _lua_push_value_of<DwReal>(L, bar->cur());
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(step)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    return _lua_push_value_of<DwReal>(L, bar->step());
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(min)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    return _lua_push_value_of<DwReal>(L, bar->min());
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(max)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    return _lua_push_value_of<DwReal>(L, bar->max());
}
//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(setArea)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->setArea(dwsafe_cast<DwScrollArea>(_lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L, -1)));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_CALL(setFlow)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->setFlow(DwWidget::Flow(_lua_pop_value_of<int>(L, -1)));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_CALL(setRange)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    LUA_3Real c = _lua_pop_value_of<LUA_3Real>(L, -1);
    bar->setRange(c.x, c.y, c.z);
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_CALL(setCur)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->setCur(_lua_pop_value_of<DwReal>(L, -1));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_CALL(scroll)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->scroll(_lua_pop_value_of<DwReal>(L, -1));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_CALL(scrollUp)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->scrollUp(_lua_pop_value_of<DwReal>(L, -1));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_CALL(scrollDown)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->scrollUp(_lua_pop_value_of<DwReal>(L, -1));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_CALL(piexl2Range)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    DwReal range = bar->piexl2Range(_lua_pop_value_of<DwReal>(L, -1));
    return _lua_push_value_of<DwReal>(L, range);
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_CALL(updateState)
{
    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->updateState();
    return 0;
}
//------------------------------------------------------------------------------
/**
*/
DwItemDelegateScrollBar::DwItemDelegateScrollBar()
{
    DECLARE_ADD_SET(area);
    DECLARE_ADD_SET(flow);
    DECLARE_ADD_SET(range);
    DECLARE_ADD_SET(cur);

    DECLARE_ADD_GET(area);
    DECLARE_ADD_GET(flow);
    DECLARE_ADD_GET(minHit);
    DECLARE_ADD_GET(maxHit);
    DECLARE_ADD_GET(thumb);
    DECLARE_ADD_GET(cur);
    DECLARE_ADD_GET(step);
    DECLARE_ADD_GET(min);
    DECLARE_ADD_GET(max);

    DECLARE_ADD_CALL(setArea);
    DECLARE_ADD_CALL(setFlow);
    DECLARE_ADD_CALL(setRange);
    DECLARE_ADD_CALL(setCur);
    DECLARE_ADD_CALL(scroll);
    DECLARE_ADD_CALL(scrollUp);
    DECLARE_ADD_CALL(scrollDown);
    DECLARE_ADD_CALL(piexl2Range);
    DECLARE_ADD_CALL(updateState);
}