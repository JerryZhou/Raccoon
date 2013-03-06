#include "stable.h"
#include "dwdummys.h"
#include "dwgraphicsevent.h"
#include "dwgraphicsview.h"
#include "dwgraphicsitem.h"
#include "dwgraphicsscene.h"
#include "dwcore/dwdebug.h"
#include "widgets/dwwidgetitem.h"
#include "dwgui/dwwindow.h"

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwSceneEvent, DwEvent);

//------------------------------------------------------------------------------
/**
*/
DwSceneEvent::DwSceneEvent()
    : m_interestItem(NULL)
    , m_doneItem(NULL)
    , m_view(NULL)
    , m_dispatchUpLevel(0)
    , m_dispatchDownLevel(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwSceneEvent::~DwSceneEvent()
{
    m_interestItem = NULL;
    m_doneItem = NULL;
    m_view = NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwVariant DwSceneEvent::vars(const DwString& key) const
{
    DW_UNUSED(key);
    return DwVariant();
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwSceneEvent::translatedToScene(const DwGraphicsView* view)
{
    DW_UNUSED(view);
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwSceneEvent::translatedToItem(const DwGraphicsItem* item)
{
    DW_UNUSED(item);
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
DwGraphicsItem* DwSceneEvent::indexInterestItemFromScene(const DwGraphicsScene *scene)
{
    DW_UNUSED(scene);
    return m_interestItem;
}

//------------------------------------------------------------------------------
/**
	The Default Implement Is Index From The indexInterestItem(scene)
*/
DwGraphicsItem* DwSceneEvent::indexInterestItemFromView(const DwGraphicsView *view)
{
    if(view)
    {
        translatedToScene(view);
    }
    return indexInterestItemFromScene(view ? view->scene() : NULL);
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwSceneEvent::setInterestItem(const DwGraphicsItem* item)
{
    m_interestItem = item;
}

//------------------------------------------------------------------------------
/**
*/
bool DwSceneEvent::isInterestedBy(const DwGraphicsItem* item)
{
    DW_UNUSED(item);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwSceneEvent::doneItem() const
{
    return m_doneItem;
}

//------------------------------------------------------------------------------
/**
*/
void DwSceneEvent::setDoneItem(const DwGraphicsItem* item)
{
    m_doneItem = item;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsView* DwSceneEvent::view() const
{
    return m_view;
}

//------------------------------------------------------------------------------
/**
*/
void DwSceneEvent::setView(const DwGraphicsView *v)
{
    m_view = v;
}

//------------------------------------------------------------------------------
/**
*/
int& DwSceneEvent::dispatchUpLevel()
{
    return m_dispatchUpLevel;
}

//------------------------------------------------------------------------------
/**
*/
int& DwSceneEvent::dispatchDownLevel()
{
    return m_dispatchDownLevel;
}

//------------------------------------------------------------------------------
/**
*/
void DwSceneEvent::clearLevels()
{
    clearUpLevel();
    clearDownLevel();
}

//------------------------------------------------------------------------------
/**
*/
void DwSceneEvent::clearUpLevel()
{
    m_dispatchUpLevel = 0;
}

//------------------------------------------------------------------------------
/**
*/
void DwSceneEvent::clearDownLevel()
{
    m_dispatchDownLevel = 0;
}

#if DW_ENABLE_EVENT_ITEM_INNER
//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwGraphicsItemEvent, DwSceneEvent);

DW_IMPLEMENT_EVENTID(DwItemCreateEvent, DwGraphicsItemEvent);

DW_IMPLEMENT_EVENTID(DwItemDestroyEvent, DwGraphicsItemEvent);

DW_IMPLEMENT_EVENTID(DwItemChangeEvent, DwGraphicsItemEvent);
#endif

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwEventAcceptedEvent, DwSceneEvent);

//------------------------------------------------------------------------------
/**
*/
DwEventAcceptedEvent::DwEventAcceptedEvent(DwSceneEvent* evt)
    : m_acceptedEvent(evt)
{
    // FORBID TO INDENTIFY SELF
    DW_ASSERT(evt && evt->eventId() != DwEventAcceptedEvent::EventId());
}

//------------------------------------------------------------------------------
/**
*/
DwEventAcceptedEvent::~DwEventAcceptedEvent()
{
    m_acceptedEvent = NULL;
}

//------------------------------------------------------------------------------
/**
    Do Not Interest By Any Item, Should Be Receive By Scene
*/
bool DwEventAcceptedEvent::isInterestedBy(const DwGraphicsItem* item)
{
    DW_UNUSED(item);
    return false;
}

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwMouseEvent, DwSceneEvent);

//------------------------------------------------------------------------------
/**
*/
DwMouseEvent::DwMouseEvent()
    : m_devicePoint(0, 0)
    , m_viewPoint(0, 0)
    , m_scenePoint(0, 0)
    , m_itemPoint(0, 0)
    , m_button(Dw::NoButton)
    , m_searched(false)
    , m_isNC(false)
    , m_nc(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwMouseEvent::~DwMouseEvent()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwMouseEvent::translatedToScene(const DwGraphicsView* view)
{
    DwSceneEvent::translatedToScene(view);
    // translated from view coordinate system to scene coordinate
    m_scenePoint = view->mapToScene(m_viewPoint);
}

//------------------------------------------------------------------------------
/**
*/
void DwMouseEvent::translatedToItem(const DwGraphicsItem* item)
{
    DwSceneEvent::translatedToItem(item);
    // translated from scene coordinate system to item local coordinate
    m_itemPoint = item->sceneTransform().inverted().map(m_scenePoint);
}

//------------------------------------------------------------------------------
/**
	IN THIS FUNCTION, THE M_MOUSEPOINT MUST IN SCENE COORDINATE
*/
DwGraphicsItem* DwMouseEvent::indexInterestItemFromScene(const DwGraphicsScene *scene)
{
    DwGraphicsItem* mouseGraber = scene ? scene->mouseGrabItem() : NULL;
    if (mouseGraber)
    {
        return mouseGraber;
    }

    // If the event is hijacked by the scene, just use target as the scene set
    if (m_interestItem || m_searched || !scene)
    {
        return m_interestItem;
    }

    // return topMost Item under mouse
    DwVector<DwGraphicsItem *> itemsUnderMouse = scene->items(m_scenePoint);
    for (int i=0; i<itemsUnderMouse.size(); ++i)
    {
        DwGraphicsItem *topMostItem = itemsUnderMouse.at(i);
        if (isInterestedBy(topMostItem))
        {
            m_interestItem = topMostItem;
            break;
        }

        if (topMostItem->flags() & Dw::ItemStopsMousePropagation)
        {
            break;
        }

    }
    m_searched = true;
    return m_interestItem;
}

//------------------------------------------------------------------------------
/**
*/
bool DwMouseEvent::isInterestedBy(const DwGraphicsItem* item)
{
    if (!m_button || item->acceptedMouseButtons() & m_button)
    {
        return true;
    }
    return false;
}

DW_IMPLEMENT_EVENTID(DwSceneMouseEvent, DwMouseEvent);

DW_IMPLEMENT_EVENTID(DwMouseDownEvent, DwMouseEvent);

DW_IMPLEMENT_EVENTID(DwMouseUpEvent, DwMouseEvent);

DW_IMPLEMENT_EVENTID(DwMouseWheelEvent, DwMouseEvent);

//------------------------------------------------------------------------------
/**
*/
DwMouseWheelEvent::DwMouseWheelEvent()
    : m_delta(0)
    , m_fwKeys(Dw::MKNOBUTTON)
{

}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwMouseWheelEvent::indexInterestItemFromScene(const DwGraphicsScene *scene)
{
    DwGraphicsItem* mouseGraber = scene->mouseGrabItem();
    if (mouseGraber)
    {
        return mouseGraber;
    }

    // If the event is hijacked by the scene, just use target as the scene set
    if (m_interestItem || m_searched)
    {
        return m_interestItem;
    }

    // current focus item
    if( scene->focusItem() )
    {
        m_interestItem = scene->focusItem();
        return m_interestItem;
    }

    m_searched = true;
    return m_interestItem;
}

//------------------------------------------------------------------------------
/**
*/
bool DwMouseWheelEvent::isInterestedBy(const DwGraphicsItem* item)
{
    return item->isFocusable();
}

//------------------------------------------------------------------------------
/**
*/
bool DwMouseWheelEvent::isRollUp()
{
    return !isRollDown();
}

//------------------------------------------------------------------------------
/**
*/
bool DwMouseWheelEvent::isRollDown()
{
    return m_delta < 0.00000001;
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwMouseWheelEvent::absDelta()
{
    return dwAbs(m_delta);
}

DW_IMPLEMENT_EVENTID(DwMouseMoveEvent, DwMouseEvent);

//------------------------------------------------------------------------------
/**
*/
DwMouseMoveEvent::DwMouseMoveEvent()
    : m_delta(0, 0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwMouseMoveEvent::indexInterestItemFromScene(const DwGraphicsScene *scene)
{
    DwGraphicsItem* mouseGraber = scene ? scene->mouseGrabItem() : NULL;
    if (mouseGraber)
    {
        return mouseGraber;
    }

    // If the event is hijacked by the scene, just use target as the scene set
    if (m_interestItem || m_searched || !scene)
    {
        return m_interestItem;
    }

    // current hover item
    if( scene->lastHoverItem() )
    {
        m_interestItem = scene->lastHoverItem();
        return m_interestItem;
    }

    // return topMost Item under mouse
    DwVector<DwGraphicsItem *> itemsUnderMouse = scene->items(m_scenePoint);
    for (int i=0; i<itemsUnderMouse.size(); ++i)
    {
        DwGraphicsItem *topMostItem = itemsUnderMouse.at(i);
        if (isInterestedBy(topMostItem))
        {
            m_interestItem = topMostItem;
            break;
        }

        if (topMostItem->flags() & Dw::ItemStopsMousePropagation)
        {
            break;
        }

    }
    m_searched = true;
    return m_interestItem;
}

DW_IMPLEMENT_EVENTID(DwMouseDbClickEvent, DwMouseEvent);

DW_IMPLEMENT_EVENTID(DwMouseClickedEvent, DwMouseEvent);

//------------------------------------------------------------------------------
/**
*/
DwMouseClickedEvent::DwMouseClickedEvent(DwGraphicsItem* clickItem)
    : m_clickedItem(clickItem)
{

}

//------------------------------------------------------------------------------
/**
*/
DwMouseClickedEvent::~DwMouseClickedEvent()
{
    m_clickedItem = NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwMouseClickedEvent::indexInterestItemFromScene(const DwGraphicsScene *scene)
{
    DW_UNUSED(scene);
    return m_clickedItem;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwMouseClickedEvent::indexInterestItemFromView(const DwGraphicsView *view)
{
    DW_UNUSED(view);
    return m_clickedItem;
}

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwKeyEvent, DwSceneEvent);

//------------------------------------------------------------------------------
/**
*/
DwKeyEvent::DwKeyEvent()
    : m_virtualKey(0)
    , m_param(0)
    , m_isShiftDown(0)
    , m_isControlDown(0)
    , m_isAltDown(0)
    , m_modifier(0)
{
    m_isShiftDown = DwKeyEvent::isKeyModifierDown(Dw::SHIFT);
    m_isControlDown = DwKeyEvent::isKeyModifierDown(Dw::CTRL);
    m_isAltDown = DwKeyEvent::isKeyModifierDown(Dw::ALT);

    m_isShiftDown ? (m_modifier &= Dw::SHIFT) : 0 ;
    m_isControlDown ? (m_modifier &= Dw::CTRL) : 0 ;
    m_isAltDown ? (m_modifier &= Dw::ALT) : 0 ;
}

//------------------------------------------------------------------------------
/**
*/
DwKeyEvent::~DwKeyEvent()
{
    m_virtualKey = 0;
    m_param = 0;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwKeyEvent::indexInterestItemFromScene(const DwGraphicsScene *scene)
{
    DwGraphicsItem *keyGraber = scene->keyBoardGrabItem();
    if (keyGraber)
    {
        return keyGraber;
    }

    // If the event is hijacked by the scene, just use target as the scene set
    if (m_interestItem)
    {
        return m_interestItem;
    }
    return scene->focusItem();
}

//------------------------------------------------------------------------------
/**
*/
bool DwKeyEvent::isInterestedBy(const DwGraphicsItem* item)
{
    DW_ASSERT(item);
    DW_ASSERT(item->scene());
    return item->hasFocus() || item->scene()->mouseGrabItem() == item;
}

//------------------------------------------------------------------------------
/**
*/
bool DwKeyEvent::isExtendKey() const
{
    return m_extendCode;
}

//------------------------------------------------------------------------------
/**
*/
bool DwKeyEvent::isKeyModifierDown(Dw::Modifier modifier)
{
    switch (modifier)
    {
    case Dw::SHIFT :
        return HIBYTE(::GetKeyState(VK_SHIFT)) > 0;
        break;
    case Dw::CTRL :
        return HIBYTE(::GetKeyState(VK_CONTROL)) > 0;
        break;
    case Dw::ALT :
        return HIBYTE(::GetKeyState(VK_MENU)) > 0;
        break;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwKeyEvent::isAnyKeyModifier()
{
    return DwKeyEvent::isKeyModifierDown(Dw::SHIFT)
           || DwKeyEvent::isKeyModifierDown(Dw::CTRL)
           || DwKeyEvent::isKeyModifierDown(Dw::ALT);
}

DW_IMPLEMENT_EVENTID(DwKeyDownEvent, DwKeyEvent);

DW_IMPLEMENT_EVENTID(DwKeyUpEvent, DwKeyEvent);

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwCharEvent, DwSceneEvent);

//------------------------------------------------------------------------------
/**
*/
DwCharEvent::DwCharEvent()
    : m_characterCode(0)
    , m_param(0)
{

}

//------------------------------------------------------------------------------
/**
*/
DwCharEvent::~DwCharEvent()
{
    m_characterCode = 0;
    m_param = 0;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwCharEvent::indexInterestItemFromScene(const DwGraphicsScene *scene)
{
    DwGraphicsItem *keyGraber = scene->keyBoardGrabItem();
    if (keyGraber)
    {
        return keyGraber;
    }
    // If the event is hijacked by the scene, just use target as the scene set
    if (m_interestItem)
    {
        return m_interestItem;
    }
    return scene->focusItem();
}

//------------------------------------------------------------------------------
/**
*/
bool DwCharEvent::isInterestedBy(const DwGraphicsItem* item)
{
    DW_ASSERT(item);
    DW_ASSERT(item->scene());
    return item->hasFocus() || item->scene()->mouseGrabItem() == item;
}

#if DW_ENABLE_EVENT_IME
//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwIMEEvent, DwSceneEvent);

DW_IMPLEMENT_EVENTID(DwIMECharEvent, DwIMEEvent);

DW_IMPLEMENT_EVENTID(DwIMEKeyEvent, DwIMEEvent);

DW_IMPLEMENT_EVENTID(DwIMEKeyDownEvent, DwIMEKeyEvent);

DW_IMPLEMENT_EVENTID(DwIMEKeyUpEvent, DwIMEKeyEvent);

DW_IMPLEMENT_EVENTID(DwIMESetConTextEvent, DwIMEEvent);

DW_IMPLEMENT_EVENTID(DwIMENotifyEvent, DwIMEEvent);

DW_IMPLEMENT_EVENTID(DwIMEControlEvent, DwIMEEvent);

DW_IMPLEMENT_EVENTID(DwIMECompositionFullEvent, DwIMEEvent);

DW_IMPLEMENT_EVENTID(DwIMESelectEvent, DwIMEEvent);

DW_IMPLEMENT_EVENTID(DwIMERequestEvent, DwIMEEvent);
#endif

#if DW_ENABLE_EVENT_HOVER
//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwHoverEvent, DwSceneEvent);

//------------------------------------------------------------------------------
/**
*/
bool DwHoverEvent::isInterestedBy(const DwGraphicsItem* item)
{
    return item->isHoverable();
}

DW_IMPLEMENT_EVENTID(DwHoverEnterEvent, DwHoverEvent);

DW_IMPLEMENT_EVENTID(DwHoverGainEvent, DwHoverEvent);

//------------------------------------------------------------------------------
/**
*/
DwHoverGainEvent::DwHoverGainEvent(DwGraphicsItem* item)
{
    setInterestItem(item);
}

DW_IMPLEMENT_EVENTID(DwHoverLeaveEvent, DwHoverEvent);
#endif

#if DW_ENABLE_EVENT_FOCUS
//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwFocusEvent, DwSceneEvent);

//------------------------------------------------------------------------------
/**
*/
DwFocusEvent::DwFocusEvent()
    : m_reason(Dw::NoFocusReason)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
bool DwFocusEvent::isInterestedBy(const DwGraphicsItem* item)
{
    return item->isFocusable();
}

DW_IMPLEMENT_EVENTID(DwFocusInEvent, DwFocusEvent);

DW_IMPLEMENT_EVENTID(DwFocusOutEvent, DwFocusEvent);

DW_IMPLEMENT_EVENTID(DwFocusGainEvent, DwFocusEvent);
#endif

#if DW_ENABLE_EVENT_DRAGDROP
//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwSceneDragEvent, DwSceneEvent);

//------------------------------------------------------------------------------
/**
*/
DwSceneDragEvent::DwSceneDragEvent(DwGraphicsItem* item)
{
    DwSceneEvent::m_interestItem = item;
}

//------------------------------------------------------------------------------
/**
*/
DwSceneDragEvent::~DwSceneDragEvent()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwSceneDragEvent::constructFromMouseEvent(DwMouseEvent* evt)
{
    DW_ASSERT(evt);
    DW_ASSERT(evt->view());
    // confrim the evt is in scene space, so we can use the m_scenePoint
    evt->translatedToScene(evt->view());
    // copy the sceneevent
    m_view = evt->view();
    // copy the mouseevent
    m_button = evt->m_button;
    m_screenPoint = evt->m_screenPoint;
    m_devicePoint = evt->m_devicePoint;
    m_scenePoint = evt->m_scenePoint;
    m_viewPoint = evt->m_viewPoint;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwSceneDragEvent::indexInterestItemFromScene(const DwGraphicsScene *scene)
{
    DW_UNUSED(scene);
    return m_interestItem;
}

//------------------------------------------------------------------------------
/**
*/
bool DwSceneDragEvent::isInterestedBy(const DwGraphicsItem* item)
{
    return item && item->isMovable();
}

DW_IMPLEMENT_EVENTID(DwDragStartEvent, DwSceneDragEvent);

DW_IMPLEMENT_EVENTID(DwDragMoveEvent, DwSceneDragEvent);

DW_IMPLEMENT_EVENTID(DwDragEndEvent, DwSceneDragEvent);

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwDragDropEvent, DwMouseEvent);

//------------------------------------------------------------------------------
/**
*/
DwDragDropEvent::DwDragDropEvent()
    : m_excludeItem(NULL)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwDragDropEvent::~DwDragDropEvent()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwDragDropEvent::constructFromMouseEvent(DwMouseEvent* evt)
{
    m_screenPoint = evt->m_screenPoint;
    m_devicePoint = evt->m_devicePoint;
    m_scenePoint = evt->m_scenePoint;
    m_viewPoint = evt->m_viewPoint;
    m_button = evt->m_button;
}

//------------------------------------------------------------------------------
/**
*/
void DwDragDropEvent::constructFromDragEvent(DwSceneDragEvent* evt)
{
    DW_UNUSED(evt);
    m_screenPoint = evt->m_screenPoint;
    m_devicePoint = evt->m_devicePoint;
    m_scenePoint = evt->m_scenePoint;
    m_viewPoint = evt->m_viewPoint;
    m_button = evt->m_button;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwDragDropEvent::indexInterestItemFromScene(const DwGraphicsScene *scene)
{
    DW_UNUSED(scene);
    // If the event is hijacked by the scene, just use target as the scene set
    if (m_interestItem || m_searched)
    {
        return m_interestItem;
    }

    // return topMost Item under mouse
    DwVector<DwGraphicsItem *> itemsUnderMouse = scene->items(m_scenePoint);
    for (int i=0; i<itemsUnderMouse.size(); ++i)
    {
        DwGraphicsItem *topMostItem = itemsUnderMouse.at(i);
        if (isInterestedBy(topMostItem))
        {
            m_interestItem = topMostItem;
            break;
        }

        if (topMostItem->flags() & Dw::ItemStopsMousePropagation)
        {
            break;
        }

    }
    m_searched = true;
    return m_interestItem;
}

//------------------------------------------------------------------------------
/**
*/
bool DwDragDropEvent::isInterestedBy(const DwGraphicsItem* item)
{
    DW_UNUSED(item);
    if (item
            && item != m_excludeItem
            && item->isAcceptDrops()
            && item->isVisible()
            && item->isEnabled())
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
const DwMimeData *DwDragDropEvent::mimeData() const
{
    return &m_mimeData;
}
#endif

#if DW_ENABLE_EVENT_GRABER
//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwGraberEvent, DwSceneEvent);

DW_IMPLEMENT_EVENTID(DwMouseGraberBaseEvent, DwGraberEvent);

DW_IMPLEMENT_EVENTID(DwGrabMouseEvent, DwMouseGraberBaseEvent);

DW_IMPLEMENT_EVENTID(DwUngrabMouseEvent, DwMouseGraberBaseEvent);

DW_IMPLEMENT_EVENTID(DwMouseGainEvent, DwMouseGraberBaseEvent);

DW_IMPLEMENT_EVENTID(DwKeyboardGraberBaseEvent, DwGraberEvent);

DW_IMPLEMENT_EVENTID(DwGrabKeyboardEvent, DwKeyboardGraberBaseEvent);

DW_IMPLEMENT_EVENTID(DwUngrabKeyboardEvent, DwKeyboardGraberBaseEvent);

DW_IMPLEMENT_EVENTID(DwKeyboardGainEvent, DwKeyboardGraberBaseEvent);
#endif

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwTimerEvent, DwSceneEvent);

//------------------------------------------------------------------------------
/**
*/
DwTimerEvent::DwTimerEvent(unsigned int timerId, DwWindow *window/* = NULL*/)
    : m_timerId(InvalidTimerId)
    , m_window(window)
{
    setTimerId(timerId);
}

//------------------------------------------------------------------------------
/**
*/
DwTimerEvent::~DwTimerEvent()
{
    m_timerId = InvalidTimerId;
}

//------------------------------------------------------------------------------
/**
*/
unsigned int DwTimerEvent::timeId() const
{
    return m_timerId;
}

//------------------------------------------------------------------------------
/**
*/
void DwTimerEvent::setTimerId(unsigned int timerId)
{
    DW_ASSERT(timerId != InvalidTimerId);
    m_timerId = timerId;
}

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwWinEventBase, DwSceneEvent);
//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwWinEventBase::indexInterestItemFromScene(const DwGraphicsScene *)
{
    if (m_interestItem)
    {
        return m_interestItem;
    }
    if (!m_view)
    {
        return NULL;
    }

    m_interestItem = m_view->windowItem();
    return m_interestItem;
}

DW_IMPLEMENT_EVENTID(DwResizeEvent, DwWinEventBase);

//------------------------------------------------------------------------------
/**
*/
DwResizeEvent::DwResizeEvent(ResizePattern pattern, const DwSizeF& oldOne, const DwSizeF& newOne)
    : m_pattern(pattern)
    , m_oldSize(oldOne)
    , m_newSize(newOne)
    , m_hwnd(0)
{
    setUp(false);
}

//------------------------------------------------------------------------------
/**
*/
DwResizeEvent::~DwResizeEvent()
{
    ;
}

//------------------------------------------------------------------------------
/**
	TODO: xx, Only Dispatch To LayoutItem
*/
bool DwResizeEvent::isInterestedBy(const DwGraphicsItem* item)
{
    return item->isVisible();
}

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwMoveEvent, DwWinEventBase);

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwWinOnShowEvent, DwWinEventBase);

//------------------------------------------------------------------------------
/**
*/
DwWinOnShowEvent::DwWinOnShowEvent()
    : m_winId(0)
    , m_window(NULL)
    , m_showCommand(Dw::SW_Normal)
{
    ;
}

#if DW_ENABLE_EVENT_VISIBLE
//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwVisibleEvent, DwSceneEvent);

//------------------------------------------------------------------------------
/**
*/
DwVisibleEvent::DwVisibleEvent()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwVisibleEvent::~DwVisibleEvent()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
bool DwVisibleEvent::isInterestedBy(const DwGraphicsItem* item)
{
    return item->isVisible();
}

DW_IMPLEMENT_EVENTID(DwShowEvent, DwVisibleEvent);

DW_IMPLEMENT_EVENTID(DwHideEvent, DwVisibleEvent);
#endif

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwWinEvent, DwWinEventBase);

//------------------------------------------------------------------------------
/**
*/
DwWinEvent::DwWinEvent()
    : m_winId(0)
{
    ;
}

DW_IMPLEMENT_EVENTID(DwNcHitTestEvent, DwWinEvent);

//------------------------------------------------------------------------------
/**
*/
DwNcHitTestEvent::DwNcHitTestEvent()
    : m_searched(false)
    , m_result(HTCLIENT)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwNcHitTestEvent::translatedToScene(const DwGraphicsView* view)
{
    DwWinEvent::translatedToScene(view);
    // translated from view coordinate system to scene coordinate
    m_scenePoint = view->mapToScene(m_viewPoint);
}

//------------------------------------------------------------------------------
/**
*/
void DwNcHitTestEvent::translatedToItem(const DwGraphicsItem* item)
{
    DwWinEvent::translatedToItem(item);
    // translated from scene coordinate system to item local coordinate
    m_itemPoint = item->sceneTransform().inverted().map(m_scenePoint);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwNcHitTestEvent::indexInterestItemFromScene(const DwGraphicsScene *scene)
{
    // If the event is hijacked by the scene, just use target as the scene set
    if (m_interestItem || m_searched)
    {
        return m_interestItem;
    }

    // return topMost Item under mouse
    DwVector<DwGraphicsItem *> itemsUnderMouse = scene->items(m_scenePoint);
    for (int i=0; i<itemsUnderMouse.size(); ++i)
    {
        DwGraphicsItem *topMostItem = itemsUnderMouse.at(i);
        if (isInterestedBy(topMostItem))
        {
            m_interestItem = topMostItem;
            break;
        }

        if (topMostItem->flags() & Dw::ItemStopsMousePropagation)
        {
            break;
        }

    }
    m_searched = true;
    return m_interestItem;
}

//------------------------------------------------------------------------------
/**
*/
bool DwNcHitTestEvent::isInterestedBy(const DwGraphicsItem* item)
{
    if (item && item->isA(DwWidgetItem::RTTI()))
    {
        return true;
    }
    return false;
}

#if DW_ENABLE_EVENT_COMMAND
//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwCommandEvent, DwWinEventBase);

//------------------------------------------------------------------------------
/**
*/
DwCommandEvent::DwCommandEvent()
    : m_winId(0)
{
    ;
}
DW_IMPLEMENT_EVENTID(DwWindowCreateEvent, DwCommandEvent);

//------------------------------------------------------------------------------
/**
*/
DwWindowCreateEvent::DwWindowCreateEvent(DwWindow* window)
    : m_window(window)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwWindowCreateEvent::~DwWindowCreateEvent()
{
    m_window = NULL;
}

DW_IMPLEMENT_EVENTID(DwWindowDestroyEvent, DwCommandEvent);

//------------------------------------------------------------------------------
/**
*/
DwWindowDestroyEvent::DwWindowDestroyEvent(DwWindow* window)
    : m_window(window)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwWindowDestroyEvent::~DwWindowDestroyEvent()
{
    m_window = NULL;
}

DW_IMPLEMENT_EVENTID(DwWindowClosedEvent, DwCommandEvent);
//------------------------------------------------------------------------------
/**
*/
DwWindowClosedEvent::DwWindowClosedEvent(DwWindow* window)
    : m_window(window)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwWindowClosedEvent::~DwWindowClosedEvent()
{
    m_window = NULL;
}


DW_IMPLEMENT_EVENTID(DwCursorChangeEvent, DwCommandEvent);

DW_IMPLEMENT_EVENTID(DwContexMenuEvent, DwCommandEvent);

DW_IMPLEMENT_EVENTID(DwQuitEvent, DwCommandEvent);
#endif

#if DW_ENABLE_EVENT_WIN_COMMAND
//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwWinCommandEventBase, DwWinEventBase);

//------------------------------------------------------------------------------
/**
*/
DwWinCommandEventBase::DwWinCommandEventBase()
    : m_wparam(0)
    , m_lparam(0)
    , m_lResult(1)
    , m_handled(FALSE)
    , m_hWnd(0)
{
    ;
}

DW_IMPLEMENT_EVENTID(DwWinCommandEvent, DwWinCommandEventBase);

DW_IMPLEMENT_EVENTID(DwWinSysCommandEvent, DwWinCommandEventBase);
#endif

#if DW_ENABLE_EVENT_TIPS
//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwTipsEvent, DwSceneEvent);

//------------------------------------------------------------------------------
/**
	Only Interest By Assigned Item
*/
bool DwTipsEvent::isInterestedBy(const DwGraphicsItem* item)
{
    return (!m_interestItem || m_interestItem == item);
}

DW_IMPLEMENT_EVENTID(DwToolTipEvent, DwTipsEvent);

DW_IMPLEMENT_EVENTID(DwWhatsThisEvent, DwTipsEvent);

DW_IMPLEMENT_EVENTID(DwStatusTipEvent, DwTipsEvent);

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwVisibleTipsEvent, DwTipsEvent);

//------------------------------------------------------------------------------
/**
*/
DwVisibleTipsEvent::DwVisibleTipsEvent(DwGraphicsItem* targetItem)
    : m_targetItem(targetItem)
{

}

//------------------------------------------------------------------------------
/**
*/
DwVisibleTipsEvent::~DwVisibleTipsEvent()
{
    m_targetItem = NULL;
}

DW_IMPLEMENT_EVENTID(DwShowToolTipEvent, DwVisibleTipsEvent);

DW_IMPLEMENT_EVENTID(DwHideToolTipEvent, DwVisibleTipsEvent);
#endif

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwPaintEvent, DwSceneEvent);

//------------------------------------------------------------------------------
/**
*/
DwPaintEvent::DwPaintEvent()
: m_window(NULL)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwPaintEvent::~DwPaintEvent()
{
    m_window = NULL;
}

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwEraseBkgndEvent, DwSceneEvent);

//------------------------------------------------------------------------------
/**
*/
DwEraseBkgndEvent::DwEraseBkgndEvent()
    : m_window(NULL)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwEraseBkgndEvent::~DwEraseBkgndEvent()
{
    m_window = NULL;
}

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwMouseNcToClientEvent, DwSceneEvent);

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwWindowEvent, DwWinEventBase);

//------------------------------------------------------------------------------
/**
*/
DwWindowEvent::DwWindowEvent()
    : m_result(0)
    , m_winId(0)
    , m_msg(0)
    , m_wparam(0)
    , m_lparam(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwWindowEvent::assignWin(int winId, unsigned int msg, unsigned int wp, unsigned int lp)
{
    m_winId = winId;
    m_msg = msg;
    m_wparam = wp;
    m_lparam = lp;
}

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwWindowActiveEvent, DwWindowEvent);

//------------------------------------------------------------------------------
/**
*/
DwWindowActiveEvent::DwWindowActiveEvent()
: m_actived(true)
{
    ;
}

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwUpdateHoverEvent, DwSceneEvent);