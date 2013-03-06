#include "stable.h"
#include "dwgraphicsscene.h"
#include "dwgraphicsitem.h"
#include "dwgraphicsscenelinearindex.h"
#include "dwgraphicsscenebsptreeindex.h"
#include "dwgraphicsscenelistener.h"
#include "dwgraphicsview.h"
#include "dwgraphicseventdispatch.h"
#include "dwgraphicsevent.h"
#include "dwgraphicseventhandler.h"
#include "dwgraphicsutil.h"
#include "dwdummys.h"
#include "dwgraphics/dwpainter.h"
#include "dwgraphics/dwcachedcanvas.h"

DW_IMPLEMENT_CLASS(DwGraphicsScene, 'DGSE', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsScene::DwGraphicsScene(DwObject *parent/* = NULL*/)
    : DwObject(parent)
    , m_hasSceneRect(0)
    , m_dirtyTopLevelItems(1)
    , m_eventHandler(NULL)
    , m_maxSiblingIndex(0)
    , m_painter(NULL)
    , m_hasFocus(0)
    , m_stickyFocus(0)
    , m_cachedItemsValid(0)
    , m_lastMouseGrabberItemHasImplicitMouseGrab(0)
    , m_lastMouseGrabberItem(0)
    , m_clearTopLevelItemsWhenDestruct(0)
    , m_updateAll(0)
    , m_calledEmitUpdated(0)
    , m_debugDraw(0)
{
    //m_index = new DwGraphicsSceneLinearIndex(this);
    m_index = new DwGraphicsSceneBspTreeIndex(this);
    memset((void*)m_indicator, 0, sizeof(m_indicator));

    m_updateTimer.sigTimeout.connect(this, &DwGraphicsScene::onUpdateTimerEvent);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsScene::DwGraphicsScene(DwRtti *indexClass, DwObject *parent/* = NULL*/)
    : DwObject(parent)
    , m_hasSceneRect(0)
    , m_dirtyTopLevelItems(1)
    , m_eventHandler(NULL)
    , m_maxSiblingIndex(0)
    , m_painter(NULL)
    , m_hasFocus(0)
    , m_stickyFocus(0)
    , m_cachedItemsValid(0)
    , m_lastMouseGrabberItemHasImplicitMouseGrab(0)
    , m_lastMouseGrabberItem(0)
    , m_clearTopLevelItemsWhenDestruct(0)
    , m_updateAll(0)
    , m_calledEmitUpdated(0)
	, m_debugDraw(0)
{
    DW_ASSERT(indexClass && indexClass->isDerivedFrom(DwGraphicsSceneIndex::RTTI()));
    m_index = (DwGraphicsSceneIndex* )indexClass->create();
    m_index->setGraphicsScene(this);
    memset((void*)m_indicator, 0, sizeof(m_indicator));

    m_updateTimer.sigTimeout.connect(this, &DwGraphicsScene::onUpdateTimerEvent);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsScene::~DwGraphicsScene()
{
    m_updateTimer.sigTimeout.disconnect_all();
    if(m_updateTimer.isActive()) m_updateTimer.stop();

    DW_ASSERT(!m_painter);
    if (m_clearTopLevelItemsWhenDestruct)
    {
        DwVector<DwGraphicsItem *> topItems = topLevelItems();
        if (m_topLevelItems.size() > 0)
        {
            DwVector<DwGraphicsItem *> topItems = m_topLevelItems;
            for (int i=0; i<topItems.size(); ++i)
            {
                delete topItems.at(i);
            }
            topItems.clear();
        }
    }

    DwVector<DwGraphicsItem *> allItems = items();
    for (int i = 0 ; i < allItems.size(); ++i)
    {
        removeItem(allItems.at(i));
    }

    m_index->clear();
    delete m_index;
    m_index = NULL;

    if (this->m_views.size() > 0)
    {
        // first copy, as the view->setScene(NULL) will remove view in scene
        DwVector<DwGraphicsView *> removeViews = this->m_views;
        for (int index=0; index<removeViews.size(); ++index)
        {
            removeViews.at(index)->setScene(NULL);
        }
    }

    setEventHandler(NULL);

    removeAllListeners();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::addItem(DwGraphicsItem *item)
{
    DW_CHECK_ASSERT(item);
    DW_CHECK_ASSERT(item->scene() != this);// should never add to this

    // Notify event, before truely added
    notifyListener(Dw::SceneNotifyAddItem, item);

    // Remove this item from its existing scene
    if (DwGraphicsScene *oldScene = item->scene())
    {
        oldScene->removeItem(item);
    }

    if (DwGraphicsItem* parent = item->parentItem())
    {
        // Detach this item from its parent if the parent's scene is different
        // from this scene.
        if (parent->scene() != this)
        {
            item->setParentItem(NULL);
        }
    }
    else
    {
        // Add to list of toplevels if this item is a toplevel.
        registerTopLevelItem(item);
    }

    // Add the item to this scene
    item->m_scene = this;
    m_index->addItem(item);
    notifyListener(Dw::SceneNotifyAfterAdd, item);

    // Focus Stuffs
    if (!tabFirstFocus() && item->isFlag(Dw::ItemIsFocusable))
    {
        setIndicator(Dw::TabFirstFocus, item);
    }
    else
    {
		//  maintain the focus chain
        DwGraphicsItem *firstFocus = tabFirstFocus();
        if (firstFocus && item->isFlag(Dw::ItemIsFocusable))
        {
            firstFocus->setFocusPre(item);
        }
    }

    // Add all children recursively
    for (int i = 0; i < item->children().size(); ++i)
    {
        addItem(item->children().at(i));
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::removeItem(DwGraphicsItem *item)
{
    DW_CHECK_ASSERT(item);
    DW_CHECK_ASSERT(item->scene() == this);

    removeItemHelper(item);

    // TODO: xx maintain the focus chain
}

//------------------------------------------------------------------------------
/**
	return the items
*/
const DwVector<DwGraphicsItem*>& DwGraphicsScene::topLevelItems() const
{
    return m_topLevelItems;
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwGraphicsScene::sceneRect() const
{
    if (m_hasSceneRect)
    {
        return m_sceneRect;
    }
    else
    {
        return itemsBoundingRect();
    }
}

//------------------------------------------------------------------------------
/**
	TODO: change event
*/
void DwGraphicsScene::setSceneRect(const DwRectF &rect)
{
    m_hasSceneRect = 1;
    m_sceneRect = rect;

    if (m_index->isA(DwGraphicsSceneBspTreeIndex::RTTI()))
    {
        DwGraphicsSceneBspTreeIndex* bspIndex = (DwGraphicsSceneBspTreeIndex*)(m_index);
        bspIndex->updateSceneRect(m_sceneRect);
    }
}

//------------------------------------------------------------------------------
/**
	The Result In Scene Coordinate System
*/
DwRectF DwGraphicsScene::itemsBoundingRect() const
{
    DwRectF boundingRect;
    for (int i=0; i<m_topLevelItems.size(); ++i)
    {
        boundingRect |= m_topLevelItems.at(i)->sceneEffectiveBoundingRect();
    }
    return boundingRect;
}

//------------------------------------------------------------------------------
/**
	TODO: xx
	render all of items in source to the target in painter
	@param painter : point to a canvas : may be from a view, may be a texture
	@param target : in painter coordinate sytem
	@param source : in scene coordinate sytem
	@param aspectRatioMode : describing how source mapping to target
*/
void DwGraphicsScene::render(DwPainter *painter,
                             const DwRectF &target/* = DwRectF()*/, const DwRectF &source/* = DwRectF()*/,
                             Dw::AspectRatioMode aspectRatioMode/* = Dw::KeepAspectRatio*/)
{
    DW_UNUSED(painter);
    DW_UNUSED(target);
    DW_UNUSED(source);
    DW_UNUSED(aspectRatioMode);
}

//------------------------------------------------------------------------------
/**
	TODO: xx
	render all of items in exposedRect to the painter
	@param painter : point to a canvas : may be from a view, may be a texture
	@param viewTransform : transform the scene coordinate system to painter(device)
	@param exposedRect : in scene sytem
*/
void DwGraphicsScene::drawItems(DwPainter *painter, const DwTransform *const viewTransform,
                                const DwRegion &exposedRegion)
{
    DwRect exposedSceneRect = exposedRegion.boundingRect();

    const DwVector<DwGraphicsItem *> tli = m_index->estimateTopLevelItems(exposedSceneRect, Dw::AscendingOrder);
    if (tli.size() > 0)
    {
        painter->save();
        for (int i = 0; i < tli.size(); ++i)
        {
            drawSubtreeRecursive(tli.at(i), painter, viewTransform, exposedRegion, tli.at(i)->isEnabled());
        }
        painter->restrore();
    }
}

//------------------------------------------------------------------------------
/**
	TODO: xx
	render all of items in exposedRect to the painter
	@param item : item to draw rescursive
	@param painter : point to a canvas : may be from a view, may be a texture
	@param viewTransform : transform the scene coordinate sytem to painter(device)
	@param exposedRect : in scene sytem
	@param parentOpacity : parent item's opacity
*/
void DwGraphicsScene::drawSubtreeRecursive(DwGraphicsItem *item, DwPainter *painter, const DwTransform *const viewTransform,
        const DwRegion &exposedRegion, DwReal parentOpacity/* = DwReal(1.0)*/, bool parentEnabled /*= true*/)
{
    DW_ASSERT(item);

    if (!item ||!item->isVisible())
        return;

    const bool itemHasChildren = !item->children().isEmpty();

    // item to painter(device): item to scene, scene to painter
    DwTransform transformToView = viewTransform ? item->sceneTransform() * (*viewTransform) : item->sceneTransform();

    bool exposedIntersects = exposedRegion.isIntersects(item->sceneBoundingRect().toRect());
    bool drawItem = !(item->flags() & Dw::ItemHasNoContents) && exposedIntersects;
    if (!drawItem && !itemHasChildren)
    {
        return;
    }

    // TODO: effect draw
    DwReal opacity = parentOpacity * item->opacity();
    // totaly transpant
    if (dwFuzzyIsNull(opacity))
    {
        return;
    }
    bool enabled = parentEnabled && item->isEnabled();

    draw(item, painter, viewTransform, transformToView, exposedRegion, opacity, NULL, drawItem, enabled);
}

//------------------------------------------------------------------------------
/**
	TODO: xx : optimize cache result
	render all of items in exposedRect to the painter
	@param item : item to draw rescursive
	@param painter : point to a canvas : may be from a view, may be a texture
	@param viewTransform : transform the scene coordinate sytem to painter
	@param exposedRect : in scene sytem
	@param parentOpacity : parent item's opacity
*/
void DwGraphicsScene::draw(DwGraphicsItem *item, DwPainter *painter, const DwTransform *const viewTransform,
                           const DwTransform &transformToView, const DwRegion &exposedRegion,
                           DwReal opacity, const DwTransform *const effectTransform, bool drawItem, bool enabled)
{
    DW_ASSERT(item);
    DW_UNUSED(effectTransform);
    DwRefPtr refPainter((RefHandler&)m_painter, (RefHandler)painter);
    DwRectF  sceneBounding = item->sceneBoundingRect();

    const bool itemClipsChildrenToShape = item->isFlag(Dw::ItemClipsChildrenToShape);
    const bool itemClipsToShape = item->isFlag(Dw::ItemClipsToShape);
    const bool itemHasChildren = !item->children().isEmpty();
    const bool itemClipsChildren = item->isFlag(Dw::ItemClipsChildren);
    const bool exposedIntersects = exposedRegion.isIntersects(sceneBounding.toRect());
    bool savedState = false;

    // draw child before parent
    bool drawChild = true;
    int childIdx = 0;
    int childNum = item->children().size();
    if (childNum > 0 && item->children().at(0)->zValue() < 0.0)
    {
        if (itemHasChildren && (!itemClipsChildren || exposedIntersects))
        {
            // if child was fully cliped by parent, break
            if((itemClipsChildrenToShape || itemClipsChildren) && !exposedIntersects)
            {
                drawChild = false;
                goto DRAW_SELF;
            }
            // children sort as asdencing order
            item->ensureSortedChildren();

            if (itemClipsChildrenToShape)
            {
                if (!savedState)
                {
                    savedState = true;
                    painter->save();
                }
                painter->setWorldTransform(transformToView);
                if (item->clipRegion())
                {
                    painter->setClip(item->clipRegion(), Dw::IntersectClip);
                }
                else
                {
                    painter->setClipRect(item->clipRect().toRect(), Dw::IntersectClip);
                }
            }
            else
            {
                if (savedState)
                {
                    savedState = false;
                    painter->restrore();
                }
                painter->setWorldTransform(transformToView);
            }

            for (; childIdx < childNum; ++childIdx)
            {
                DwGraphicsItem *child = item->children().at(childIdx);
                if (child->zValue() > 0.0)
                {
                    break;
                }
                drawSubtreeRecursive(child, painter, viewTransform, 
                    exposedRegion, opacity, enabled);
            }

            notifyListener(Dw::SceneNotifyAfterDrawChild, item);
        }
    }
    if (savedState)
    {
        painter->restrore();
        savedState = false;
    }

DRAW_SELF:
    if (drawItem)
    {
        if (itemClipsToShape)
        {
            if (!savedState)
            {
                savedState = true;
                painter->save();
            }

            painter->setWorldTransform(transformToView);
            if (item->clipRegion())
            {
                painter->setClip(item->clipRegion(), Dw::IntersectClip);
            }
            else
            {
                painter->setClipRect(item->clipRect().toRect(), Dw::IntersectClip);
            }
        }
        else
        {
            if (savedState)
            {
                savedState = false;
                painter->restrore();
            }
            painter->setWorldTransform(transformToView);
        }

        if (dwFuzzyCompare(opacity, 1.0) && enabled)
        {
            // TOOD: option
            notifyListener(Dw::SceneNotifyDrawItem, item);
            item->paint(painter, NULL);
            notifyListener(Dw::SceneNotifyAfterDrawitem, item);
        }
        else
        {
            // TODO : ADD EFFECT SUPPORT, INCLUDE THE EFFECT BOUNDING RECT
            // In This We Can do shader piexl algorithm(Effect)
            DwSize size = item->boundingRect().size().toSize();
            DwCachedCanvas cachedCavas(size);
            DwPainter tempPainter(&cachedCavas);
            cachedCavas.beginePaint(DwRegion());
            notifyListener(Dw::SceneNotifyDrawItem, item);
            item->paint(&tempPainter, NULL);
            notifyListener(Dw::SceneNotifyAfterDrawitem, item);
            cachedCavas.endPaint();

            cachedCavas.setOpacity(opacity);
            cachedCavas.setEnabled(enabled);
            DwCachedCanvas::paintCache(painter, &cachedCavas);
        }
    }
    if (savedState)
    {
        painter->restrore();
        savedState = false;
    }

    if (drawChild && itemHasChildren && (!itemClipsChildren || exposedIntersects))
    {
        // children sort as asdencing order
        item->ensureSortedChildren();

        if (itemClipsChildrenToShape)
        {
            if (!savedState)
            {
                savedState = true;
                painter->save();
            }
            painter->setWorldTransform(transformToView);
            if (item->clipRegion())
            {
                painter->setClip(item->clipRegion(), Dw::IntersectClip);
            }
            else
            {
                painter->setClipRect(item->clipRect().toRect(), Dw::IntersectClip);
            }
        }
        else
        {
            if (savedState)
            {
                savedState = false;
                painter->restrore();
            }
            painter->setWorldTransform(transformToView);
        }

        int childNum = item->children().size();
        for (childIdx; childIdx < childNum; ++childIdx)
        {
            drawSubtreeRecursive(item->children().at(childIdx), painter, viewTransform, 
                exposedRegion, opacity, enabled);
        }

        notifyListener(Dw::SceneNotifyAfterDrawChild, item);
    }

    if (savedState)
    {
        painter->restrore();
        savedState = false;
    }
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwGraphicsItem *> DwGraphicsScene::items() const
{
    DW_ASSERT(m_index);
    return m_index->items(Dw::DescendingOrder);
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwGraphicsItem *> DwGraphicsScene::items(const DwPointF &pos, Dw::ItemSelectionMode mode,
        Dw::SortOrder order, const DwTransform &deviceTransform/* = DwTransform()*/) const
{
    DW_ASSERT(m_index);
    DW_UNUSED(deviceTransform);
    DW_UNUSED(order);
    DW_UNUSED(mode);
    return m_index->items(pos, mode, order);
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwGraphicsItem *> DwGraphicsScene::items(const DwRectF &rect, Dw::ItemSelectionMode mode,
        Dw::SortOrder order, const DwTransform &deviceTransform/* = DwTransform()*/) const
{
    DW_ASSERT(m_index);
    return m_index->items(rect, mode, order, deviceTransform);
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
DwVector<DwGraphicsItem *> DwGraphicsScene::collidingItems(const DwGraphicsItem *item,
        Dw::ItemSelectionMode mode/* = Dw::IntersectsItemShape*/) const
{
    DW_UNUSED(item);
    DW_UNUSED(mode);
    return DwVector<DwGraphicsItem *>();
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwGraphicsScene::itemAt(const DwPointF &pos, const DwTransform &deviceTransform) const
{
    DwVector<DwGraphicsItem *> itemsAtPoint = items(pos, Dw::IntersectsItemShape, Dw::DescendingOrder, deviceTransform);
    return itemsAtPoint.isEmpty() ? 0 : itemsAtPoint.front();
}

DwVector<DwGraphicsItem *> DwGraphicsScene::items(DwPoint &pos) const
{
    if (m_scenePoint == pos && m_cachedItemsValid)
    {
        return m_cachedItemsUnderMouse;
    }

    const_cast<DwGraphicsScene*>(this)->m_cachedItemsUnderMouse = items(DwPointF(pos), Dw::IntersectsItemShape, Dw::DescendingOrder);
    const_cast<DwGraphicsScene*>(this)->m_scenePoint = pos;
    const_cast<DwGraphicsScene*>(this)->m_cachedItemsValid = true;
    return m_cachedItemsUnderMouse;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSceneIndex *DwGraphicsScene::index() const
{
    return m_index;
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsScene::sendEvent(DwSceneEvent *evt) const
{
    dwGraphicsEventDispatch()->dispatchEvent(this, evt);

    return evt->isAccepted();
}

//------------------------------------------------------------------------------
/**
	NB! DO NOT SEND EVENT TO ITEM IN DESTRUCTOR
*/
bool DwGraphicsScene::sendEvent(const DwGraphicsItem *item, DwSceneEvent *evt) const
{
    DW_ASSERT(!item->m_inDestructor);

    evt->setInterestItem(item);

    dwGraphicsEventDispatch()->dispatchEvent(this, evt);

    return evt->isAccepted();
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsScene::lastHoverItem() const
{
    return itemOf(Dw::LastHoverItem);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsScene::focusItem() const
{
    return itemOf(Dw::FocusItem);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsScene::lastFocusItem() const
{
    return itemOf(Dw::LastFocusItem);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsScene::tabFirstFocus() const
{
    return itemOf(Dw::TabFirstFocus);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsScene::mouseGrabItem() const
{
    if (m_mouseGrabberItems.isEmpty())
    {
        return NULL;
    }
    return m_mouseGrabberItems.back();
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsScene::keyBoardGrabItem() const
{
    return itemOf(Dw::KeyBoardGraber);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::grabMouse(DwGraphicsItem *item, bool implicit/* = false*/)
{
    // Append to list of mouse grabber items, and send a mouse grab event.
    int index = m_mouseGrabberItems.indexOf(item);
    if (index != InvalidIndex)
    {
        if (m_mouseGrabberItems.back() == item)
        {
            DW_ASSERT(!implicit);
            if (!m_lastMouseGrabberItemHasImplicitMouseGrab)
            {
                DW_WARNING("DwGraphicsScene::grabMouse: already a mouse grabber");
            }
            else
            {
                // Upgrade to an explicit mouse grab
                m_lastMouseGrabberItemHasImplicitMouseGrab = false;
            }
        }
        else
        {
            DW_WARNING("DwGraphicsScene::grabMouse: already blocked by mouse grabber: %p");
        }
        return;
    }

    // Send ungrab event to the last grabber.
    if (!m_mouseGrabberItems.isEmpty())
    {
        DwGraphicsItem *last = m_mouseGrabberItems.back();
        if (m_lastMouseGrabberItemHasImplicitMouseGrab)
        {
            // Implicit mouse grab is immediately lost.
            last->ungrabMouse();
        }
        else
        {
            // Just send ungrab event to current grabber.
#if DW_ENABLE_EVENT_GRABER
            DwUngrabMouseEvent evt;
            sendEvent(last, &evt);
#endif
        }
    }

    m_mouseGrabberItems.append(item);
    m_lastMouseGrabberItemHasImplicitMouseGrab = implicit;

    // Send grab event to current grabber.
#if DW_ENABLE_EVENT_GRABER
    DwGrabMouseEvent evt;
    sendEvent(item, &evt);
#endif
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::ungrabMouse(DwGraphicsItem *item, bool itemIsDying/* = false*/)
{
    int index = m_mouseGrabberItems.indexOf(item);
    if (index == InvalidIndex)
    {
        DW_WARNING("QGraphicsItem::ungrabMouse: not a mouse grabber");
        return;
    }

    if (item != m_mouseGrabberItems.back())
    {
        // Recursively ungrab the next mouse grabber until we reach this item
        // to ensure state consistency.
        ungrabMouse(m_mouseGrabberItems.at(index + 1), itemIsDying);
    }

    // TODO: xx
    //if (!popupWidgets.isEmpty() && item == popupWidgets.last())
    //{
    //	// If the item is a popup, go via removePopup to ensure state
    //	// consistency and that it gets hidden correctly - beware that
    //	// removePopup() reenters this function to continue removing the grab.
    //	removePopup((QGraphicsWidget *)item, itemIsDying);
    //	return;
    //}

    // Send notification about mouse ungrab.
    if (!itemIsDying)
    {
#if DW_ENABLE_EVENT_GRABER
        DwUngrabMouseEvent evt;
        sendEvent(item, &evt);
#endif
    }

    // Remove the item from the list of grabbers. Whenever this happens, we
    // reset the implicitGrab (there can be only ever be one implicit grabber
    // in a scene, and it is always the latest grabber; if the implicit grab
    // is lost, it is not automatically regained.
    m_mouseGrabberItems.pop_back();
    m_lastMouseGrabberItemHasImplicitMouseGrab = false;

    // Send notification about mouse regrab. ### It's unfortunate that all the
    // items get a GrabMouse event, but this is a rare case with a simple
    // implementation and it does ensure a consistent state.
    if (!itemIsDying && !m_mouseGrabberItems.isEmpty())
    {
#if DW_ENABLE_EVENT_GRABER
        DwGraphicsItem *last = m_mouseGrabberItems.back();
        DwGrabMouseEvent evt;
        sendEvent(last, &evt);
#endif
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::clearImplicitMouseGrabber()
{
    if (!m_mouseGrabberItems.isEmpty())
    {
        m_lastMouseGrabberItem = m_mouseGrabberItems.back();
        if (m_lastMouseGrabberItemHasImplicitMouseGrab)
        {
            m_mouseGrabberItems.back()->ungrabMouse();
        }
    }
    else
    {
        m_lastMouseGrabberItem = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::grabKeyboard(DwGraphicsItem *item)
{
    DwGraphicsItem* lastKeyboardGrabItem = keyBoardGrabItem();
    if (lastKeyboardGrabItem == item)
    {
        return;
    }

    if (lastKeyboardGrabItem)
    {
        setIndicator(Dw::KeyBoardGraber, item);
#if DW_ENABLE_EVENT_GRABER
        DwUngrabKeyboardEvent evt;
        sendEvent(lastKeyboardGrabItem, &evt);
#endif
    }

    if (item)
    {
        setIndicator(Dw::KeyBoardGraber, item);
#if DW_ENABLE_EVENT_GRABER
        DwGrabKeyboardEvent evt;
        sendEvent(item, &evt);
#endif
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::unGrabKeyboard()
{
    grabKeyboard(NULL);
}

//------------------------------------------------------------------------------
/**
*/
DwSceneEventHandler *DwGraphicsScene::eventHandler() const
{
    return m_eventHandler;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::setEventHandler(DwSceneEventHandler *handler)
{
    if (m_eventHandler == handler)
    {
        return;
    }

    if (m_eventHandler)
    {
        delete m_eventHandler;
    }
    m_eventHandler = handler;
}

//------------------------------------------------------------------------------
/**
*/
const DwVector<DwGraphicsView *>& DwGraphicsScene::views() const
{
    return m_views;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsScene::itemOf(Dw::SceneIndicator indicator) const
{
    DW_ASSERT(indicator >= 0 && indicator < _countof(m_indicator));
    return m_indicator[indicator];
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::setIndicator(Dw::SceneIndicator indicator, DwGraphicsItem* item)
{
    DW_ASSERT(indicator >= 0 && indicator < _countof(m_indicator));
    DW_ASSERT(!item || ( item && (item->scene() == this)) );
    m_indicator[indicator] = item;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::setFocusItem(DwGraphicsItem *item, Dw::FocusReason reason)
{
    DW_UNUSED(item);
    DW_UNUSED(reason);

    bool continueDispatch = true;
    if (item)
    {
        item = collectFocus_helper(item, reason, &continueDispatch);
    }

    if (continueDispatch)
    {
        setFocusItem_helper(item, reason);
    }
}

//------------------------------------------------------------------------------
/**
	TODO: xx, Such As Focus Proxy, Such As FocusScopeItem, Such As BreakFocus
*/
DwGraphicsItem * DwGraphicsScene::collectFocus_helper(DwGraphicsItem *item, Dw::FocusReason reason, bool* continuation/* = NULL*/)
{
    DW_UNUSED(reason);
    if (continuation)
    {
        *continuation = true;
    }

    // focus proxy
    DwGraphicsItem* subFocus = item;
    while (subFocus->focusProxy())
    {
        subFocus = subFocus->focusProxy();
    }
    return subFocus;
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwGraphicsScene::setFocusItem_helper(DwGraphicsItem *item, Dw::FocusReason reason)
{
    DW_UNUSED(reason);
    if (item == focusItem())
    {
        return;
    }

    // Clear focus if asked to set focus on something that can't
    // accept input focus.
    if (item && (!(item->flags() & Dw::ItemIsFocusable)
                 || !item->isVisible() || !item->isEnabled()))
    {
        item = 0;
    }

    DwGraphicsItem *curFocus = focusItem();
    if(curFocus)
    {
        m_hasFocus = false;

        setIndicator(Dw::LastFocusItem, curFocus);
        setIndicator(Dw::FocusItem, NULL);

#if DW_ENABLE_EVENT_FOCUS
        // send focus out event to item
        DwFocusOutEvent focusOutEvt;
        focusOutEvt.m_reason = reason;
        sendEvent(curFocus, &focusOutEvt);
#endif

        if (curFocus && (curFocus->isFlag(Dw::ItemAcceptsInputMethod)))
        {
            // TODO: xx
            // Reset any visible preedit text
            //DwInputMethodEvent imEvent;
            //m_view->sendEvent(curFocus, &imEvent);
        }
    }

    // This handles the case that the item has been removed from the
    // scene in response to the FocusOut event.
    if (item && item->scene() != this)
        item = 0;

    if (item)
    {
        m_hasFocus = true;
        setIndicator(Dw::FocusItem, item);

#if DW_ENABLE_EVENT_FOCUS
        // send focus in event to item
        DwFocusInEvent focusInEvt;
        focusInEvt.m_reason = reason;
        sendEvent(item, &focusInEvt);
#endif
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::setFocus(Dw::FocusReason focusReason/* = Dw::OtherFocusReason*/)
{
    if (hasFocus()) //TODO: Active
        return;

    switch (focusReason)
    {
    case Dw::TabFocusReason:
        focusNextPrevChild(true);
        break;
    case Dw::BacktabFocusReason:
        focusNextPrevChild(false);
        break;
    default:
        if (lastFocusItem())
        {
            setFocusItem(lastFocusItem(), focusReason);
        }
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::clearFocus()
{
    if (!hasFocus()) //TODO: Active
        return;

    setFocusItem(NULL, Dw::OtherFocusReason);
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsScene::focusNextPrevChild(bool next)
{
    DwGraphicsItem *item = focusItem();
    if (!item)
    {
        if (lastFocusItem())
        {
            // Restore focus to the last focusable non-widget item that had focus.
            setFocusItem(lastFocusItem(), next ? Dw::TabFocusReason : Dw::BacktabFocusReason);
            return true;
        }
    }
    DwGraphicsItem *tabFocusFirst = tabFirstFocus();
    if (!tabFocusFirst)
    {
        // No widgets...
        return false;
    }

    // The item must be a widget.
    DwGraphicsItem *widget = 0;
    if (!item)
    {
        widget = next ? tabFocusFirst : tabFocusFirst->focusPrev();
    }
    else
    {
        widget = next ? item->focusNext() : item->focusPrev();
        // ??? WE REALLY NEED TO STOP THE TAB CIRCURS
        // the next is the first, or the pre is the last
        // if ((next && widget == tabFocusFirst) || (!next && widget == tabFocusFirst->focusPrev()))
            // return false;
    }
    DwGraphicsItem *widgetThatHadFocus = widget;

    // Run around the focus chain until we find a widget that can take tab focus.
    do
    {
        // the next is null
        if (!widget)
            return false;
        
        if (widget->isFlag(Dw::ItemIsFocusable)
                && widget->isEnabled() && widget->isVisibleTo(0)
                && (widget->focusPolicy() & Dw::TabFocus))
                // && (!item || item->isAncestorOf(widget)))
        {
            setFocusItem(widget, next ? Dw::TabFocusReason : Dw::BacktabFocusReason);
            return true;
        }
        widget = next ? widget->focusNext() : widget->focusPrev();
        // ??? WE REALLY NEED TO STOP THE TAB CIRCURS
        // the next is the first, or the pre is the last
        // if ((next && widget == tabFocusFirst) || (!next && widget == tabFocusFirst->focusPrev()))
            // return false;
    }
    while (widget != widgetThatHadFocus);

    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::onItemChange(DwGraphicsItem* item, Dw::GraphicsItemChange change, const DwVariant& newOne, const DwVariant& oldOne)
{
    if (m_index)
    {
        m_index->itemChange(item, change, newOne, oldOne);
    }
}

void DwGraphicsScene::invalidate(const DwRectF &rect/* = DwRectF()*/, Dw::SceneLayer layers/* = Dw::AllLayers*/)
{
    for (int i=0; i<m_views.size(); ++i)
    {
        DwGraphicsView * view = m_views.at(i);
        view->invalidateScene(rect, layers);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::update(const DwRectF &rect/* = DwRectF()*/)
{
    if(rect.isEmpty() || m_updateAll)
    {
        return;
    }

    if (rect.contains(sceneRect()))
    {
        m_updateAll = true;
        return;
    }
    m_updatedRects.append(rect);

    if (!m_updateTimer.isActive())
    {
        m_updateTimer.start(16);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::advance()
{
    ;//TODO: xx
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::markDirty(DwGraphicsItem *item, const DwRectF& rect)
{
    DW_UNUSED(rect);
    m_dirtyItems.append(item);

    if (!m_updateTimer.isActive())
    {
        m_updateTimer.start(16);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::resetDirty(DwGraphicsItem *item)
{
    item->m_dirty = 0;
    item->m_dirtyPrepareGemotry = 0;
    item->m_dirtyUpdate = 0;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::onUpdateTimerEvent()
{
    if (m_updateAll)
    {
        DwRectF updateRect = sceneRect();
        m_updatedRects.append(updateRect);
    }

    m_updateAll = false;
    m_updateTimer.stop();

    if (m_dirtyItems.size() > 0)
    {
        for (int i=0; i<m_dirtyItems.size(); ++i)
        {
            resetDirty(m_dirtyItems.at(i));
        }

        m_dirtyItems.clear();
    }

    if (m_updatedRects.size() > 0)
    {
        DwVector<DwRectF> updatedRects = m_updatedRects;
        m_updatedRects.clear();

        for (int i=0; i<m_views.size(); ++i)
        {
            DwGraphicsView * view = m_views.at(i);
            view->updateScene(updatedRects);
        }
    }

}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::addListener(DwGraphicsSceneListener* listener)
{
    DW_ASSERT(listener);
    listener->addDepend(this);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::removeListener(DwGraphicsSceneListener* listener)
{
    DW_ASSERT(listener);
    listener->remDepend(this);
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsScene::hasListener(DwGraphicsSceneListener* listener) const
{
    return m_listeners.indexOf(listener) != InvalidIndex;
}

//------------------------------------------------------------------------------
/**
*/
int  DwGraphicsScene::listenerSize() const
{
    return m_listeners.size();
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSceneListener* DwGraphicsScene::listenerAt(int index) const
{
    DW_ASSERT(index>=0 && index<m_listeners.size());
    return m_listeners.at(index);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::removeAllListeners()
{
    DwVector<DwGraphicsSceneListener *> listeners = m_listeners;
    for (int i = 0; i < listeners.size(); ++i)
    {
        removeListener(listeners.at(i));
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::removeItemHelper(DwGraphicsItem *item)
{
    DW_ASSERT(item);
    // clear search restult
    if(m_cachedItemsValid && !m_cachedItemsUnderMouse.isEmpty())
    {
        m_cachedItemsUnderMouse.removeOne(item);
    }

    // clear mouse graber
    if (!m_mouseGrabberItems.isEmpty())
    {
        m_mouseGrabberItems.removeOne(item);
    }
    if (m_lastMouseGrabberItem == item)
    {
        m_lastMouseGrabberItem = NULL;
    }

    // remove dirty items
    if (!m_dirtyItems.isEmpty())
    {
        resetDirty(item);
        m_dirtyItems.removeOne(item);
    }

    // Notify before truely removed
    notifyListener(Dw::SceneNotifyRemoveItem, item);

    // Remove from index
    if(item->m_inDestructor)
    {
        m_index->deleteItem(item);
    }
    else
    {
        m_index->removeItem(item);
    }

    // Remove the item from scene
    item->m_scene = 0;
    notifyListener(Dw::SceneNotifyAfterRemove, item);

    // We need to remove all children first because they might use their parent
    // attributes (e.g. sceneTransform).
    // Also be sure not in destructor, as the graphicsitem's deconstructor will
    // delete item recursively
    if (!item->m_inDestructor)
    {
        // Remove all children recursively
        for (int i = 0; i < item->children().size(); ++i)
        {
            removeItem(item->children().at(i));
        }
    }

    if (DwGraphicsItem* parent = item->parentItem())
    {
        // Detach this item from its parent if the parent's scene is different
        if (parent->scene() && parent->scene() == this)
        {
            //DW_ASSERT(parent->scene() == this && "Parent item's scene is different from this item's scene");
            item->setParentItem(NULL);
        }
    }
    else
    {
        // remove frome list of toplevels if this item is a toplevel.
        unRegisterTopLevelItem(item);
    }
}

//------------------------------------------------------------------------------
/**
	NB! do not called user
*/
void DwGraphicsScene::registerTopLevelItem(DwGraphicsItem* item)
{
    DW_ASSERT(m_topLevelItems.indexOf(item) == InvalidIndex);
    notifyListener(Dw::SceneNotifyRegTopLevel, item);
    m_dirtyTopLevelItems = 1;
    item->m_itemSiblingIndex = m_maxSiblingIndex++;
    m_topLevelItems.append(item);
}

//------------------------------------------------------------------------------
/**
	NB! do not called user
*/
void DwGraphicsScene::unRegisterTopLevelItem(DwGraphicsItem* item)
{
    DW_ASSERT(m_topLevelItems.indexOf(item) != InvalidIndex);
    notifyListener(Dw::SceneNotifyUnRegTopLevel, item);
    m_dirtyTopLevelItems = 1;
    m_topLevelItems.removeOne(item);
}

//------------------------------------------------------------------------------
/**
	sort the top items as the descendingorder order
*/
void DwGraphicsScene::ensureSortedTopLevelItem()
{
    if (m_dirtyTopLevelItems && m_topLevelItems.size() > 0)
    {
        ensureSequentialTopLevelSiblingIndexes();

        DwGraphicsUtil::sortSiblingsItems(&m_topLevelItems, Dw::DescendingOrder);
        m_maxSiblingIndex = m_topLevelItems.size();
        m_dirtyTopLevelItems = 0;
    }
}

//------------------------------------------------------------------------------
/**
	sort the top items as the descendingorder order
*/
void DwGraphicsScene::ensureSequentialTopLevelSiblingIndexes()
{
    if(m_topLevelItems.size() <= 0) return;
    DwGraphicsUtil::sortChildBySiblingIndex(&m_topLevelItems, Dw::DescendingOrder);
    for (int index=0; index<m_topLevelItems.size(); ++index)
    {
        m_topLevelItems.at(index)->m_itemSiblingIndex = m_topLevelItems.size() - 1 - index;
    }
}

//------------------------------------------------------------------------------
/**
    NB! ONLY SHOULD BE CALLED BY LISTENER IN ADDPEND() AND REMDEPEND()
*/
void DwGraphicsScene::attachListener(DwGraphicsSceneListener *listener)
{
    DW_ASSERT(listener);
    DW_ASSERT(m_listeners.indexOf(listener) == InvalidIndex);

    listener->onAdded(this);
    m_listeners.append(listener);
}

//------------------------------------------------------------------------------
/**
	NB! ONLY SHOULD BE CALLED BY LISTENER IN ADDPEND() AND REMDEPEND()
*/
void DwGraphicsScene::detachListener(DwGraphicsSceneListener *listener)
{
    DW_ASSERT(listener);
    DW_ASSERT(m_listeners.indexOf(listener) != InvalidIndex);

    listener->onRemoved(this);
    m_listeners.removeOne(listener);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsScene::notifyListener(Dw::GraphicsSceneNotify notify, DwGraphicsItem* item)
{
    for (int i=0; i<m_listeners.size(); ++i)
    {
        m_listeners.at(i)->sceneNotify(notify, this, item);
    }

	// we do the debug
#ifdef _DEBUG
	if (m_debugDraw && notify == Dw::SceneNotifyAfterDrawitem && item)
	{
		m_painter->drawRect(item->boundingRect().adjusted(0, 0, -1, -1), DwColor(255, 0, 0, 255));
	}
#endif
}

//------------------------------------------------------------------------------
/**
*/
DwPainter* DwGraphicsScene::curPainter() const
{
    return m_painter;
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
void DwGraphicsScene::drawBackground(DwPainter *painter, const DwRectF &rect)
{
    DW_UNUSED(painter);
    DW_UNUSED(rect);
}


//------------------------------------------------------------------------------
/**
    TODO: xx
*/
void DwGraphicsScene::drawForeground(DwPainter *painter, const DwRectF &rect)
{
    DW_UNUSED(painter);
    DW_UNUSED(rect);
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
void DwGraphicsScene::drawItems(DwPainter *painter, const DwVector<DwGraphicsItem *> inItems,
                                const DwStyleOptionGraphicsItem options[])
{
    DW_UNUSED(painter);
    DW_UNUSED(inItems);
    DW_UNUSED(options);
}