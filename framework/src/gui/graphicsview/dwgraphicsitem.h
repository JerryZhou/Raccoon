#pragma once
#include "dwnamespace.h"
#include "dwcore/dwstring.h"
#include "dwcore/dwvector.h"
#include "dwcore/dwrect.h"
#include "dwcore/dwvariant.h"
#include "dwgraphics/dwtransform.h"
#include "dwgui/dwrtti.h"
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwpropertys.h"
#include "dwgui/dwringlogcase.h"

//------------------------------------------------------------------------------
class DwStyleOptionGraphicsItem;
class DwGraphicsScene;
class DwGraphicsView;
class DwPainter;
class DwGraphicsItem;
class DwItemEventHandler;
class DwGraphicsItemListener;
class DwAnchors;
class DwAbstractAnimation;
class DwRegion;
class DwSceneEvent;
typedef DwVector<DwGraphicsItem *> DwGraphicsItemVector;
class DwGraphicsItemHook;
class DwEventSignalDispatch;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsItem : public DwRttiObject
{
    DW_DECLARE_CLASS(DwGraphicsItem);
public:
    explicit DwGraphicsItem(DwGraphicsItem *parent = NULL, DwGraphicsScene *s = NULL);
    virtual ~DwGraphicsItem();

    DwGraphicsScene* scene() const;

    DwGraphicsItem* parentItem() const;
    void setParentItem(DwGraphicsItem* parent);

    DwGraphicsItem* topLevelItem() const;
    bool isAncestorOf(const DwGraphicsItem *child) const;
    DwGraphicsItem *commonAncestorItem(const DwGraphicsItem *other) const;

    inline const DwVector<DwGraphicsItem*>& children() const;

    // Depth stuffs
    int depth() const;
    void invalidateDepthRecursively();
    void resolveDepth();

    // Hit test(NB! TO DISCRIMINATE DIFFERENT COORDINATE SYSTEM)
    virtual DwRectF boundingRect() const;
    void setBoundingRect(const DwRectF &bounding);
    DwRectF childrenBoundingRect() const;
    DwRectF sceneBoundingRect() const;
    DwRectF sceneEffectiveBoundingRect() const;
    DwRectF viewRect(const DwGraphicsView *view) const;
    DwRectF deviceRect(const DwGraphicsView *view) const;
    DwRectF screenRect(const DwGraphicsView *view) const;

    // Clip Stuffs
    virtual DwRectF clipRect() const;
    virtual void setClipRect(const DwRectF&);

    // Drawing
    virtual void paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option);// = 0;
    void setOpacity(DwReal opaque);
    DwReal opacity() const;

    // Coordinate mapping
    DwPointF mapToParent(const DwPointF &point) const;
    DwPointF mapToScene(const DwPointF &point) const;
    DwRectF mapToParent(const DwRectF &r) const;
    DwRectF mapToScene(const DwRectF &r) const;
    
    DwPointF mapFromScene(const DwPointF &point) const;
    DwPoint mapFromScene(const DwPoint &point) const;
    DwPointF mapFromParent(const DwPointF &point) const;
    DwPoint mapFromParent(const DwPoint &point) const;

    DwRectF mapFromScene(const DwRectF &r) const;
    DwRectF mapFromParent(const DwRectF &r) const;

    // Visible things
    inline bool isVisible() const;
    bool isVisibleTo(const DwGraphicsItem *parent) const;
    void setVisible(bool visible);
    inline void hide();
    inline void show();

    // Enable things
    inline bool isEnabled() const;
    void setEnabled(bool enabled) ;

    // Positioning stuffs(parent coordinate system)
    DwPointF pos() const;
    inline DwReal x() const;
    void setX(DwReal x);
    inline DwReal y() const;
    void setY(DwReal y);
    DwPointF scenePos() const;
    void setPos(const DwPointF &pos);
    inline void setPos(DwReal x, DwReal y);
    inline void moveBy(DwReal dx, DwReal dy);

    // Geometry stuffs in parent coordinate system
    DwRectF rect() const;
    inline DwReal width() const;
    inline DwReal height() const;
    void setWidth(DwReal w);
    void setHeight(DwReal h);
    DwSizeF size() const;
    void setSize(DwReal w, DwReal h);
    void setSize(DwSizeF size);
    inline void setRect(DwReal x, DwReal y, DwReal width, DwReal height);
    void setRect(const DwRectF &rect);
    void setGeometry(const DwRectF &rect);

    // Local transformation
    const DwTransform &transform() const;
    const DwTransform &sceneTransform() const;
    void resetTransform();
    void setTransform(const DwTransform &matrix, bool combine = false);

    // Stacking order
    DwReal zValue() const;
    void setZValue(DwReal z);
    DwReal sceneZValue() const;
    void resolveSceneZValue();
    void ensureSortedChildren();
    void ensureSequentialSiblingIndex();
    int siblingIndex() const;
    void stackBefore(const DwGraphicsItem *sibling);

    // misc
    inline DwString name() const;
    inline void setName(const DwString &name);

    // a unique index
    inline uint32_t id() const;

    DwGraphicsItem *findChild(const DwString &id) const;
    virtual const DwString fullId() const;

    // Item property change stuffs
    virtual DwVariant itemChange(Dw::GraphicsItemChange change, const DwVariant &val);
    virtual void notifyItemChange(Dw::GraphicsItemChange change, const DwVariant& newOne, const DwVariant& oldOne);
    //virtual void notifyItemDirty(Dw::GraphicsItemDirty dirty);

    // Simple item tree visit algorithm
    typedef bool (*pfnSimpleItemEntry)(DwGraphicsItem* item);
    void notifyParentRecursively(pfnSimpleItemEntry visitor);
    void notifyChildrenRecursively(pfnSimpleItemEntry visitor);

    // Event stuffs
    DwItemEventHandler* eventHandler() const;
    void setEventHandler(DwItemEventHandler* handler);
    void sendEvent(DwSceneEvent *evt) const;
    void sendEvent(const DwGraphicsItem *item, DwSceneEvent *evt) const;
    DwEventSignalDispatch* signalDispatch() const;

    // Control bit stuffs
    inline bool isMovable() const;
    inline void setMovable(bool accept);

    inline bool isSelectable() const;
    inline void setSelectable(bool accept);

    inline bool isFocusable() const;
    inline void setFocusable(bool accept);

    inline bool isHoverable() const;
    inline void setHoverable(bool accept);

    inline bool isAcceptInputMethod() const;
    inline void setAcceptInputMethod(bool accept);

    inline bool isAcceptDrops() const;
    inline void setAcceptDrops(bool accept);

    inline uint32_t reserved() const;
    inline void setReserved(uint32_t i);

    uint32_t flags() const;
    void setFlag(Dw::GraphicsItemFlag flag, bool enabled = true);
    bool isFlag(Dw::GraphicsItemFlag flag) const;

    uint32_t acceptedMouseButtons() const;
    void setAcceptedMouseButtons(uint32_t buttons);
    void setAcceptedMouseButton(Dw::MouseButton button, bool accept);

    // Listener Stuffs
    void addListener(DwGraphicsItemListener* listener);
    void removeListener(DwGraphicsItemListener* listener);
    bool hasListener(DwGraphicsItemListener* listener) const;
    int  listenerSize() const;
    DwGraphicsItemListener* listenerAt(int idx) const;
    void removeAllListeners();
    void notifyListeners(Dw::ChangeType change, const DwVariant& newOne, const DwVariant& oldOne);

    // Anchor Stuffs
    DwAnchors* anchors();
    inline bool hasAnchors() const;
    void clearAnchors();
    inline void setBaselineOffset(DwReal offset);
    inline DwReal baselineOffset() const;

    // Focus Stuffs
    inline Dw::FocusPolicy focusPolicy()const;
    inline void setFocusPolicy(Dw::FocusPolicy policy);
    inline DwGraphicsItem* focusProxy() const;
    void setFocusProxy(DwGraphicsItem *item);
    void resetFocusProxy();
    bool hasFocus() const;
    void clearFocus() const;
    void setFocus() ;
    virtual DwGraphicsItem* focusNext() const;
    virtual DwGraphicsItem* focusPrev() const;
    void setTabOrder(DwGraphicsItem *pre, DwGraphicsItem* next);
    void setFocusNext(DwGraphicsItem *next);
    void setFocusPre(DwGraphicsItem *pre);
    void removeFromFocusLinkList();

    // Grab Stuffs
    void grabMouse();
    void ungrabMouse();
    void grabKeyboard();
    void ungrabKeyboard();

    // Index Stuffs
    inline int globalStackingOrder() const;
    inline int index() const;

    // Dirty Stuffs
    void prepareGemortyChange() ;
    void markDirty(bool b) ;
    void update(const DwRectF &r = DwRectF());

    // Debug Stuffs
    virtual void debugString(DwDebug &stream)const ;

    // Tips Stuffs
    virtual DwString toolTips() const;
    DwString toolTip() const;
    void setToolTips(DwString tips);

    // Animation Stuffs
    void addAnimation(DwAbstractAnimation* animation);
    void removeAnimation(DwAbstractAnimation* animation);
    bool hasAnimation(DwAbstractAnimation* animation) const;
    int animationNum() const;
    DwAbstractAnimation* animationAt(int idx)const;
    DwAbstractAnimation* animationOf(uint32_t uid)const;
    void removeAllAnimation();
    void clearAnimations();

    // Clip Stuffs
    const DwRegion* clipRegion() const;
    void setClipRegion(DwRegion *r) ;

    // Key Stuffs
    bool sendChangedEvent() const;
    void setSendChangedEvent(bool b);

    // internationlaztion
    DwString tr(const char * sourceText, const char * disambiguation = 0 );

    // Accessible Stuffs
    virtual DwString accessibleName() const;
    void setAccessibleName(const DwString &s);
    virtual DwString accessibleDescription() const;
    void setAccessibleDescription(const DwString &s);
    virtual DwString whatsThis() const;
    void setWhatsThis(const DwString& s);
    bool accessible() const;
    void setAccessible(bool b);

    // Global Item Index Stuffs
    static DwGraphicsItem * findItem(uint32_t idx);
    static bool addHook(DwGraphicsItemHook *hook);
    static bool removeHook(DwGraphicsItemHook *hook);

protected:
    void attachListener(DwGraphicsItemListener* listener);
    void detachListener(DwGraphicsItemListener* listener);

    void attachAnimation(DwAbstractAnimation* animation);
    void dettachAnimation(DwAbstractAnimation* animation);

    void addChild(DwGraphicsItem* item);
    void removeChild(DwGraphicsItem* item);
    
    void removeFromFocusLinkList(DwGraphicsItem* item);

    // Packed 32 bits
    uint32_t m_flags;
    // Packed 32 bits
    union
    {
        uint32_t m_packed32;
        struct
        {
            uint32_t m_reserved : 13 ;	// NB! MUST BE KEEP PACKED 32 BITS, USED FOR ENGINE
            uint32_t m_acceptDrops : 1;
            uint32_t m_dirty : 1 ;
            uint32_t m_dirtyPrepareGemotry : 1;
            uint32_t m_dirtyUpdate : 1;
            uint32_t m_inDestructor : 1;
            uint32_t m_visible : 1;
            uint32_t m_enabled : 1;
            uint32_t m_itemDiscovered : 1;
            uint32_t m_dirtyChildrenBoundingRect : 1;
            uint32_t m_dirtySceneTransform : 1;
            uint32_t m_dirtySceneZ : 1;
            uint32_t m_dirtyChildZ : 1;
            uint32_t m_prepareGemoryChange : 1;
            uint32_t m_acceptedMouseButtons : 5;
            uint32_t m_sendChangedEvent : 1;
        };
    };
    // a unique id
    uint32_t m_id;

    // Bounding Rect
    DwRectF m_localBoundingRect;
    DwRectF m_childrenBoundingRect;
    DwRectF m_clipRect;

    // Clip Stuffs
    DwRegion *m_clipRegion;

    // Translation, pos
    DwPointF m_pos;
    DwTransform m_transform;
    DwTransform m_sceneTransform;

    // Depth stuffs
    int m_itemDepth;
    int m_itemSiblingIndex;
    DwReal m_z;
    DwReal m_sceneZ;

    // misc
    DwString m_name;

    // Event stuffs
    DwItemEventHandler* m_eventHander;

    // Listener stuffs
    DwVector<DwGraphicsItemListener *> m_listeners;

    // Anchors stuffs
    DwAnchors* m_anchors;
    DwReal m_baselineOffset;

    // Focus Stuffs
    Dw::FocusPolicy m_focusPolicy;
    DwGraphicsItem *m_focusProxy;
    DwVector<DwGraphicsItem **> m_focusProxyRefs;
    DwGraphicsItem *m_focusNext;
    DwGraphicsItem *m_focusPrev;

    // Optional stacking order, used to bsp index
    int m_globalStackingOrder;
    int m_index;

    // Drawing Stuff
    DwReal m_opacity;

    // Tips Stuff
    DwString m_toolTips;

    // Animations Stuff
    DwVector<DwAbstractAnimation*> m_animations;

    // Accessible
    DwString m_accessibleName;
    DwString m_accessibleDescription;
    DwString m_whatsThis;

private:
    friend class DwGraphicsScene;
    friend class DwGraphicsSceneBspTree;
    friend class DwGraphicsSceneBspTreeVisitor;
    friend class DwGraphicsSceneFindItemBspTreeVisitor;
    friend class DwGraphicsItemListener;
    friend class DwGraphicsSceneBspTreeIndex;
    friend class DwAbstractAnimation;

    friend bool dw_pfn_entry_children_bounding_dirty(DwGraphicsItem* item);
    friend bool dw_pfn_entry_scene_transform_dirty(DwGraphicsItem* item);
    friend bool dw_pfn_entry_scene_z_dirty(DwGraphicsItem* item);
    friend bool dw_pfn_entry_child_z_dirty(DwGraphicsItem* item);

    DwGraphicsScene* m_scene;
    DwGraphicsItem* m_parent;
    DwGraphicsItemVector m_children;

    DW_DISABLE_COPY(DwGraphicsItem);
    DW_DECLARE_PROPERTY(DwGraphicsItem);

    DW_WIDGET_ROOT_PROPERTY_BEGIN(DwGraphicsItem);
#if DW_ENABLE_GRAPHICSITEM_PROPERTY
    DW_WIDGET_PROPERTY(pos, DwPointF, setPos, pos);
    DW_WIDGET_PROPERTY(clipRect, DwRectF, setClipRect, clipRect);
    DW_WIDGET_PROPERTY_NC(x, DwReal, setX, x);
    DW_WIDGET_PROPERTY_NC(y, DwReal, setY, y);
    DW_WIDGET_PROPERTY_NC(width, DwReal, setWidth, width);
    DW_WIDGET_PROPERTY_NC(height, DwReal, setHeight, height);
    DW_WIDGET_PROPERTY(rect, DwRectF, setRect, rect);
    DW_WIDGET_PROPERTY_NC(zValue, DwReal, setZValue, zValue);
    DW_WIDGET_PROPERTY(name, DwString, setName, name);
    DW_WIDGET_PROPERTY(boundingRect, DwRectF, setBoundingRect, boundingRect);
    DW_WIDGET_PROPERTY_NC(opacity, DwReal, setOpacity, opacity);
#endif
    DW_WIDGET_PROPERTY_END();
};// end of DwGraphicsItem
DW_REGISTER_CLASS(DwGraphicsItem);

//------------------------------------------------------------------------------
/**
*/
inline DwGraphicsItem* DwGraphicsItem::parentItem() const
{
    return m_parent;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsItem::isVisible() const
{
    return m_visible;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::hide()
{
    setVisible(false);
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::show()
{
    setVisible(true);
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsItem::isEnabled() const
{
    return m_enabled;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwGraphicsItem::x() const
{
    return pos().x();
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwGraphicsItem::y() const
{
    return pos().y();
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setPos(DwReal x, DwReal y)
{
    setPos(DwPointF(x, y));
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::moveBy(DwReal dx, DwReal dy)
{
    setPos(pos().x() + dx, pos().y() + dy);
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwGraphicsItem::width() const
{
    return boundingRect().width();
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwGraphicsItem::height() const
{
    return boundingRect().height();
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setRect(DwReal x, DwReal y, DwReal width, DwReal height)
{
    setRect(DwRectF(x, y, width, height));
}

//------------------------------------------------------------------------------
/**
*/
inline const DwVector<DwGraphicsItem*>& DwGraphicsItem::children() const
{
    return m_children;
}

//------------------------------------------------------------------------------
/**
*/
inline DwString DwGraphicsItem::name() const
{
    return m_name;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setName(const DwString &name)
{
    m_name = name;
}

//------------------------------------------------------------------------------
/**
*/
inline uint32_t DwGraphicsItem::id() const
{
    return m_id;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsItem::isMovable() const
{
    if(m_flags & Dw::ItemIsMovable) return true;
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setMovable(bool accept)
{
    setFlag(Dw::ItemIsMovable, accept);
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsItem::isSelectable() const
{
    if(m_flags & Dw::ItemIsSelectable) return true;
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setSelectable(bool accept)
{
    setFlag(Dw::ItemIsSelectable, accept);
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsItem::isFocusable() const
{
    if(m_flags & Dw::ItemIsFocusable) return true;
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setFocusable(bool accept)
{
    setFlag(Dw::ItemIsFocusable, accept);
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsItem::isHoverable() const
{
    if(m_flags & Dw::ItemIsHoverable) return true;
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setHoverable(bool accept)
{
    setFlag(Dw::ItemIsHoverable, accept);
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsItem::isAcceptInputMethod() const
{
    if(m_flags & Dw::ItemAcceptsInputMethod) return true;
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setAcceptInputMethod(bool accept)
{
    setFlag(Dw::ItemAcceptsInputMethod, accept);
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsItem::isAcceptDrops() const
{
    return m_acceptDrops;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setAcceptDrops(bool accept)
{
    m_acceptDrops = accept;
}

//------------------------------------------------------------------------------
/**
*/
inline uint32_t DwGraphicsItem::reserved() const
{
    return m_reserved;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setReserved(uint32_t i)
{
    m_reserved = i;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsItem::hasAnchors() const
{
    return NULL != m_anchors;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setBaselineOffset(DwReal offset)
{
    m_baselineOffset = offset;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwGraphicsItem::baselineOffset() const
{
    return m_baselineOffset;
}

//------------------------------------------------------------------------------
/**
*/
inline Dw::FocusPolicy DwGraphicsItem::focusPolicy()const
{
    return m_focusPolicy;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setFocusPolicy(Dw::FocusPolicy policy)
{
    m_focusPolicy = policy;
}

//------------------------------------------------------------------------------
/**
*/
inline DwGraphicsItem* DwGraphicsItem::focusProxy() const
{
    return m_focusProxy;
}

//------------------------------------------------------------------------------
/**
*/
inline int DwGraphicsItem::globalStackingOrder() const
{
    return m_globalStackingOrder;
}

//------------------------------------------------------------------------------
/**
*/
inline int DwGraphicsItem::index() const
{
    return m_index;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setAccessibleName(const DwString &s)
{
    m_accessibleName = s;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setAccessibleDescription(const DwString &s)
{
    m_accessibleDescription = s;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setWhatsThis(const DwString& s)
{
    m_whatsThis = s;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsItem::accessible() const
{
    return isFlag(Dw::ItemAccessible);
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsItem::setAccessible(bool b)
{
    setFlag(Dw::ItemAccessible, b);
}