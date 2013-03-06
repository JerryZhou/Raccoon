#include "stable.h"
#include "dwitemdelegatepixmap.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwgraphicspixmapitem.h"

DW_IMPLEMENT_CLASS(DwItemDelegatePixmap, 'IDPP', DwItemDelegate);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(pixmap)
{
    DwGraphicsPixmapItem *pixmap = dwsafe_cast<DwGraphicsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(pixmap, 0);

    pixmap->setPixmap(_lua_pop_value_of<DwImage>(L, 3));
    return 0;
}

DECLARE_DELEGATE_SET(url)
{
    DwGraphicsPixmapItem *pixmap = dwsafe_cast<DwGraphicsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(pixmap, 0);

    pixmap->setPixmap(_lua_pop_value_of<DwUrl>(L, 3));
    return 0;
}

DECLARE_DELEGATE_SET(offset)
{
    DwGraphicsPixmapItem *pixmap = dwsafe_cast<DwGraphicsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(pixmap, 0);

    pixmap->setOffset(_lua_pop_value_of<DwPointF>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(offsetX)
{
    DwGraphicsPixmapItem *pixmap = dwsafe_cast<DwGraphicsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(pixmap, 0);

    DwPointF poffset = pixmap->offset();
    poffset.setX(_lua_pop_value_of<double>(L, -1));
    pixmap->setOffset(poffset);
    return 0;
}

DECLARE_DELEGATE_SET(offsetY)
{
    DwGraphicsPixmapItem *pixmap = dwsafe_cast<DwGraphicsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(pixmap, 0);

    DwPointF poffset = pixmap->offset();
    poffset.setY(_lua_pop_value_of<double>(L, -1));
    pixmap->setOffset(poffset);
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(pixmap)
{
    DwGraphicsPixmapItem *pixmap = dwsafe_cast<DwGraphicsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(pixmap, 0);
    return _lua_push_value_of<DwImage>(L, pixmap->pixmap());
}

DECLARE_DELEGATE_GET(url)
{
    DwGraphicsPixmapItem *pixmap = dwsafe_cast<DwGraphicsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(pixmap, 0);
    return _lua_push_value_of<DwUrl>(L, DwUrl(pixmap->pixmap().path()));
}

DECLARE_DELEGATE_GET(offset)
{
    DwGraphicsPixmapItem *pixmap = dwsafe_cast<DwGraphicsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(pixmap, 0);
    return _lua_push_value_of<DwPointF>(L, pixmap->offset());
}

DECLARE_DELEGATE_GET(offsetX)
{
    DwGraphicsPixmapItem *pixmap = dwsafe_cast<DwGraphicsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(pixmap, 0);
    return _lua_push_value_of<double>(L, pixmap->offset().x());
}

DECLARE_DELEGATE_GET(offsetY)
{
    DwGraphicsPixmapItem *pixmap = dwsafe_cast<DwGraphicsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(pixmap, 0);
    return _lua_push_value_of<double>(L, pixmap->offset().y());
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(setPixmap)
{
    DwGraphicsPixmapItem *pixmap = dwsafe_cast<DwGraphicsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(pixmap, 0);
    pixmap->setPixmap(_lua_pop_value_of<DwImage>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(setURL)
{
    DwGraphicsPixmapItem *pixmap = dwsafe_cast<DwGraphicsPixmapItem>(obj);
    DW_CHECK_ASSERT_RET(pixmap, 0);
    pixmap->setPixmap(_lua_pop_value_of<DwUrl>(L, 3));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DwItemDelegatePixmap::DwItemDelegatePixmap()
{
    DECLARE_ADD_SET(pixmap);
    DECLARE_ADD_SET(url);
    DECLARE_ADD_SET(offset);
    DECLARE_ADD_SET(offsetX);
    DECLARE_ADD_SET(offsetY);

    DECLARE_ADD_GET(pixmap);
    DECLARE_ADD_GET(url);
    DECLARE_ADD_GET(offset);
    DECLARE_ADD_GET(offsetX);
    DECLARE_ADD_GET(offsetY);

    DECLARE_ADD_CALL(setPixmap);
    DECLARE_ADD_CALL(setURL);
}