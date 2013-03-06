#include "stable.h"
#include "dwitemdelegatewidgetitem.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwwidgetitem.h"
#include "dwgui/dwgraphicsframeitem.h"
#include "dwgui/dwgraphicsbackgrounditem.h"

DW_IMPLEMENT_CLASS(DwItemDelegateWidgetItem, 'IDWI', DwItemDelegate);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(dragPolicy)
{
    DwWidgetItem *widget = dwsafe_cast<DwWidgetItem>(obj);
    DW_CHECK_ASSERT_RET(widget, 0);

    widget->setDragPolicy((DwWidget::DragPolicy)(_lua_pop_value_of<int>(L, -1)));
    return 0;
}

DECLARE_DELEGATE_SET(backgroundColor)
{
    DwWidgetItem *widget = dwsafe_cast<DwWidgetItem>(obj);
    DW_CHECK_ASSERT_RET(widget, 0);

    //widget->background()->setBackgroundColor(_lua_pop_value_of<DwColor>(L, -1));
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(dragPolicy)
{
    DwWidgetItem *widget = dwsafe_cast<DwWidgetItem>(obj);
    DW_CHECK_ASSERT_RET(widget, 0);

    return _lua_push_value_of<int>(L, (int)(widget->dragPolicy()));
}

DECLARE_DELEGATE_GET(background)
{
    DwWidgetItem *widget = dwsafe_cast<DwWidgetItem>(obj);
    DW_CHECK_ASSERT_RET(widget, 0);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, widget->background());
}

DECLARE_DELEGATE_GET(backgroundColor)
{
    DwWidgetItem *widget = dwsafe_cast<DwWidgetItem>(obj);
    DW_CHECK_ASSERT_RET(widget, 0);

    //return _lua_push_value_of<DwColor>(L,  widget->background()->backgroundColor());
    return _lua_push_value_of<DwColor>(L,  DwColor());
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(background)
{
    DwWidgetItem *widget = dwsafe_cast<DwWidgetItem>(obj);
    DW_CHECK_ASSERT_RET(widget, 0);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, widget->background());
}

DECLARE_DELEGATE_CALL(setDragPolicy)
{
    DwWidgetItem *widget = dwsafe_cast<DwWidgetItem>(obj);
    DW_CHECK_ASSERT_RET(widget, 0);

    widget->setDragPolicy((DwWidget::DragPolicy)(_lua_pop_value_of<int>(L, 3)));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DwItemDelegateWidgetItem::DwItemDelegateWidgetItem()
{
    DECLARE_ADD_SET(dragPolicy);
    DECLARE_ADD_SET(backgroundColor);

    DECLARE_ADD_GET(dragPolicy);
    DECLARE_ADD_GET(background);
    DECLARE_ADD_GET(backgroundColor);

    DECLARE_ADD_CALL(background);
    DECLARE_ADD_CALL(setDragPolicy);
}