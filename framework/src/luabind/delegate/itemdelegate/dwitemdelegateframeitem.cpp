#include "stable.h"
#include "dwitemdelegateframeitem.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwgraphicsframeitem.h"

DW_IMPLEMENT_CLASS(DwItemDelegateFrameItem, 'IDFI', DwItemDelegate);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(isDisable)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    frameItem->setDisable(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(isHover)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    frameItem->setHover(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(isPressedDown)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    frameItem->setPressedDown(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(isFocus)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    frameItem->setFocus(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(margins)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    frameItem->setMargins(_lua_pop_value_of<DwMargins>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(mask)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    frameItem->setMask(_lua_pop_value_of<DwString>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(backgroundColor)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    frameItem->setBackgroundColor(_lua_pop_value_of<DwColor>(L, -1));
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(isDisable)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    return _lua_push_value_of<bool>(L, frameItem->isDisable());
}

DECLARE_DELEGATE_GET(isHover)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    return _lua_push_value_of<bool>(L, frameItem->isHover());
}

DECLARE_DELEGATE_GET(isPressedDown)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    return _lua_push_value_of<bool>(L, frameItem->isPressedDown());
}

DECLARE_DELEGATE_GET(isFocus)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    return _lua_push_value_of<bool>(L, frameItem->isFocus());
}

DECLARE_DELEGATE_GET(margins)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    return _lua_push_value_of<DwMargins>(L, frameItem->margins());
}

DECLARE_DELEGATE_GET(mask)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    return _lua_push_value_of<DwImage>(L, frameItem->mask());
}

DECLARE_DELEGATE_GET(backgroundColor)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    return _lua_push_value_of<DwColor>(L, frameItem->backgroundColor());
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(setImage)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    frameItem->setImage((DwWidget::FrameState)(_lua_pop_value_of<int>(L, 3)),
                        (_lua_pop_value_of<DwImage>(L, 4)));
    return 0;
}

DECLARE_DELEGATE_CALL(imageOf)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    DwImage img = frameItem->imageOf((DwWidget::FrameState)(_lua_pop_value_of<int>(L, 3)));
    return _lua_push_value_of<DwImage>(L, img);
}

DECLARE_DELEGATE_CALL(setMask)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    frameItem->setMask((DwString)(_lua_pop_value_of<DwString>(L, 3)));
    return 0;
}

DECLARE_DELEGATE_CALL(mask)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    return _lua_push_value_of<DwImage>(L, frameItem->mask());
}

DECLARE_DELEGATE_CALL(setMargins)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    frameItem->setMargins(_lua_pop_value_of<DwMargins>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(margins)
{
    DwGraphicsFrameItem *frameItem = dwsafe_cast<DwGraphicsFrameItem>(obj);
    DW_CHECK_ASSERT_RET(frameItem, 0);
    return _lua_push_value_of<DwMargins>(L, frameItem->margins());
}

//------------------------------------------------------------------------------
/**
*/
DwItemDelegateFrameItem::DwItemDelegateFrameItem()
{
    DECLARE_ADD_SET(isDisable);
    DECLARE_ADD_SET(isHover);
    DECLARE_ADD_SET(isPressedDown);
    DECLARE_ADD_SET(margins);
    DECLARE_ADD_SET(mask);
    DECLARE_ADD_SET(backgroundColor);

    DECLARE_ADD_GET(isDisable);
    DECLARE_ADD_GET(isHover);
    DECLARE_ADD_GET(isPressedDown);
    DECLARE_ADD_GET(isFocus);
    DECLARE_ADD_GET(margins);
    DECLARE_ADD_GET(mask);
    DECLARE_ADD_GET(backgroundColor);

    DECLARE_ADD_CALL(setImage);
    DECLARE_ADD_CALL(imageOf);
    DECLARE_ADD_CALL(setMask);
    DECLARE_ADD_CALL(mask);
    DECLARE_ADD_CALL(setMargins);
    DECLARE_ADD_CALL(margins);
}