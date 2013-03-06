#pragma once
#include "dwnamespace.h"
#include "dwcore/dwvector.h"
#include "dwcore/dwrect.h"
#include "dwcore/dwvariant.h"
#include "dwgraphics/dwtransform.h"
#include "dwgraphics/dwregion.h"
#include "dwcore.h"
#include "dwgui/dwtimer.h"
#include "dwcore/sigslot.h"
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwrttiobjectptr.h"

//------------------------------------------------------------------------------
class DwPainter;
class DwGraphicsItem;
class DwGraphicsSceneIndex;
class DwGraphicsSceneListener;
class DwGraphicsView;
class DwSceneEvent;
class DwStyleOptionGraphicsItem;
class DwSceneEventHandler;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsScene : public DwObject, public sigslot::has_slots , public DwRttiObject
{
    DW_DECLARE_CLASS(DwGraphicsScene);
public:
    explicit DwGraphicsScene(DwObject *parent = NULL);
    explicit DwGraphicsScene(DwRtti *indexClass, DwObject *parent = NULL);
    virtual ~DwGraphicsScene();

    void addItem(DwGraphicsItem *item);
    void removeItem(DwGraphicsItem *item);

    const DwVector<DwGraphicsItem*>& topLevelItems() const;

    // Scene Rect Stuffs - similarity to scene's camera settings
    DwRectF sceneRect() const;
    inline DwReal width() const;
    inline DwReal height() const;
    void setSceneRect(const DwRectF &rect);
    inline void setSceneRect(DwReal x, DwReal y, DwReal w, DwReal h);

    DwRectF itemsBoundingRect() const;

    // Render stuffs
    void render(DwPainter *painter,
                const DwRectF &target = DwRectF(), const DwRectF &source = DwRectF(),
                Dw::AspectRatioMode aspectRatioMode = Dw::KeepAspectRatio);

    void drawItems(DwPainter *painter, const DwTransform *const viewTransform,
                   const DwRegion &exposedRegion);

    // Scene Index Stuffs
    DwVector<DwGraphicsItem *> items() const;
    DwVector<DwGraphicsItem *> items(const DwPointF &pos, Dw::ItemSelectionMode mode, Dw::SortOrder order, const DwTransform &deviceTransform = DwTransform()) const;
    DwVector<DwGraphicsItem *> items(const DwRectF &rect, Dw::ItemSelectionMode mode, Dw::SortOrder order, const DwTransform &deviceTransform = DwTransform()) const;
    DwVector<DwGraphicsItem *> collidingItems(const DwGraphicsItem *item, Dw::ItemSelectionMode mode = Dw::IntersectsItemShape) const;
    DwGraphicsItem *itemAt(const DwPointF &pos, const DwTransform &deviceTransform = DwTransform()) const;
    DwGraphicsSceneIndex* index() const;
    DwVector<DwGraphicsItem *> items(DwPoint &pos) const;// Will Cache Result, IntersetMode, Descending Order
    inline void clearSearchCache();

    // Scene Event Stuffs
    bool sendEvent(DwSceneEvent *evt) const;
    bool sendEvent(const DwGraphicsItem *item, DwSceneEvent *evt) const;
    DwSceneEventHandler *eventHandler() const;
    void setEventHandler(DwSceneEventHandler *handler);

    // Hover Stuffs
    DwGraphicsItem* lastHoverItem() const;

    // Grab Stuffs
    DwGraphicsItem* keyBoardGrabItem() const;	// TODO: xx
    void grabKeyboard(DwGraphicsItem *item);	// TODO: xx
    void unGrabKeyboard();	// TODO: xx

    DwGraphicsItem* mouseGrabItem() const;
    void grabMouse(DwGraphicsItem *item, bool implicit = false);
    void ungrabMouse(DwGraphicsItem *item, bool itemIsDying = false);
    void clearMouseGrabber();
    void clearImplicitMouseGrabber();

    // View Stuffs
    const DwVector<DwGraphicsView *>& views() const;

    // SceneIndicator Stuffs
    DwGraphicsItem* itemOf(Dw::SceneIndicator indicator) const;
    void setIndicator(Dw::SceneIndicator indicator, DwGraphicsItem* item);

    // Listener Stuffs
    void addListener(DwGraphicsSceneListener* listener);
    void removeListener(DwGraphicsSceneListener* listener);
    bool hasListener(DwGraphicsSceneListener* listener) const;
    int  listenerSize() const;
    DwGraphicsSceneListener* listenerAt(int index) const;
    void removeAllListeners();
    void notifyListener(Dw::GraphicsSceneNotify notify, DwGraphicsItem* item);

    // Procedure variable
    DwPainter* curPainter() const;

    // Focus Stuffs
    DwGraphicsItem* focusItem() const;
    DwGraphicsItem* lastFocusItem() const;
    DwGraphicsItem* tabFirstFocus() const;
    inline bool hasFocus() const;
    inline bool stickyFocus() const;
    inline void setStickyFocus(bool sticked);
    virtual void setFocusItem(DwGraphicsItem *item, Dw::FocusReason reason);
    virtual void setFocus(Dw::FocusReason focusReason = Dw::OtherFocusReason);
    virtual void clearFocus();
    virtual bool focusNextPrevChild(bool next);

    // flags
    inline void clearTopItemsWhenDestruct(bool b);
    inline bool isClearTopItemWhenDestruct() const;

    // Item Changes Stuffs
    void onItemChange(DwGraphicsItem* item, Dw::GraphicsItemChange change, const DwVariant& newOne, const DwVariant& oldOne);

    // Dirty Rectange Stuffs
    void invalidate(const DwRectF &rect = DwRectF(), Dw::SceneLayer layers = Dw::AllLayers) ;
    inline void invalidate(DwReal x, DwReal y, DwReal w, DwReal h, Dw::SceneLayer layers = Dw::AllLayers);
    void update(const DwRectF &rect = DwRectF());
    inline void update(DwReal x, DwReal y, DwReal w, DwReal h);
    void advance();
    void markDirty(DwGraphicsItem *item, const DwRectF& rect);
    void resetDirty(DwGraphicsItem *item);

    void onUpdateTimerEvent();

    void ensureSortedTopLevelItem();
    void ensureSequentialTopLevelSiblingIndexes();

	// Debug Stuffs
	void setDebugDraw(bool b);
	bool debugDraw() const;

protected:
    void removeItemHelper(DwGraphicsItem *item);
    void registerTopLevelItem(DwGraphicsItem* item);
    void unRegisterTopLevelItem(DwGraphicsItem* item);


    // Listener Stuffs
    void attachListener(DwGraphicsSceneListener *listener);
    void detachListener(DwGraphicsSceneListener *listener);

    // Drawing Stuffs
    virtual void drawBackground(DwPainter *painter, const DwRectF &rect);
    virtual void drawForeground(DwPainter *painter, const DwRectF &rect);
    virtual void drawItems(DwPainter *painter, const DwVector<DwGraphicsItem *> inItems,
                           const DwStyleOptionGraphicsItem options[]);

    void drawSubtreeRecursive(DwGraphicsItem *item, DwPainter *painter, const DwTransform *const,
                              const DwRegion &exposedRegion, DwReal parentOpacity = DwReal(1.0), bool parentEnabled = true);
    void draw(DwGraphicsItem *, DwPainter *, const DwTransform *const, const DwTransform &,
              const DwRegion &, DwReal, const DwTransform *const, bool, bool);

    // Focus Stuffs
    DwGraphicsItem * collectFocus_helper(DwGraphicsItem *item, Dw::FocusReason reason, bool* continuation = NULL);
    void setFocusItem_helper(DwGraphicsItem *item, Dw::FocusReason reason);

private:
    friend class DwGraphicsItem;
    friend class DwGraphicsView;
    friend class DwGraphicsSceneIndex;
    friend class DwGraphicsSceneListener;

    DwGraphicsSceneIndex* m_index;
    DwGraphicsSceneListener* m_listener;
    DwVector<DwGraphicsItem*> m_topLevelItems;
    int m_maxSiblingIndex;

    DwVector<DwGraphicsView*> m_views;

    DwSceneEventHandler* m_eventHandler;
    DwRttiObjectPtr<DwGraphicsItem> m_indicator[Dw::SceneIndicatorMax];

    DwRectF m_sceneRect;
    uint32_t m_hasSceneRect : 1;
    uint32_t m_dirtyTopLevelItems : 1;
    uint32_t m_stickyFocus : 1;
    uint32_t m_hasFocus : 1;
    uint32_t m_cachedItemsValid : 1;
    uint32_t m_lastMouseGrabberItemHasImplicitMouseGrab : 1;
    uint32_t m_clearTopLevelItemsWhenDestruct : 1;
    uint32_t m_updateAll : 1;
    uint32_t m_calledEmitUpdated : 1;
	uint32_t m_debugDraw : 1;

    DwTimer m_updateTimer;

    DwVector<DwGraphicsSceneListener *> m_listeners;

    DwPainter *m_painter;

    DwPointF m_scenePoint;
    DwVector<DwGraphicsItem *> m_cachedItemsUnderMouse;
    DwVector<DwGraphicsItem *> m_mouseGrabberItems;
    DwGraphicsItem *m_lastMouseGrabberItem;

    DwVector<DwRectF> m_updatedRects;
    DwVector<DwGraphicsItem *> m_dirtyItems;
};// end of DwGraphicsScene

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwGraphicsScene::width() const
{
    return sceneRect().width();
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwGraphicsScene::height() const
{
    return sceneRect().height();
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsScene::setSceneRect(DwReal x, DwReal y, DwReal w, DwReal h)
{
    setSceneRect(DwRectF(x, y, w, h));
}

//------------------------------------------------------------------------------
/**
*/inline void DwGraphicsScene::clearSearchCache()
{
    m_cachedItemsValid = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsScene::hasFocus() const
{
    return m_hasFocus;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsScene::stickyFocus() const
{
    return m_stickyFocus;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsScene::setStickyFocus(bool sticked)
{
    m_stickyFocus = sticked;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsScene::clearTopItemsWhenDestruct(bool b)
{
    m_clearTopLevelItemsWhenDestruct = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsScene::isClearTopItemWhenDestruct() const
{
    return m_clearTopLevelItemsWhenDestruct;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsScene::invalidate(DwReal x, DwReal y, DwReal w, DwReal h, Dw::SceneLayer layers/* = Dw::AllLayers*/)
{
    invalidate(DwRectF(x, y, w, h), layers);
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsScene::update(DwReal x, DwReal y, DwReal w, DwReal h)
{
    update(DwRectF(x, y, w, h));
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsScene::setDebugDraw(bool b)
{
    m_debugDraw = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsScene::debugDraw() const
{
    return m_debugDraw;
}