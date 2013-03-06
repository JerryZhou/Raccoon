#include "stable.h"
#include "dwitemdelegate.h"
#include "dwitemdelegate_p.h"
#include "export/dwexportutil_p.h"
#include "dwgui/dwgraphicseventhandler.h"

DW_IMPLEMENT_CLASS(DwItemDelegate, 'IDTE', DwDelegate);

// Set
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(x)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setX(_lua_pop_value_of<double>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(y)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setY(_lua_pop_value_of<double>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(rect)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setRect(_lua_pop_value_of<DwRectF>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(name)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setName(_lua_pop_value_of<DwString>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(width)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setWidth(_lua_pop_value_of<double>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(height)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setHeight(_lua_pop_value_of<double>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(size)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setSize(_lua_pop_value_of<DwSizeF>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(boundingRect)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setBoundingRect(_lua_pop_value_of<DwRectF>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(clipRect)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setClipRect(_lua_pop_value_of<DwRectF>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(opacity)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setOpacity(_lua_pop_value_of<double>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(parent)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    DwGraphicsItem * p = _lua_itemOf(L, -1);
    item->setParentItem(p);
    return 0;
}

DECLARE_DELEGATE_SET(focusPolicy)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setFocusPolicy(Dw::FocusPolicy(_lua_pop_value_of<int>(L, -1)));
    return 0;
}

DECLARE_DELEGATE_SET(visible)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setVisible(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(enabled)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setEnabled(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(pos)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setPos(_lua_pop_value_of<DwPointF>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(z)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setZValue(_lua_pop_value_of<double>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(movable)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setMovable(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(selectable)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setSelectable(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(focusable)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setFocusable(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(hoverable)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setHoverable(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(acceptInputMethod)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setAcceptInputMethod(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(acceptDrops)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setAcceptDrops(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(acceptedMouseButtons)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setAcceptedMouseButtons(_lua_pop_value_of<int>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(toolTips)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setToolTips(_lua_pop_value_of<DwString>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(eventHandler)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    bool isClear = true;
    DwString s = DwString("Dw") + _lua_pop_value_of<DwString>(L, -1);
    if (dwFactory()->classExists(s))
    {
        const DwRtti * h = dwFactory()->classRttiOf(s);
        if (h && h->isDerivedFrom(DwItemEventHandler::RTTI()))
        {
            if (!item->eventHandler() || item->eventHandler()->rtti() != h)
            {
                DwRttiObject *obj = h->create();
                DwItemEventHandler *handler = dwsafe_cast<DwItemEventHandler>(obj);
                handler->setItem(item);

                item->setEventHandler(handler);
            }

            if (item->eventHandler() && item->eventHandler()->rtti() == h)
            {
                isClear = false;
            }
        }
    }

    if (isClear)
    {
        item->setEventHandler(NULL);
    }
    return 0;
}

DECLARE_DELEGATE_SET(translate)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    DW_ASSERT(lua_gettop(L) >= 2);
    DwPointF p = _lua_pop_value_of<DwPointF>(L, -1);
    DwTransform transform;
    transform.translate(p.x() , p.y());
    item->setTransform(transform, true);
    return 0;
}

DECLARE_DELEGATE_SET(scale)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    DW_ASSERT(lua_gettop(L) >= 2);
    DwPointF p = _lua_pop_value_of<DwPointF>(L, -1);
    DwTransform transform;
    transform.scale(p.x(), p.y());
    item->setTransform(transform, true);
    return 0;
}

DECLARE_DELEGATE_SET(rotate)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    DW_ASSERT(lua_gettop(L) >= 2);
    DwTransform transform;
    transform.rotate(_lua_pop_value_of<DwReal>(L, -1));
    item->setTransform(transform, true);
    return 0;
}

DECLARE_DELEGATE_SET(shear)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    DW_ASSERT(lua_gettop(L) >= 2);
    DwPointF p = _lua_pop_value_of<DwPointF>(L, -1);
    DwTransform transform;
    transform.shear(p.x(), p.y());
    item->setTransform(transform, true);
    return 0;
}

DECLARE_DELEGATE_SET(baselineOffset)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setBaselineOffset(_lua_pop_value_of<DwReal>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(sendChangedEvent)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setSendChangedEvent(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(focusNext)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->setFocusNext(_lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(focusPrev)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->setFocusPre(_lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L, -1));
    return 0;
}

// Get
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(x)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<double>(L, item->x());
}

DECLARE_DELEGATE_GET(y)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<double>(L, item->y());
}

DECLARE_DELEGATE_GET(rect)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<DwRectF>(L, item->rect());
}

DECLARE_DELEGATE_GET(name)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<DwString>(L, item->name());
}

DECLARE_DELEGATE_GET(width)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<double>(L, item->width());
}

DECLARE_DELEGATE_GET(height)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<double>(L, item->height());
}

DECLARE_DELEGATE_GET(size)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<DwSizeF>(L, item->size());
}

DECLARE_DELEGATE_GET(boundingRect)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<DwRectF>(L, item->boundingRect());
}

DECLARE_DELEGATE_GET(clipRect)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<DwRectF>(L, item->clipRect());
}

DECLARE_DELEGATE_GET(opacity)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<double>(L, item->opacity());
}

DECLARE_DELEGATE_GET(parent)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, item->parentItem());
}

DECLARE_DELEGATE_GET(focusPolicy)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<int>(L, (int)(item->focusPolicy()));
}

DECLARE_DELEGATE_GET(visible)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<bool>(L, item->isVisible());
}

DECLARE_DELEGATE_GET(enabled)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<bool>(L, item->isEnabled());
}

DECLARE_DELEGATE_GET(pos)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<DwPointF>(L, item->pos());
}

DECLARE_DELEGATE_GET(z)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<double>(L, item->zValue());
}

DECLARE_DELEGATE_GET(movable)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<bool>(L, item->isMovable());
}

DECLARE_DELEGATE_GET(selectable)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<bool>(L, item->isSelectable());
}

DECLARE_DELEGATE_GET(focusable)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<bool>(L, item->isFocusable());
}

DECLARE_DELEGATE_GET(hoverable)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<bool>(L, item->isHoverable());
}

DECLARE_DELEGATE_GET(acceptInputMethod)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<bool>(L, item->isAcceptInputMethod());
}

DECLARE_DELEGATE_GET(acceptDrops)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<bool>(L, item->isAcceptDrops());
}

DECLARE_DELEGATE_GET(acceptedMouseButtons)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<int>(L, item->acceptedMouseButtons());
}

DECLARE_DELEGATE_GET(toolTips)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<DwString>(L, item->toolTips());
}

DECLARE_DELEGATE_GET(eventHandler)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    DwString s("null");
    if (item->eventHandler())
    {
        s = item->eventHandler()->rtti()->name();
        s = s.right(s.length() - 2);

    }
    return _lua_push_value_of<DwString>(L, s);
}

DECLARE_DELEGATE_GET(topLevelItem)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<DwGraphicsItem*>(L, item->topLevelItem());
}

DECLARE_DELEGATE_GET(translate)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<DwPointF>(L, item->transform().translated());
}

DECLARE_DELEGATE_GET(scale)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<DwPointF>(L, item->transform().scaled());
}

DECLARE_DELEGATE_GET(shear)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<DwPointF>(L, item->transform().shearded());
}

DECLARE_DELEGATE_GET(baselineOffset)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<DwReal>(L, item->baselineOffset());
}

DECLARE_DELEGATE_GET(rtti)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<LUA_RTTI_PTR>(L, item->rtti());
}

DECLARE_DELEGATE_GET(rttiName)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<DwString>(L, item->rtti()->name());
}

DECLARE_DELEGATE_GET(sendChangedEvent)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<bool>(L, item->sendChangedEvent());
}

DECLARE_DELEGATE_GET(focusNext)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, item->focusNext());
}

DECLARE_DELEGATE_GET(focusPrev)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, item->focusPrev());
}

DECLARE_DELEGATE_GET(childCount)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<int>(L, item->children().size());
}
//------------------------------------------------------------------------------
/**
    (item, k, args)
    (1, 2, 3, ....)
*/
// CALL
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(setBoundingRect)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setBoundingRect(_lua_pop_value_of<DwRectF>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(moveBy)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->moveBy(_lua_pop_value_of<DwReal>(L, 3),
                 _lua_pop_value_of<DwReal>(L, 4));
    return 0;
}

DECLARE_DELEGATE_CALL(setFlag)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setFlag((Dw::GraphicsItemFlag)(_lua_pop_value_of<int>(L, 3)),
                  _lua_pop_value_of<bool>(L, 4));
    return 0;
}

DECLARE_DELEGATE_CALL(setAcceptedMouseButton)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setAcceptedMouseButton((Dw::MouseButton)(_lua_pop_value_of<int>(L, 3)),
                                 _lua_pop_value_of<bool>(L, 4));
    return 0;
}

DECLARE_DELEGATE_CALL(findChild)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    DwGraphicsItem *p = item->findChild(_lua_pop_value_of<DwString>(L, 3));
    return _lua_push_value_of<DwGraphicsItem *>(L, p);
}

DECLARE_DELEGATE_CALL(deleteLater)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->deleteLater();
    return 0;
}

DECLARE_DELEGATE_CALL(clearAnchors)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->clearAnchors();
    return 0;
}

DECLARE_DELEGATE_CALL(clearAnimations)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->clearAnimations();
    return 0;
}

DECLARE_DELEGATE_CALL(anchors)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<LUA_ANCHOR_PTR>(L, item->anchors());
}

DECLARE_DELEGATE_CALL(setVisible)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setVisible(_lua_pop_value_of<bool>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(setName)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setName(_lua_pop_value_of<DwString>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(hasFocus)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    return _lua_push_value_of<bool>(L, item->hasFocus());
}

DECLARE_DELEGATE_CALL(clearFocus)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->clearFocus();
    return 0;
}

DECLARE_DELEGATE_CALL(setFocus)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setFocus();
    return 0;
}

DECLARE_DELEGATE_CALL(setFocusPolicy)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setFocusPolicy((Dw::FocusPolicy)(_lua_pop_value_of<int>(L, 3)));
    return 0;
}

DECLARE_DELEGATE_CALL(mapToParent)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    int n = 0;
    DwVariant v  = _lua_pop_value_of<DwVariant>(L, 3);
    switch (v.vtype())
    {
    case DwVariant::Point:
    case DwVariant::PointF:
        n = _lua_push_value_of<DwPointF>(L, item->mapToParent(v.as<DwPointF>()));
        break;
    case DwVariant::Rect:
    case DwVariant::RectF:
        n = _lua_push_value_of<DwRectF>(L, item->mapToParent(v.as<DwRectF>()));
        break;
    default:
        DW_ASSERT(false);
        DW_UI_LOGER() << "graphics item mapToParent error type" << "\n";
    }

    return n;
}

DECLARE_DELEGATE_CALL(mapToScene)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    int n = 0;
    DwVariant v  = _lua_pop_value_of<DwVariant>(L, 3);
    switch (v.vtype())
    {
    case DwVariant::Point:
    case DwVariant::PointF:
        n = _lua_push_value_of<DwPointF>(L, item->mapToScene(v.as<DwPointF>()));
        break;
    case DwVariant::Rect:
    case DwVariant::RectF:
        n = _lua_push_value_of<DwRectF>(L, item->mapToScene(v.as<DwRectF>()));
        break;
    default:
        DW_ASSERT(false);
        DW_UI_LOGER() << "graphics item mapToParent error type" << "\n";
    }

    return n;
}

DECLARE_DELEGATE_CALL(isAncestorOf)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    DwGraphicsItem *v  = _lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L, 3);
    return _lua_push_value_of<bool>(L, item->isAncestorOf(v));
}

DECLARE_DELEGATE_CALL(commonAncestorItem)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    DwGraphicsItem *v  = _lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L, 3);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, item->commonAncestorItem(v));
}

DECLARE_DELEGATE_CALL(invalidateDepthRecursively)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->invalidateDepthRecursively();
    return 0;
}

DECLARE_DELEGATE_CALL(resolveDepth)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->resolveDepth();
    return 0;
}

DECLARE_DELEGATE_CALL(childrenBoundingRect)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<DwRectF>(L, item->childrenBoundingRect());
}

DECLARE_DELEGATE_CALL(sceneBoundingRect)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<DwRectF>(L, item->sceneBoundingRect());
}

DECLARE_DELEGATE_CALL(sceneEffectiveBoundingRect)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<DwRectF>(L, item->sceneEffectiveBoundingRect());
}

DECLARE_DELEGATE_CALL(isVisibleTo)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    DwGraphicsItem *v  = _lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L, 3);
    return _lua_push_value_of<bool>(L, item->isVisibleTo(v));
}

DECLARE_DELEGATE_CALL(hide)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->hide();
    return 0;
}

DECLARE_DELEGATE_CALL(show)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->show();
    return 0;
}

DECLARE_DELEGATE_CALL(stackBefore)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    DwGraphicsItem *v  = _lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L, 3);
    item->stackBefore(v);
    return 0;
}

DECLARE_DELEGATE_CALL(baselineOffset)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<DwReal>(L, item->baselineOffset());
}

DECLARE_DELEGATE_CALL(setBaselineOffset)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->setBaselineOffset(_lua_pop_value_of<DwReal>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(update)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    if(lua_gettop(L) >= 3)
    {
        item->update(_lua_pop_value_of<DwRectF>(L, 3));
    }
    else
    {
        item->update();
    }

    return 0;
}

DECLARE_DELEGATE_CALL(focusProxy)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, item->focusProxy());
}

DECLARE_DELEGATE_CALL(setFocusProxy)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->setFocusProxy(_lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(resetFocusProxy)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->resetFocusProxy();
    return 0;
}

DECLARE_DELEGATE_CALL(focusNext)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, item->focusNext());
}

DECLARE_DELEGATE_CALL(setFocusNext)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->setFocusNext(_lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L,3));
    return 0;
}

DECLARE_DELEGATE_CALL(focusPrev)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, item->focusPrev());
}


DECLARE_DELEGATE_CALL(setFocusPre)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->setFocusPre(_lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L,3));
    return 0;
}

DECLARE_DELEGATE_CALL(removeFromFocusLinkList)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->removeFromFocusLinkList();
    return 0;
}

DECLARE_DELEGATE_CALL(grabMouse)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->grabMouse();
    return 0;
}

DECLARE_DELEGATE_CALL(ungrabMouse)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->ungrabMouse();
    return 0;
}

DECLARE_DELEGATE_CALL(grabKeyboard)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->grabKeyboard();
    return 0;
}

DECLARE_DELEGATE_CALL(ungrabKeyboard)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->ungrabKeyboard();
    return 0;
}

DECLARE_DELEGATE_CALL(globalStackingOrder)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<int>(L, item->globalStackingOrder());
}

DECLARE_DELEGATE_CALL(index)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<int>(L, item->index());
}

DECLARE_DELEGATE_CALL(debugString)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->debugString(DwDebug());
    return 0;
}

DECLARE_DELEGATE_CALL(addAnimation)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    DwAbstractAnimation * anim = _lua_pop_value_of<LUA_ANIMATION_PTR>(L, 3);

    if (anim)
    {
        item->addAnimation(anim);
    }

    return 0;
}

DECLARE_DELEGATE_CALL(removeAnimation)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    DwAbstractAnimation * anim = _lua_pop_value_of<LUA_ANIMATION_PTR>(L, 3);

    if (anim)
    {
        item->removeAnimation(anim);
    }

    return 0;
}

DECLARE_DELEGATE_CALL(hasAnimation)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    DwAbstractAnimation * anim = _lua_pop_value_of<LUA_ANIMATION_PTR>(L, 3);

    bool has = false;
    if (anim)
    {
        has = item->hasAnimation(anim);
    }

    return _lua_push_value_of<bool>(L, has);
}

DECLARE_DELEGATE_CALL(animationNum)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<int>(L, item->animationNum());
}

DECLARE_DELEGATE_CALL(animationAt)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    int idx = _lua_pop_value_of<int>(L, 3);
    DwAbstractAnimation * anim = NULL;
    if (idx >=0 && idx < item->animationNum())
    {
        anim = item->animationAt(idx);
    }

    return _lua_push_value_of<LUA_ANIMATION_PTR>(L, anim);
}

DECLARE_DELEGATE_CALL(animationOf)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    uint32_t uidx = _lua_pop_value_of<uint32_t>(L, 3);
    DwAbstractAnimation * anim = item->animationOf(uidx);

    return _lua_push_value_of<LUA_ANIMATION_PTR>(L, anim);
}

DECLARE_DELEGATE_CALL(removeAllAnimation)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    item->removeAllAnimation();
    return 0;
}

DECLARE_DELEGATE_CALL(rtti)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<LUA_RTTI_PTR>(L, item->rtti());
}

DECLARE_DELEGATE_CALL(rttiName)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);

    return _lua_push_value_of<DwString>(L, item->rtti()->name());
}

DECLARE_DELEGATE_CALL(isA)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    DwRtti *info = _lua_pop_value_of<LUA_RTTI_PTR>(L, 3);

    return _lua_push_value_of<bool>(L, item->isA(info));
}

DECLARE_DELEGATE_CALL(isAccurateA)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    DwRtti *info = _lua_pop_value_of<LUA_RTTI_PTR>(L, 3);

    return _lua_push_value_of<bool>(L, item->rtti() == info);
}

DECLARE_DELEGATE_CALL(setRect)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setRect(_lua_pop_value_of<DwRectF>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(setSize)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setSize(_lua_pop_value_of<DwSizeF>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(setSendChangedEvent)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setSendChangedEvent(_lua_pop_value_of<bool>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(setToolTips)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    item->setToolTips(_lua_pop_value_of<DwString>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(isNull)
{
    DW_UNUSED(obj);
    return _lua_push_value_of<bool>(L, false);
}

DECLARE_DELEGATE_CALL(isValid)
{
    DW_UNUSED(obj);
    return _lua_push_value_of<bool>(L, true);
}

DECLARE_DELEGATE_CALL(childAt)
{
    DW_UNUSED(obj);
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    int idx = _lua_pop_value_of<int>(L, 3);

    if (idx < item->children().size())
    {
        return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, item->children().at(idx));
    }
    else
    {
        return 0;
    }
}

DECLARE_DELEGATE_CALL(acquireImage)
{
    DW_UNUSED(obj);
    return _lua_push_value_of<DwImage>(L, DW_PIXMAP_ASYNC(_lua_pop_value_of<DwUrl>(L, 3), obj));
}

DECLARE_DELEGATE_CALL(acquireData)
{
    DW_UNUSED(obj);
    return _lua_push_value_of<DwByteArray>(L, DW_DATA_ASYNC(
        _lua_pop_value_of<DwUrl>(L, 3), obj,
        _lua_pop_value_of<bool>(L, 4)));
}

//------------------------------------------------------------------------------
/**
*/
DwItemDelegate::DwItemDelegate()
{
    // SET
    DECLARE_ADD_SET(x);
    DECLARE_ADD_SET(y);
    DECLARE_ADD_SET(rect);
    DECLARE_ADD_SET(name);
    DECLARE_ADD_SET(width);
    DECLARE_ADD_SET(height);
    DECLARE_ADD_SET(size);
    DECLARE_ADD_SET(boundingRect);
    DECLARE_ADD_SET(clipRect);
    DECLARE_ADD_SET(opacity);
    DECLARE_ADD_SET(parent);
    DECLARE_ADD_SET(focusPolicy);
    DECLARE_ADD_SET(visible);
    DECLARE_ADD_SET(enabled);
    DECLARE_ADD_SET(pos);
    DECLARE_ADD_SET(z);
    DECLARE_ADD_SET(movable);
    DECLARE_ADD_SET(selectable);
    DECLARE_ADD_SET(focusable);
    DECLARE_ADD_SET(hoverable);
    DECLARE_ADD_SET(acceptInputMethod);
    DECLARE_ADD_SET(acceptDrops);
    DECLARE_ADD_SET(acceptedMouseButtons);
    DECLARE_ADD_SET(toolTips);
    DECLARE_ADD_SET(eventHandler);
    DECLARE_ADD_SET(translate);
    DECLARE_ADD_SET(scale);
    DECLARE_ADD_SET(rotate);
    DECLARE_ADD_SET(shear);
    DECLARE_ADD_SET(baselineOffset);
    DECLARE_ADD_SET(sendChangedEvent);
    DECLARE_ADD_SET(focusNext);
    DECLARE_ADD_SET(focusPrev);

    // GET
    DECLARE_ADD_GET(x);
    DECLARE_ADD_GET(y);
    DECLARE_ADD_GET(rect);
    DECLARE_ADD_GET(name);
    DECLARE_ADD_GET(width);
    DECLARE_ADD_GET(height);
    DECLARE_ADD_GET(size);
    DECLARE_ADD_GET(boundingRect);
    DECLARE_ADD_GET(clipRect);
    DECLARE_ADD_GET(opacity);
    DECLARE_ADD_GET(parent);
    DECLARE_ADD_GET(focusPolicy);
    DECLARE_ADD_GET(visible);
    DECLARE_ADD_GET(enabled);
    DECLARE_ADD_GET(pos);
    DECLARE_ADD_GET(z);
    DECLARE_ADD_GET(movable);
    DECLARE_ADD_GET(selectable);
    DECLARE_ADD_GET(focusable);
    DECLARE_ADD_GET(hoverable);
    DECLARE_ADD_GET(acceptInputMethod);
    DECLARE_ADD_GET(acceptDrops);
    DECLARE_ADD_GET(acceptedMouseButtons);
    DECLARE_ADD_GET(toolTips);
    DECLARE_ADD_GET(eventHandler);
    DECLARE_ADD_GET(topLevelItem);
    DECLARE_ADD_GET(translate);
    DECLARE_ADD_GET(scale);
    DECLARE_ADD_GET(shear);
    DECLARE_ADD_GET(baselineOffset);
    DECLARE_ADD_GET(rtti);
    DECLARE_ADD_GET(rttiName);
    DECLARE_ADD_GET(sendChangedEvent);
    DECLARE_ADD_GET(focusNext);
    DECLARE_ADD_GET(focusPrev);
    DECLARE_ADD_GET(childCount);

    // CALL
    DECLARE_ADD_CALL(setBoundingRect);
    DECLARE_ADD_CALL(moveBy);
    DECLARE_ADD_CALL(setFlag);
    DECLARE_ADD_CALL(setAcceptedMouseButton);
    DECLARE_ADD_CALL(findChild);
    DECLARE_ADD_CALL(deleteLater);
    DECLARE_ADD_CALL(clearAnimations);
    DECLARE_ADD_CALL(clearAnchors);
    DECLARE_ADD_CALL(anchors);
    DECLARE_ADD_CALL(setVisible);
    DECLARE_ADD_CALL(setName);
    DECLARE_ADD_CALL(hasFocus);
    DECLARE_ADD_CALL(clearFocus);
    DECLARE_ADD_CALL(setFocus);
    DECLARE_ADD_CALL(setFocusPolicy);
    DECLARE_ADD_CALL(mapToParent);
    DECLARE_ADD_CALL(mapToScene);
    DECLARE_ADD_CALL(isAncestorOf);
    DECLARE_ADD_CALL(commonAncestorItem);
    DECLARE_ADD_CALL(invalidateDepthRecursively);
    DECLARE_ADD_CALL(childrenBoundingRect);
    DECLARE_ADD_CALL(sceneBoundingRect);
    DECLARE_ADD_CALL(sceneEffectiveBoundingRect);
    DECLARE_ADD_CALL(isVisibleTo);
    DECLARE_ADD_CALL(show);
    DECLARE_ADD_CALL(hide);
    DECLARE_ADD_CALL(stackBefore);
    DECLARE_ADD_CALL(baselineOffset);
    DECLARE_ADD_CALL(setBaselineOffset);
    DECLARE_ADD_CALL(update);
    DECLARE_ADD_CALL(focusProxy);
    DECLARE_ADD_CALL(setFocusProxy);
    DECLARE_ADD_CALL(resetFocusProxy);
    DECLARE_ADD_CALL(focusNext);
    DECLARE_ADD_CALL(focusPrev);
    DECLARE_ADD_CALL(setFocusNext);
    DECLARE_ADD_CALL(setFocusPre);
    DECLARE_ADD_CALL(removeFromFocusLinkList);
    DECLARE_ADD_CALL(grabMouse);
    DECLARE_ADD_CALL(ungrabMouse);
    DECLARE_ADD_CALL(grabKeyboard);
    DECLARE_ADD_CALL(ungrabKeyboard);
    DECLARE_ADD_CALL(debugString);
    DECLARE_ADD_CALL(addAnimation);
    DECLARE_ADD_CALL(removeAnimation);
    DECLARE_ADD_CALL(hasAnimation);
    DECLARE_ADD_CALL(animationNum);
    DECLARE_ADD_CALL(animationAt);
    DECLARE_ADD_CALL(animationOf);
    DECLARE_ADD_CALL(removeAllAnimation);
    DECLARE_ADD_CALL(rtti);
    DECLARE_ADD_CALL(rttiName);
    DECLARE_ADD_CALL(isA);
    DECLARE_ADD_CALL(isAccurateA);
    DECLARE_ADD_CALL(setRect);
    DECLARE_ADD_CALL(setSize);
    DECLARE_ADD_CALL(setSendChangedEvent);
    DECLARE_ADD_CALL(setToolTips);
    DECLARE_ADD_CALL(isNull);
    DECLARE_ADD_CALL(isValid);
    DECLARE_ADD_CALL(childAt);
    DECLARE_ADD_CALL(acquireImage);
    DECLARE_ADD_CALL(acquireData);
}