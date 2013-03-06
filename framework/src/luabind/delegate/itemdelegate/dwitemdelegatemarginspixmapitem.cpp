#include "stable.h"
#include "dwitemdelegatemarginspixmapitem.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwgraphicsmarginspixmapitem.h"

DW_IMPLEMENT_CLASS(DwItemDelegateMarginsPixmapItem, 'DMPI', DwItemDelegate);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(pixmap)
{
    DwGraphicsMarginsPixmapItem *marginsPixmapItem = dwsafe_cast<DwGraphicsMarginsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(marginsPixmapItem, 0);
    marginsPixmapItem->setPixmap(_lua_pop_value_of<DwImage>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(margins)
{
    DwGraphicsMarginsPixmapItem *marginsPixmapItem = dwsafe_cast<DwGraphicsMarginsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(marginsPixmapItem, 0);
    marginsPixmapItem->setMargins(_lua_pop_value_of<DwMargins>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(isDrawCenter)
{
    DwGraphicsMarginsPixmapItem *marginsPixmapItem = dwsafe_cast<DwGraphicsMarginsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(marginsPixmapItem, 0);
    marginsPixmapItem->setDrawCenter(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(pixmap)
{
    DwGraphicsMarginsPixmapItem *marginsPixmapItem = dwsafe_cast<DwGraphicsMarginsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(marginsPixmapItem, 0);
    return _lua_push_value_of<DwImage>(L, marginsPixmapItem->pixmap());
}

DECLARE_DELEGATE_GET(margins)
{
    DwGraphicsMarginsPixmapItem *marginsPixmapItem = dwsafe_cast<DwGraphicsMarginsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(marginsPixmapItem, 0);
    return _lua_push_value_of<DwMargins>(L, marginsPixmapItem->margins());
}

DECLARE_DELEGATE_GET(isDrawCenter)
{
    DwGraphicsMarginsPixmapItem *marginsPixmapItem = dwsafe_cast<DwGraphicsMarginsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(marginsPixmapItem, 0);
    return _lua_push_value_of<bool>(L, marginsPixmapItem->isDrawCenter());
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(setPixmap)
{
    DwGraphicsMarginsPixmapItem *marginsPixmapItem = dwsafe_cast<DwGraphicsMarginsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(marginsPixmapItem, 0);
    marginsPixmapItem->setPixmap((_lua_pop_value_of<DwImage>(L, 3)));
    return 0;
}

DECLARE_DELEGATE_CALL(pixmap)
{
    DwGraphicsMarginsPixmapItem *marginsPixmapItem = dwsafe_cast<DwGraphicsMarginsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(marginsPixmapItem, 0);
    return _lua_push_value_of<DwImage>(L, marginsPixmapItem->pixmap());
}

DECLARE_DELEGATE_CALL(setMargins)
{
    DwGraphicsMarginsPixmapItem *marginsPixmapItem = dwsafe_cast<DwGraphicsMarginsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(marginsPixmapItem, 0);
    marginsPixmapItem->setMargins(_lua_pop_value_of<DwMargins>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(margins)
{
    DwGraphicsMarginsPixmapItem *marginsPixmapItem = dwsafe_cast<DwGraphicsMarginsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(marginsPixmapItem, 0);
    return _lua_push_value_of<DwMargins>(L, marginsPixmapItem->margins());
}

DECLARE_DELEGATE_CALL(setDrawCenter)
{
    DwGraphicsMarginsPixmapItem *marginsPixmapItem = dwsafe_cast<DwGraphicsMarginsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(marginsPixmapItem, 0);
    marginsPixmapItem->setDrawCenter(_lua_pop_value_of<bool>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(isDrawCenter)
{
    DwGraphicsMarginsPixmapItem *marginsPixmapItem = dwsafe_cast<DwGraphicsMarginsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(marginsPixmapItem, 0);
    return _lua_push_value_of<DwMargins>(L, marginsPixmapItem->margins());
}

//------------------------------------------------------------------------------
/**
*/
DwItemDelegateMarginsPixmapItem::DwItemDelegateMarginsPixmapItem()
{
    DECLARE_ADD_SET(pixmap);
    DECLARE_ADD_SET(margins);
    DECLARE_ADD_SET(isDrawCenter);

    DECLARE_ADD_GET(pixmap);
    DECLARE_ADD_GET(margins);
    DECLARE_ADD_GET(isDrawCenter);

    DECLARE_ADD_CALL(setPixmap);
    DECLARE_ADD_CALL(pixmap);
    DECLARE_ADD_CALL(setMargins);
    DECLARE_ADD_CALL(margins);
    DECLARE_ADD_CALL(setDrawCenter);
    DECLARE_ADD_CALL(isDrawCenter);
}