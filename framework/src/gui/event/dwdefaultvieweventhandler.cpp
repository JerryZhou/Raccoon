#include "stable.h"
#include "graphicsview/dwdummys.h"
#include "graphicsview/dwgraphicsview.h"
#include "graphicsview/dwgraphicsscene.h"
#include "graphicsview/dwgraphicsitem.h"
#include "graphicsview/dwgraphicsutil.h"
#include "dwdefaultvieweventhandler.h"
#include "dwcore/dwlinkedlist.h"
#include "dwgraphics/dwpainter.h"

DW_IMPLEMENT_CLASS(DwDefaultViewEventHandler, 'DVEH', DwViewEventHandler);

//------------------------------------------------------------------------------
/**
*/
DwDefaultViewEventHandler::DwDefaultViewEventHandler(DwGraphicsView* view)
    : DwViewEventHandler(view)
    , m_startTipsTiming(false)
    , m_tipsCount(0)
    , m_showDirtyRect(false)
{
#if DW_ENABLE_EVENT_TIPS
    m_tipsTimer.start(30);
    m_tipsTimer.sigTimeout.connect(this, &DwDefaultViewEventHandler::onTipsTimer);
#endif

    DW_ASSERT(view->scene());
    if (view->scene())
    {
        view->scene()->addListener(this);
    }
}

//------------------------------------------------------------------------------
/**
*/
DwDefaultViewEventHandler::~DwDefaultViewEventHandler()
{
    m_tipsTimer.sigTimeout.disconnect_all();
    m_tipsTimer.stop();
    remAllDepends();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onEvent(DwEvent* evt)
{
    DW_ASSERT(evt->isA(DwSceneEvent::EventId()));
    // base class procedure
    DwViewEventHandler::onEvent(evt);
    DW_EVENT_CHECK(evt);

    DW_EVENT_BEGIN();
    // mouse event handler
    DW_EVENT_DISPATCH_AS(evt, DwMouseEvent, onMouseEvent);
    // keyborard event handler
    DW_EVENT_DISPATCH_AS(evt, DwKeyEvent, onKeyEvent);
    // char event handler
    DW_EVENT_DISPATCH_AS(evt, DwCharEvent, onCharEvent);
    // hover event handler
#if DW_ENABLE_EVENT_HOVER
    DW_EVENT_DISPATCH_AS(evt, DwHoverEvent, onHoverEvent);
#endif
    // timer event handler
    DW_EVENT_DISPATCH(evt, DwTimerEvent, onTimerEvent);
    // tips event handler
#if DW_ENABLE_EVENT_TIPS
    DW_EVENT_DISPATCH_AS(evt, DwTipsEvent, onTipsEvent);
#endif
    // resize event handler
    DW_EVENT_DISPATCH(evt, DwResizeEvent, onResizeEvent);
    // command event handler
#if DW_ENABLE_EVENT_COMMAND
    DW_EVENT_DISPATCH_AS(evt, DwCommandEvent, onCommandEvent);
#endif
    // paint event handler
    DW_EVENT_DISPATCH(evt, DwPaintEvent, onPaintEvent);
    // update the hover
    DW_EVENT_DISPATCH(evt, DwUpdateHoverEvent, onUpdateHoverEvent);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onNotify(Dw::GraphicsSceneNotify notify, DwGraphicsScene *scene, DwGraphicsItem* item)
{
    DW_UNUSED(scene);

    if (notify == Dw::SceneNotifyRemoveItem)
    {
        m_cachedItemsUnderMouse.removeOne(item);
        m_hoverItems.removeOne(item);
    }
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwDefaultViewEventHandler::onMouseEvent(DwMouseEvent* evt)
{
    // clear item when any mouse msg
    m_cachedItemsUnderMouse.clear();
    m_view->scene()->clearSearchCache();

    // dispatch msg
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwMouseMoveEvent, onMouseMoved);
    DW_EVENT_DISPATCH(evt, DwMouseDownEvent, onMouseDown);
    DW_EVENT_DISPATCH(evt, DwMouseUpEvent, onMouseUp);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwDefaultViewEventHandler::onMouseDown(DwMouseDownEvent* evt)
{
    DW_UNUSED(evt);
    if (m_view)
    {
#if DW_ENABLE_EVENT_HOVER
        dispatchHoverEvent(evt);
#endif

#if DW_ENABLE_EVENT_FOCUS
        dispatchFocusEvent(evt);
#endif
    }
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwDefaultViewEventHandler::onMouseUp(DwMouseUpEvent* evt)
{
    DW_UNUSED(evt);
#if DW_ENABLE_EVENT_HOVER
    if (m_view)
    {
        dispatchHoverEvent(evt);
    }
#endif
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwDefaultViewEventHandler::onMouseWheel(DwMouseWheelEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwDefaultViewEventHandler::onMouseMoved(DwMouseMoveEvent* evt)
{
    DW_UNUSED(evt);
#if DW_ENABLE_EVENT_HOVER
    if (m_view)
    {
        dispatchHoverEvent(evt);
    }
#endif
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onKeyEvent(DwKeyEvent* evt)
{
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwKeyDownEvent, onKeyDown);
    DW_EVENT_DISPATCH(evt, DwKeyUpEvent, onKeyUp);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwDefaultViewEventHandler::onKeyDown(DwKeyDownEvent* evt)
{
    DW_UNUSED(evt);

#if DW_ENABLE_EVENT_FOCUS
    dispatchFocusEvent(evt);
#endif
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwDefaultViewEventHandler::onKeyUp(DwKeyUpEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
	TODO: Dispath Char Input Event
*/
void DwDefaultViewEventHandler::onCharEvent(DwCharEvent* evt)
{
    DW_UNUSED(evt);
}

#if DW_ENABLE_EVENT_HOVER
//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onHoverEvent(DwHoverEvent* evt)
{
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwHoverEnterEvent, onHoverEnter);
    DW_EVENT_DISPATCH(evt, DwHoverGainEvent, onHoverGain);
    DW_EVENT_DISPATCH(evt, DwHoverLeaveEvent, onHoverLeave);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onHoverEnter(DwHoverEnterEvent* evt)
{
    DW_UNUSED(evt);

    if (m_view->scene())
    {
        m_view->scene()->setIndicator(Dw::LastHoverItem, m_hoverItems.back());
        m_startTipsTiming = true;
        m_tipsCount = ::GetTickCount();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onHoverGain(DwHoverGainEvent* evt)
{
    DW_UNUSED(evt);
    m_startTipsTiming = true;
    m_tipsCount = ::GetTickCount();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onHoverLeave(DwHoverLeaveEvent* evt)
{
    DW_UNUSED(evt);
    m_startTipsTiming = false;
    m_tipsCount = 0;
#if DW_ENABLE_EVENT_TIPS
    DwGraphicsItem* hoverItem = evt->indexInterestItemFromView(m_view); // same to interest item
    if (hoverItem)
    {

        DwHideToolTipEvent hideEvt(hoverItem);
        m_view->sendEvent(&hideEvt);
    }
#endif

    m_view->scene()->setIndicator(Dw::LastHoverItem, m_hoverItems.size() > 0 ? m_hoverItems.back() : NULL);

    // there are some hover item still
    if (m_hoverItems.size() > 0)
    {
        m_startTipsTiming = true;
        m_tipsCount = ::GetTickCount();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::dispatchHoverEvent(DwMouseEvent *evt)
{
    if (m_view->scene()->mouseGrabItem()) return;
    // Find the first item that accepts hover events, reusing earlier
    // calculated data is possible.
    DwGraphicsItem *underMouseItem = NULL;
    if (m_cachedItemsUnderMouse.isEmpty())
    {
        evt->translatedToScene(m_view);
        m_cachedItemsUnderMouse = m_view->scene()->items(evt->m_scenePoint);
    }
    if (m_cachedItemsUnderMouse.size() > 0)
    {
        underMouseItem = m_cachedItemsUnderMouse.front();
    }
    // ### ?? Is to reback to parent or to background item in m_cachedItemsUnderMouse
    DwGraphicsItem *item = DwGraphicsUtil::ancestorOfAccept(m_cachedItemsUnderMouse, Dw::ItemIsHoverable);

    // Find the common ancestor item for the new topmost hoverItem and the
    // last item in the hoverItem list.
    DwGraphicsItem *commonAncestorItem = (item && !m_hoverItems.isEmpty()) ? item->commonAncestorItem(m_hoverItems.back()) : 0;
    while (commonAncestorItem && !itemAcceptsHoverEvents_helper(commonAncestorItem))
    {
        commonAncestorItem = commonAncestorItem->parentItem();
    }
    // The common ancestor isn't in the same panel as the two hovered
    // items.
    if (commonAncestorItem && commonAncestorItem->topLevelItem() != item->topLevelItem())
    {
        commonAncestorItem = 0;
    }

    // Check if the common ancestor item is known.
    int index = commonAncestorItem ? m_hoverItems.indexOf(commonAncestorItem) : -1;
    // Send hover leaves to any existing hovered children of the common
    // ancestor item.
    for (int i = m_hoverItems.size() - 1; i > index; --i)
    {
        DwGraphicsItem *lastItem = m_hoverItems.back();
        m_hoverItems.pop_back();
        if (itemAcceptsHoverEvents_helper(lastItem))
        {
            DwHoverLeaveEvent hoverLeave;
            m_view->sendEvent(lastItem, &hoverLeave);
        }
    }

    // Item is a child of a known item. Generate enter events for the
    // missing links.
    DwLinkedList<DwGraphicsItem *> parents;
    DwGraphicsItem *parent = item;
    while (parent && parent != commonAncestorItem)
    {
        parents.prepend(parent);
        // TODO: break in panel
        parent = parent->parentItem();
    }
    while (parents.size() > 0)
    {
        parent = parents.front();
        m_hoverItems.append(parent);
        if (itemAcceptsHoverEvents_helper(parent))
        {
            DwHoverEnterEvent hoverEnter;
            m_view->sendEvent(parent, &hoverEnter);
        }
        parents.pop_front();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwDefaultViewEventHandler::itemAcceptsHoverEvents_helper(DwGraphicsItem *item) const
{
    if (!item) return false;
    if (item->flags() & Dw::ItemIsHoverable) return true;
    return false;
}
#endif

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::clearHovers()
{
#if DW_ENABLE_EVENT_HOVER
    // Send hover leaves to any existing hovered children of the common
    // ancestor item.
    for (int i = m_hoverItems.size() - 1; i > 0; --i)
    {
        DwGraphicsItem *lastItem = m_hoverItems.back();
        m_hoverItems.pop_back();
        if (itemAcceptsHoverEvents_helper(lastItem))
        {
            DwHoverLeaveEvent hoverLeave;
            m_view->sendEvent(lastItem, &hoverLeave);
        }
    }
#endif
}

#if DW_ENABLE_EVENT_FOCUS
//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::dispatchFocusEvent(DwMouseDownEvent* evt)	// reason mouse
{
    DW_UNUSED(evt);
    DW_CHECK_ASSERT(m_view->scene());

    if(m_view->scene()->mouseGrabItem()) return;

    if (m_cachedItemsUnderMouse.isEmpty())
    {
        evt->translatedToScene(m_view);
        m_cachedItemsUnderMouse = m_view->scene()->items(evt->m_scenePoint);
    }
    //DW_CHECK(!m_cachedItemsUnderMouse.isEmpty());

    // Set focus on the topmost enabled item that can take focus.
    bool setFocus = false;
    for (int i=0; i<m_cachedItemsUnderMouse.size(); ++i)
    {
        DwGraphicsItem *item = m_cachedItemsUnderMouse.at(i);

        if (itemAcceptsFocusEvents_helper(item) && item->isEnabled()
                && (item->focusPolicy() & Dw::ClickFocus))
        {
            setFocus = true;
            if (item != focusItem())
            {
                setFocusItem(item, Dw::MouseFocusReason);
            }
            break;
        }

        // is item will stop propagation when click(mean to set focus to null)
        if (item->flags() & Dw::ItemStopsClickFocusPropagation)
        {
            break;
        }
    }

    // If nobody could take focus, clear it.
    if (!m_view->scene()->stickyFocus() && !setFocus)
    {
        setFocusItem(NULL, Dw::MouseFocusReason);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::dispatchFocusEvent(DwKeyDownEvent* evt)		// reason tab
{
    DW_UNUSED(evt);
    if(m_view->scene()->keyBoardGrabItem()) return;
    // If no focus or focus do not care tab event
    DwGraphicsItem *focus = focusItem();
    if (!focus || !focus->isFlag(Dw::ItemSelfKeyFocus))
    {
        if (evt->keyCode() == VK_TAB
                && !(evt->isCtrlDown() || evt->isAltDown()))
        {
            if (focusNextPrevChild(!evt->isShfitDown()))
            {
                evt->accept();
                return;
            }
            evt->ignore();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwDefaultViewEventHandler::itemAcceptsFocusEvents_helper(DwGraphicsItem *item) const
{
    if (!item) return false;
    if (item->flags() & Dw::ItemIsFocusable) return true;
    return false;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem * DwDefaultViewEventHandler::focusItem() const
{
    return m_view->scene()->focusItem();
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem * DwDefaultViewEventHandler::lastFocusItem() const
{
    return m_view->scene()->lastFocusItem();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::setFocusItem(DwGraphicsItem *item, Dw::FocusReason reason)
{
    m_view->scene()->setFocusItem(item, reason);
}

//------------------------------------------------------------------------------
/**
*/
bool DwDefaultViewEventHandler::focusNextPrevChild(bool next)
{
    return m_view->scene()->focusNextPrevChild(next);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::setFocus(Dw::FocusReason focusReason/* = Dw::OtherFocusReason*/)
{
    m_view->scene()->setFocus(focusReason);
}
#endif

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::clearFocus()
{
    m_view->scene()->clearFocus();
}

//------------------------------------------------------------------------------
/**
*/
unsigned int const TipsTriggerElapse = 600;
void DwDefaultViewEventHandler::onTimerEvent(DwTimerEvent* evt)
{
    DW_UNUSED(evt);
#if DW_ENABLE_EVENT_TIPS
    if (m_view && m_view->scene() && m_startTipsTiming)
    {
        DWORD curTickCount = ::GetTickCount();
        if (curTickCount - m_tipsCount >= TipsTriggerElapse)
        {
            m_startTipsTiming = false;
            DwGraphicsItem* hoverItem = m_view->lastHoverItem();
            if (hoverItem)
            {
                //DW_OUT << "onTimerEvent sendEvent Tool Tips: " << curTickCount << ":" << m_tipsCount << DW_ENDL;
                DwToolTipEvent evt;
                m_view->sendEvent(hoverItem, &evt);

                if (evt.doneItem() && evt.m_tips.size() > 0)
                {
                    DwShowToolTipEvent showEvt(evt.doneItem());
                    showEvt.m_tips = evt.m_tips;
                    m_view->sendEvent(&showEvt);
                }
            }
        }
    }
#endif
}

#if DW_ENABLE_EVENT_TIPS
//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onTipsEvent(DwTipsEvent* evt)
{
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwShowToolTipEvent, onShowToolTip);
    DW_EVENT_DISPATCH(evt, DwHideToolTipEvent, onHideToolTip);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onShowToolTip(DwShowToolTipEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onHideToolTip(DwHideToolTipEvent* evt)
{
    DW_UNUSED(evt);
}
#endif

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onTipsTimer()
{
    DwTimerEvent evt(m_tipsTimer.timerId(), NULL);
    m_view->sendEvent(&evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::clearTips()
{
    m_startTipsTiming = false;
    m_tipsCount = 0;
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onResizeEvent(DwResizeEvent *evt)
{
    m_view->setViewPortRect(DwRectF(DwPointF(), evt->m_newSize));
    m_view->setSceneRect(DwRectF(DwPointF(), evt->m_newSize));

    clearHovers();
    clearFocus();
    clearTips();
}

#if DW_ENABLE_EVENT_COMMAND
//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onCommandEvent(DwCommandEvent *evt)
{
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwWindowCreateEvent, onWindowCreate);
    DW_EVENT_DISPATCH(evt, DwWindowDestroyEvent, onWindowDestroy);
    DW_EVENT_DISPATCH(evt, DwWindowClosedEvent, onWindowClosed);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onWindowCreate(DwWindowCreateEvent *evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onWindowClosed(DwWindowClosedEvent *evt)
{
    DW_UNUSED(evt);
    clearHovers();
    clearFocus();
    clearTips();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onWindowDestroy(DwWindowDestroyEvent *evt)
{
    DW_UNUSED(evt);
    //m_view->setWindow(NULL);
}
#endif

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onPaintEvent(DwPaintEvent *evt)
{
    DwRect sRect = m_view->mapToScene(m_view->mapFromDevice(evt->m_deviceRect));
    m_view->drawSceneRegion(DwRegion(sRect));
    evt->accept();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onEraseBkgndEvent(DwEraseBkgndEvent *evt)
{
    evt->accept();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::onUpdateHoverEvent(DwUpdateHoverEvent *evt)
{
#if DW_ENABLE_EVENT_HOVER
    if (evt->m_mousEvt)
    {
        dispatchHoverEvent(evt->m_mousEvt);
    }
#endif
    evt->accept();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultViewEventHandler::setShowDirtyRect(bool show)
{
    m_showDirtyRect = show;
}

//------------------------------------------------------------------------------
/**
*/
bool DwDefaultViewEventHandler::showDirtyRect()
{
    return m_showDirtyRect;
}
