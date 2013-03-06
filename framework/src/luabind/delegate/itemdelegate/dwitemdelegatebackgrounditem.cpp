#include "stable.h"
#include "dwitemdelegatebackgrounditem.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwgraphicsbackgrounditem.h"

DW_IMPLEMENT_CLASS(DwItemDelegateBackgroundItem, 'IDBI', DwItemDelegate);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(pixmap)
{
    DwGraphicsBackgroundItem *back = dwsafe_cast<DwGraphicsBackgroundItem>(obj);
    DW_CHECK_ASSERT_RET(back, 0);

    back->setPixmap(_lua_pop_value_of<DwImage>(L, 3));
    return 0;
}

DECLARE_DELEGATE_SET(url)
{
    DwGraphicsBackgroundItem *back = dwsafe_cast<DwGraphicsBackgroundItem>(obj);
    DW_CHECK_ASSERT_RET(back, 0);

    back->setURL(_lua_pop_value_of<DwUrl>(L, 3));
    return 0;
}

DECLARE_DELEGATE_SET(idxOffset)
{
    DwGraphicsBackgroundItem *back = dwsafe_cast<DwGraphicsBackgroundItem>(obj);
    DW_CHECK_ASSERT_RET(back, 0);

    back->setIdxOffset(_lua_pop_value_of<int>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(isDrawCenter)
{
    DwGraphicsBackgroundItem *back = dwsafe_cast<DwGraphicsBackgroundItem>(obj);
    DW_CHECK_ASSERT_RET(back, 0);

    back->setDrawCenter(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(pixmap)
{
    DwGraphicsBackgroundItem *back = dwsafe_cast<DwGraphicsBackgroundItem>(obj);
    DW_CHECK_ASSERT_RET(back, 0);

    return _lua_push_value_of<DwImage>(L, back->pixmap());
}

DECLARE_DELEGATE_GET(url)
{
    DwGraphicsBackgroundItem *back = dwsafe_cast<DwGraphicsBackgroundItem>(obj);
    DW_CHECK_ASSERT_RET(back, 0);

    return _lua_push_value_of<DwUrl>(L, back->url());
}

DECLARE_DELEGATE_GET(idxOffset)
{
    DwGraphicsBackgroundItem *back = dwsafe_cast<DwGraphicsBackgroundItem>(obj);
    DW_CHECK_ASSERT_RET(back, 0);
    return _lua_push_value_of<int>(L, back->idxOffset());
}

DECLARE_DELEGATE_GET(isDrawCenter)
{
    DwGraphicsBackgroundItem *back = dwsafe_cast<DwGraphicsBackgroundItem>(obj);
    DW_CHECK_ASSERT_RET(back, 0);
    return _lua_push_value_of<bool>(L, back->isDrawCenter());
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(setPixmap)
{
    DwGraphicsBackgroundItem *back = dwsafe_cast<DwGraphicsBackgroundItem>(obj);
    DW_CHECK_ASSERT_RET(back, 0);
    back->setPixmap(_lua_pop_value_of<DwImage>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(setURL)
{
    DwGraphicsBackgroundItem *back = dwsafe_cast<DwGraphicsBackgroundItem>(obj);
    DW_CHECK_ASSERT_RET(back, 0);
    back->setURL(_lua_pop_value_of<DwUrl>(L, 3));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DwItemDelegateBackgroundItem::DwItemDelegateBackgroundItem()
{
    DECLARE_ADD_SET(pixmap);
    DECLARE_ADD_SET(url);
    DECLARE_ADD_SET(idxOffset);
    DECLARE_ADD_SET(isDrawCenter);

    DECLARE_ADD_GET(pixmap);
    DECLARE_ADD_GET(url);
    DECLARE_ADD_GET(idxOffset);
    DECLARE_ADD_GET(isDrawCenter);

    DECLARE_ADD_CALL(setPixmap);
    DECLARE_ADD_CALL(setURL);
}