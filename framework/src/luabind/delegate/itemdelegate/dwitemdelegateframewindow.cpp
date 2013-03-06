#include "stable.h"
#include "dwitemdelegateframewindow.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwframewindow.h"
#include "dwgui/dwgraphicsframeitem.h"
#include "dwgui/dwtitlebar.h"
#include "dwgui/dwgraphicsmarginspixmapitem.h"
#include "dwgui/dwgraphicsbackgrounditem.h"

DW_IMPLEMENT_CLASS(DwItemDelegateFrameWindow, 'IDFW', DwItemDelegateWidgetItem);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(backgroundColor)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setBackgroundColor(_lua_pop_value_of<DwColor>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(backgroundImage)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setBackgroundImage(_lua_pop_value_of<DwString>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(caption)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setCaption(_lua_pop_value_of<DwString>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(icon)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setWinIcon(_lua_pop_value_of<int>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(enabled)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setEnabled(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(pos)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setPos(_lua_pop_value_of<DwPointF>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(x)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setX(_lua_pop_value_of<DwReal>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(y)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setY(_lua_pop_value_of<DwReal>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(size)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setSize(_lua_pop_value_of<DwSizeF>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(width)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setWidth(_lua_pop_value_of<DwReal>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(height)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setHeight(_lua_pop_value_of<DwReal>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(rect)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setRect(_lua_pop_value_of<DwRectF>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(opacity)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setOpacity(_lua_pop_value_of<DwReal>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(centerWindowOnShow)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setCenterWindowOnShow(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(winStyle)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setWinStyle(_lua_pop_value_of<int>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(roundEliptic)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setRoundEliptic(_lua_pop_value_of<int>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(shadowEdage)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setShadowEdage(_lua_pop_value_of<int>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(resizeAble)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setAttribute(Dw::WA_ResizeAble, _lua_pop_value_of<bool>(L, -1));
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(backgroundColor)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<DwColor>(L, window->backgroundColor());
}

DECLARE_DELEGATE_GET(background)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<DwGraphicsItem*>(L, window->background());
}

DECLARE_DELEGATE_GET(border)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<DwGraphicsItem*>(L, window->border());
}

DECLARE_DELEGATE_GET(titleBar)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<DwGraphicsItem*>(L, window->titleBar());
}

DECLARE_DELEGATE_GET(caption)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<DwString>(L, window->caption());
}

DECLARE_DELEGATE_GET(icon)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<int>(L, window->icon());
}

DECLARE_DELEGATE_GET(enabled)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<bool>(L, window->isEnabled());
}

DECLARE_DELEGATE_GET(pos)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<DwPointF>(L, window->pos());
}

DECLARE_DELEGATE_GET(x)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<DwReal>(L, window->x());
}

DECLARE_DELEGATE_GET(y)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<DwReal>(L, window->y());
}

DECLARE_DELEGATE_GET(size)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<DwSizeF>(L, window->size());
}

DECLARE_DELEGATE_GET(width)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<DwReal>(L, window->width());
}

DECLARE_DELEGATE_GET(height)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<DwReal>(L, window->height());
}

DECLARE_DELEGATE_GET(rect)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<DwRectF>(L, window->rect());
}

DECLARE_DELEGATE_GET(opacity)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<DwReal>(L, window->opacity());
}

DECLARE_DELEGATE_GET(centerWindowOnShow)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<bool>(L, window->isCenterWindowOnShow());
}

DECLARE_DELEGATE_GET(winStyle)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<int>(L, window->winStyle());
}

DECLARE_DELEGATE_GET(roundEliptic)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<int>(L, window->roundEliptic());
}

DECLARE_DELEGATE_GET(shadowEdage)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<int>(L, window->shadowEdage());
}

DECLARE_DELEGATE_GET(backgrondLayer)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, window->backgrondLayer());
}

DECLARE_DELEGATE_GET(contentLayer)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, window->contentLayer());
}

DECLARE_DELEGATE_GET(popupLayer)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, window->popupLayer());
}

DECLARE_DELEGATE_GET(fullContentLayer)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, window->fullContentLayer());
}

DECLARE_DELEGATE_GET(contentContainerLayer)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, window->contentContainerLayer());
}

DECLARE_DELEGATE_GET(borderLayer)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, window->borderLayer());
}

DECLARE_DELEGATE_GET(resizeAble)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<bool>(L, window->isAttribute(Dw::WA_ResizeAble));
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(show)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    if (lua_gettop(L) >= 3)
    {
        window->show(Dw::ShowCommand(_lua_pop_value_of<int>(L, 3)));
    }
    else
    {
        window->show();
    }
    return 0;
}

DECLARE_DELEGATE_CALL(hide)
{
    DW_UNUSED(L);
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->hide();
    return 0;
}

DECLARE_DELEGATE_CALL(close)
{
    DW_UNUSED(L);
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->close();
    return 0;
}

DECLARE_DELEGATE_CALL(update)
{
    DW_UNUSED(L);
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->update();
    return 0;
}

DECLARE_DELEGATE_CALL(setAttribute)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setAttribute((Dw::WidgetAttribute)(_lua_pop_value_of<int>(L, 3)),
                         _lua_pop_value_of<bool>(L, 4));
    return 0;
}

DECLARE_DELEGATE_CALL(setRect)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setRect(_lua_pop_value_of<DwRect>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(setOpacity)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setOpacity(_lua_pop_value_of<DwReal>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(setRoundEliptic)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setRoundEliptic(_lua_pop_value_of<int>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(border)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<DwGraphicsItem*>(L, window->border());
}

DECLARE_DELEGATE_CALL(centerWindow)
{
    DW_UNUSED(L);
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->centerWindow();
    return 0;
}

DECLARE_DELEGATE_CALL(setEnabled)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setEnabled(_lua_pop_value_of<bool>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(setShadowEdage)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setShadowEdage(_lua_pop_value_of<int>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(backgrondLayer)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, window->backgrondLayer());
}

DECLARE_DELEGATE_CALL(contentLayer)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, window->contentLayer());
}

DECLARE_DELEGATE_CALL(popupLayer)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, window->popupLayer());
}

DECLARE_DELEGATE_CALL(fullContentLayer)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, window->fullContentLayer());
}

DECLARE_DELEGATE_CALL(contentContainerLayer)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, window->contentContainerLayer());
}

DECLARE_DELEGATE_CALL(borderLayer)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, window->borderLayer());
}

DECLARE_DELEGATE_CALL(setResizeAble)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(obj);
    DW_CHECK_ASSERT_RET(window, 0);
    window->setAttribute(Dw::WA_ResizeAble, _lua_pop_value_of<bool>(L, 3));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DwItemDelegateFrameWindow::DwItemDelegateFrameWindow()
{
    DECLARE_ADD_SET(backgroundColor);
    DECLARE_ADD_SET(backgroundImage);
    DECLARE_ADD_SET(caption);
    DECLARE_ADD_SET(icon);
    DECLARE_ADD_SET(enabled);
    DECLARE_ADD_SET(pos);
    DECLARE_ADD_SET(x);
    DECLARE_ADD_SET(y);
    DECLARE_ADD_SET(size);
    DECLARE_ADD_SET(width);
    DECLARE_ADD_SET(height);
    DECLARE_ADD_SET(rect);
    DECLARE_ADD_SET(opacity);
    DECLARE_ADD_SET(centerWindowOnShow);
    DECLARE_ADD_SET(winStyle);
    DECLARE_ADD_SET(roundEliptic);
    DECLARE_ADD_SET(shadowEdage);
    DECLARE_ADD_SET(resizeAble);

    DECLARE_ADD_GET(backgroundColor);
    DECLARE_ADD_GET(background);
    DECLARE_ADD_GET(titleBar);
    DECLARE_ADD_GET(caption);
    DECLARE_ADD_GET(icon);
    DECLARE_ADD_GET(enabled);
    DECLARE_ADD_GET(pos);
    DECLARE_ADD_GET(x);
    DECLARE_ADD_GET(y);
    DECLARE_ADD_GET(size);
    DECLARE_ADD_GET(width);
    DECLARE_ADD_GET(height);
    DECLARE_ADD_GET(rect);
    DECLARE_ADD_GET(opacity);
    DECLARE_ADD_GET(centerWindowOnShow);
    DECLARE_ADD_GET(winStyle);
    DECLARE_ADD_GET(roundEliptic);
    DECLARE_ADD_GET(border);
    DECLARE_ADD_GET(shadowEdage);
    DECLARE_ADD_GET(backgrondLayer);
    DECLARE_ADD_GET(contentLayer);
    DECLARE_ADD_GET(popupLayer);
    DECLARE_ADD_GET(fullContentLayer);
    DECLARE_ADD_GET(contentContainerLayer);
    DECLARE_ADD_GET(borderLayer);
    DECLARE_ADD_GET(resizeAble);

    DECLARE_ADD_CALL(show);
    DECLARE_ADD_CALL(hide);
    DECLARE_ADD_CALL(close);
    DECLARE_ADD_CALL(update);
    DECLARE_ADD_CALL(setAttribute);
    DECLARE_ADD_CALL(setRect);
    DECLARE_ADD_CALL(setOpacity);
    DECLARE_ADD_CALL(setRoundEliptic);
    DECLARE_ADD_CALL(border);
    DECLARE_ADD_CALL(centerWindow);
    DECLARE_ADD_CALL(setEnabled);
    DECLARE_ADD_CALL(setShadowEdage);
    DECLARE_ADD_CALL(backgrondLayer);
    DECLARE_ADD_CALL(contentLayer);
    DECLARE_ADD_CALL(popupLayer);
    DECLARE_ADD_CALL(fullContentLayer);
    DECLARE_ADD_CALL(contentContainerLayer);
    DECLARE_ADD_CALL(borderLayer);
    DECLARE_ADD_CALL(setResizeAble);
}