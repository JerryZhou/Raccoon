#include "stable.h"
#include "dwitemdelegateprogressbar.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwscrollbar.h"
#include "dwgui/dwscrollarea.h"
#include "dwgui/dwpushbutton.h"
#include "dwgui/dwprogressbar.h"

DW_IMPLEMENT_CLASS(DwItemDelegateProgressBar, 'DPAR', DwItemDelegate);

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_SET(min)
{
    DwProgressBar *bar = dwsafe_cast<DwProgressBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->setMin((_lua_pop_value_of<DwReal>(L, -1)));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_SET(max)
{
    DwProgressBar *bar = dwsafe_cast<DwProgressBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->setMax((_lua_pop_value_of<DwReal>(L, -1)));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_SET(cur)
{
    DwProgressBar *bar = dwsafe_cast<DwProgressBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->setCur((_lua_pop_value_of<DwReal>(L, -1)));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_SET(margins)
{
    DwProgressBar *bar = dwsafe_cast<DwProgressBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->setMargins((_lua_pop_value_of<DwMargins>(L, -1)));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_SET(flow)
{
    DwProgressBar *bar = dwsafe_cast<DwProgressBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    bar->setFlow((DwWidget::Flow)_lua_pop_value_of<int>(L, -1));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(min)
{
    DwProgressBar *bar = dwsafe_cast<DwProgressBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    return _lua_push_value_of<DwReal>(L, bar->min());
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(max)
{
    DwProgressBar *bar = dwsafe_cast<DwProgressBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    return _lua_push_value_of<DwReal>(L, bar->max());
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(cur)
{
    DwProgressBar *bar = dwsafe_cast<DwProgressBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    return _lua_push_value_of<DwReal>(L, bar->cur());
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(margins)
{
    DwProgressBar *bar = dwsafe_cast<DwProgressBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    return _lua_push_value_of<DwMargins>(L, bar->margins());
}

//------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_GET(flow)
{
    DwProgressBar *bar = dwsafe_cast<DwProgressBar>(obj);
    DW_CHECK_ASSERT_RET(bar, 0);

    return _lua_push_value_of<int>(L, (int)bar->flow());
}

//////////////////////////////////////////////////////////////////////////
////------------------------------------------------------------------------------
///**
//*/
//DECLARE_DELEGATE_CALL(setFlow)
//{
//    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
//    DW_CHECK_ASSERT_RET(bar, 0);
//
//    bar->setFlow(DwWidget::Flow(_lua_pop_value_of<int>(L, -1)));
//    return 0;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//DECLARE_DELEGATE_CALL(setRange)
//{
//    DwScrollBar *bar = dwsafe_cast<DwScrollBar>(obj);
//    DW_CHECK_ASSERT_RET(bar, 0);
//
//    LUA_3Real c = _lua_pop_value_of<LUA_3Real>(L, -1);
//    bar->setRange(c.x, c.y, c.z);
//    return 0;
//}

//------------------------------------------------------------------------------
/**
*/
DwItemDelegateProgressBar::DwItemDelegateProgressBar()
{
    DECLARE_ADD_SET(min);
    DECLARE_ADD_SET(max);
    DECLARE_ADD_SET(cur);
    DECLARE_ADD_SET(margins);
    DECLARE_ADD_SET(flow);

    DECLARE_ADD_GET(min);
    DECLARE_ADD_GET(max);
    DECLARE_ADD_GET(cur);
    DECLARE_ADD_GET(margins);
    DECLARE_ADD_GET(flow);
}