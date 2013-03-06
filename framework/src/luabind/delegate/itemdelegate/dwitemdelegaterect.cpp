#include "stable.h"
#include "dwitemdelegaterect.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwgraphicsrectitem.h"

DW_IMPLEMENT_CLASS(DwItemDelegateRect, 'IDRT', DwItemDelegate);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(color)
{
    DwGraphicsRectItem *rect = dwsafe_cast<DwGraphicsRectItem>(obj);
    DW_CHECK_ASSERT_RET(rect, 0);

    rect->setColor(_lua_pop_value_of<DwColor>(L, -1));
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(color)
{
    DwGraphicsRectItem *rect = dwsafe_cast<DwGraphicsRectItem>(obj);
    DW_CHECK_ASSERT_RET(rect, 0);

    return _lua_push_value_of<DwColor>(L, rect->color());
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(setColor)
{
    DwGraphicsRectItem *rect = dwsafe_cast<DwGraphicsRectItem>(obj);
    DW_CHECK_ASSERT_RET(rect, 0);

    rect->setColor(_lua_pop_value_of<DwColor>(L, 3));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DwItemDelegateRect::DwItemDelegateRect()
{
    DECLARE_ADD_SET(color);
    DECLARE_ADD_GET(color);
    DECLARE_ADD_CALL(setColor);
}