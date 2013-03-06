#include "stable.h"
#include "dwgraphicsitem.h"
#include "dwgraphicsscene.h"
#include "dwgraphicseventhandler.h"
#include "dwgraphicsutil.h"
#include "dwdummys.h"
#include "dwgui/dwgraphicsview.h"
#include "dwgui/dwgraphicsitemlistener.h"
#include "dwgui/dwanchors.h"
#include "dwgui/dwabstractanimation.h"
#include "dwgui/dwgraphicsitemhook.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dweventsignaldispatch.h"
#include "dwgraphics/dwregion.h"
#include "dwgui/dwi18n.h"
#include "dwgui/dwdefaultitemeventhandler.h"

DW_IMPLEMENT_PROPERTY(DwGraphicsItem);
DW_IMPLEMENT_CLASS(DwGraphicsItem, 'ITEM', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
static uint32_t _dw_item_id()
{
    static uint32_t _dwId = 1;
    return _dwId++;
}

//------------------------------------------------------------------------------
/**
*/
struct ItemLib
{
    DwMap<uint32_t, DwGraphicsItem*> allItems;
    DwGraphicsItemHook hook;

    ~ItemLib()
    {
        DwGraphicsHook* next = hook.next();
        while(next)
        {
            DwGraphicsHook *lnext = next->next();
            DW_SAFE_DELETE(next);
            next = lnext;
        }
    }

    void insertItem(DwGraphicsItem *item)
    {
        DW_ASSERT(item);
        allItems[item->id()] = item;
    }

    void removeItem(DwGraphicsItem *item)
    {
        allItems.remove(item->id());
    }

    DwGraphicsItem *findItem(uint32_t idx) const
    {
        DwMap<uint32_t, DwGraphicsItem*>::const_iterator ite = allItems.find(idx);
        if (ite != allItems.end())
        {
            return ite->second;
        }
        return NULL;
    }

    bool addHook(DwGraphicsItemHook *h)
    {
        hook.append(h);
        return true;
    }

    bool removeHook(DwGraphicsItemHook *h)
    {
        bool isOk = false;

        DwGraphicsHook *l = &hook;
        DwGraphicsHook *n = hook.next();
        while(n)
        {
            if (n == h)
            {
                isOk = true;
                l->setNext(h->next());
                break;
            }

            l = n;
            n = l->next();
        }

        return isOk;
    }

    void doHook(Dw::ItemHook i, DwGraphicsItem* item, const DwVariant& v)
    {
        bool isBreak = false;
        DwGraphicsHook *l = &hook;
        DwGraphicsHook *n = hook.next();
        while(n && !isBreak)
        {
            DwGraphicsItemHook *h = dwsafe_cast<DwGraphicsItemHook>(n);
            if (h && h->hookApi())
            {
                dw_hook_api ha = h->hookApi();
                isBreak = ha(i, item, v);
            }
            l = n;
            n = l->next();
        }
    }
};
DW_GLOBAL_STATIC(ItemLib, _dw_ItemLib);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem::DwGraphicsItem(DwGraphicsItem *parent/* = NULL*/, DwGraphicsScene *scene/* = NULL*/)
    : m_parent(NULL)
    , m_scene(NULL)
    , m_flags(0)
    , m_reserved(0)
    , m_inDestructor(0)
    , m_itemDiscovered(0)
    , m_dirtyChildrenBoundingRect(1)
    , m_dirtySceneTransform(1)
    , m_dirtySceneZ(1)
    , m_prepareGemoryChange(0)
    , m_visible(1)
    , m_enabled(1)
    , m_itemDepth(-1)
    , m_z(0.0)
    , m_sceneZ(0.0)
    , m_eventHander(NULL)
    , m_itemSiblingIndex(InvalidIndex)
    , m_anchors(NULL)
    , m_baselineOffset(0.0)
    , m_focusPolicy(Dw::NoFocus)
    , m_focusProxy(NULL)
    , m_acceptedMouseButtons(0)
    , m_index(InvalidIndex)
    , m_globalStackingOrder(InvalidIndex)
    , m_dirty(0)
    , m_dirtyPrepareGemotry(0)
    , m_dirtyUpdate(0)
    , m_opacity(1.0)
    , m_acceptDrops(0)
    , m_id(0)
    , m_clipRegion(NULL)
    , m_sendChangedEvent(0)
    , m_focusNext(this)
    , m_focusPrev(this)
{
    m_id = _dw_item_id();

    setParentItem(parent);

    // add to scene
    if (scene && !parent)
    {
        scene->addItem(this);
    }

    _dw_ItemLib()->doHook(Dw::HookCreate, this, DwVariant());
    _dw_ItemLib()->insertItem(this);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem::~DwGraphicsItem()
{
    // native event
    DW_SAFE_DELETE(m_clipRegion);
    if (m_scene)
    {
#if DW_ENABLE_EVENT_ITEM_INNER
        DwItemDestroyEvent evt;
        m_scene->sendEvent(this, &evt);
#endif
    }
    m_inDestructor = 1;
    m_sendChangedEvent = false;

    resetFocusProxy();

    // notify listeners
    notifyListeners(Dw::Destroyed, DwVariant(), DwVariant());

    // while remove child in child's,
    // as in vector , we removing item back to front
    if(!m_children.isEmpty()) do
        {
            delete m_children.back();
        }
        while (!m_children.isEmpty());

    // move to scene
    if (m_scene)
    {
        m_scene->removeItemHelper(this);
    }
    else
    {
        setParentItem(NULL);
    }

    setClipRegion(NULL);
    setEventHandler(NULL);

    removeAllListeners();

    clearAnchors();

    clearAnimations();

    removeFromFocusLinkList(this);

    _dw_ItemLib()->doHook(Dw::HookDestroy, this, DwVariant());
    _dw_ItemLib()->removeItem(this);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsScene* DwGraphicsItem::scene() const
{
    return m_scene;
}

//------------------------------------------------------------------------------
/**
	call entry, addchild and remove child,
*/
void DwGraphicsItem::setParentItem(DwGraphicsItem* parent)
{
    DW_CHECK(parent != m_parent);
    DW_CHECK_ASSERT(parent != this);

    prepareGemortyChange();
    DwGraphicsItem *oldOne = m_parent;

    // do not deal with item in destructor
    // should do here to
    if (m_scene && !m_inDestructor)
    {
        if (!m_parent && parent)
        {
            m_scene->unRegisterTopLevelItem(this);
        }
        else if (m_parent && !parent)
        {
            m_scene->registerTopLevelItem(this);
        }
    }

    if (m_parent)
    {
        m_parent->removeChild(this);
    }

    // should set m_parent before scene->additem, removeitem
    m_parent = parent;
    if (parent)
    {
        if (parent->scene() && parent->scene() != m_scene)
        {
            parent->scene()->addItem(this);
        }
        else if (!parent->scene() && m_scene)
        {
            m_scene->removeItem(this);
        }

        parent->addChild(this);
    }

    invalidateDepthRecursively();
    notifyItemChange(Dw::ItemParentChange, DwVariant((void*)(m_parent)), DwVariant((void*)(oldOne)));
    update();
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsItem::topLevelItem() const
{
    DwGraphicsItem *parent = const_cast<DwGraphicsItem *>(this);
    while (DwGraphicsItem *grandPa = parent->m_parent)
    {
        parent = grandPa;
    }
    return parent;
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsItem::isAncestorOf(const DwGraphicsItem *child) const
{
    if (!child || child == this)
    {
        return false;
    }
    if (child->depth() < depth())
    {
        return false;
    }
    const DwGraphicsItem *ancestor = child;
    while ((ancestor = ancestor->m_parent))
    {
        if (ancestor == this)
            return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwGraphicsItem::commonAncestorItem(const DwGraphicsItem *other) const
{
    if (!other)
    {
        return 0;
    }
    if (other == this)
    {
        return const_cast<DwGraphicsItem *>(this);
    }
    const DwGraphicsItem *thisw = this;
    const DwGraphicsItem *otherw = other;
    int thisDepth = depth();
    int otherDepth = other->depth();
    while (thisDepth > otherDepth)
    {
        thisw = thisw->m_parent;
        --thisDepth;
    }
    while (otherDepth > thisDepth)
    {
        otherw = otherw->m_parent;
        --otherDepth;
    }
    while (thisw && thisw != otherw)
    {
        thisw = thisw->m_parent;
        otherw = otherw->m_parent;
    }
    return const_cast<DwGraphicsItem *>(thisw);
}

//------------------------------------------------------------------------------
/**
*/
int DwGraphicsItem::depth() const
{
    if (m_itemDepth != -1)
    {
        return m_itemDepth;
    }
    else
    {
        (const_cast<DwGraphicsItem*>(this))->resolveDepth();
        return m_itemDepth;
    }
}


//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::invalidateDepthRecursively()
{
    if (m_itemDepth == -1)
    {
        return;
    }

    m_itemDepth = -1;
    for (int index = 0; index < m_children.size(); ++index)
    {
        m_children.at(index)->invalidateDepthRecursively();
    }
}


//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::resolveDepth()
{
    if (m_parent)
    {
        m_itemDepth = m_parent->depth() + 1;
    }
    else
    {
        m_itemDepth = 0;
    }
}

//------------------------------------------------------------------------------
/**
	The Result In Item's Local Coordsystem.
    return the item's local bounding rect
*/
DwRectF DwGraphicsItem::boundingRect() const
{
    return m_localBoundingRect;
}

//------------------------------------------------------------------------------
/**
    set the item's local bounding rect
*/
void DwGraphicsItem::setBoundingRect(const DwRectF &bounding)
{
    // Re Entry
    if (m_localBoundingRect == bounding)
    {
        return;
    }

    // Gemory Change Prepare
    prepareGemortyChange();
    bool sendChange = !m_prepareGemoryChange;
    if (sendChange)
    {
        m_prepareGemoryChange = true;
    }
    DwRectF oldRect = rect();

    m_localBoundingRect = bounding;
    // tell parent that the parent's childrenBoundingRect() is dirty
    notifyParentRecursively(dw_pfn_entry_children_bounding_dirty);

    // Notify Change
    if (sendChange)
    {
        notifyListeners(Dw::Geometry, DwVariant(rect()), DwVariant(oldRect));
        m_prepareGemoryChange = false;
        itemChange(Dw::ItemGemoryChange, DwVariant(rect()));
        notifyItemChange(Dw::ItemGemoryChange, DwVariant(rect()), DwVariant(oldRect));
    }
    update();
}

//------------------------------------------------------------------------------
/**
	The Result In Item's Local Coordsystem.
	return the bounding rect that
	composed by all of the children in parent coordinate system
	in this way, a child's bounding will contains the grandchild and child's local
*/
DwRectF DwGraphicsItem::childrenBoundingRect() const
{
    if (!m_dirtyChildrenBoundingRect)
    {
        return m_childrenBoundingRect;
    }
    else
    {
        // init with the item self's local boundingRect
        const_cast<DwGraphicsItem*>(this)->m_childrenBoundingRect = DwRectF();
        for (int i=0; i<m_children.size(); ++i)
        {
            DwGraphicsItem* child = m_children.at(i);
            const_cast<DwGraphicsItem*>(this)->m_childrenBoundingRect |= child->mapToParent(
                        child->childrenBoundingRect()|child->m_localBoundingRect);
        }
        const_cast<DwGraphicsItem*>(this)->m_dirtyChildrenBoundingRect = 0;
        return m_childrenBoundingRect;
    }
}

//------------------------------------------------------------------------------
/**
	The Result In Scene Coordsystem.
    return the local bounding rect in the scene coord
*/
DwRectF DwGraphicsItem::sceneBoundingRect() const
{
    return this->mapToScene(boundingRect());
}

//------------------------------------------------------------------------------
/**
	The Result In Scene Coordsystem.
    return (the local bounding rect | the children's bounding rect) in the scene coord
*/
DwRectF DwGraphicsItem::sceneEffectiveBoundingRect() const
{
    return this->mapToScene(childrenBoundingRect() | m_localBoundingRect) ;
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwGraphicsItem::viewRect(const DwGraphicsView *view) const
{
    if (view)
    {
        DW_ASSERT(view && view->scene() == scene());
        DwRectF sceneRect = sceneBoundingRect();
        return view->mapFromScene(sceneRect);
    }
    return boundingRect();
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwGraphicsItem::deviceRect(const DwGraphicsView *view) const
{
    if (view)
    {
        DW_ASSERT(view && view->scene() == scene());
        return view->mapToDevice(viewRect(view));
    }
    return boundingRect();
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwGraphicsItem::screenRect(const DwGraphicsView *view) const
{
    if (view)
    {
        DW_ASSERT(view && view->scene() == scene());
        return view->mapToGlobal(viewRect(view));
    }
    return boundingRect();
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwGraphicsItem::clipRect() const
{
    if (m_clipRect.isValid())
    {
        return m_clipRect;
    }
    return m_localBoundingRect;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setClipRect(const DwRectF& rect)
{
    if(m_clipRect == rect)
        return;

    prepareGemortyChange();
    m_clipRect = rect;
    update(m_clipRect);
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
void DwGraphicsItem::paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option)// = 0;
{
    DW_UNUSED(painter);
    DW_UNUSED(option);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setOpacity(DwReal opaque)
{
    if(m_opacity == opaque) return;
    m_opacity = opaque;
    update();
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwGraphicsItem::opacity() const
{
    return m_opacity;
}

//------------------------------------------------------------------------------
/**
*/
DwPointF DwGraphicsItem::mapToParent(const DwPointF &point) const
{
    return transform().map(point);
}

//------------------------------------------------------------------------------
/**
*/
DwPointF DwGraphicsItem::mapToScene(const DwPointF &point) const
{
    return sceneTransform().map(point);
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwGraphicsItem::mapToParent(const DwRectF &rect) const
{
    return transform().mapRect(rect);
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwGraphicsItem::mapToScene(const DwRectF &rect) const
{
    return sceneTransform().mapRect(rect);
}

//------------------------------------------------------------------------------
/**
*/
DwPointF DwGraphicsItem::mapFromScene(const DwPointF &point) const
{
    return sceneTransform().inverted().map(point);
}

//------------------------------------------------------------------------------
/**
*/
DwPoint DwGraphicsItem::mapFromScene(const DwPoint &point) const
{
    return sceneTransform().inverted().map(point);
}

//------------------------------------------------------------------------------
/**
*/
DwPointF DwGraphicsItem::mapFromParent(const DwPointF &point) const
{
    return transform().inverted().map(point);
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwGraphicsItem::mapFromScene(const DwRectF &r) const
{
    return sceneTransform().inverted().mapRect(r);
}

//------------------------------------------------------------------------------
/**
*/
DwPoint DwGraphicsItem::mapFromParent(const DwPoint &point) const
{
    return transform().inverted().map(point);
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwGraphicsItem::mapFromParent(const DwRectF &r) const
{
    return transform().inverted().mapRect(r);
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsItem::isVisibleTo(const DwGraphicsItem *parent) const
{
    if (!m_visible)
        return false;
    if (parent == this)
        return true;
    if (m_parent && m_parent->isVisibleTo(parent))
        return true;
    if (!parent && !m_parent)
        return true;
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setVisible(bool visible)
{
    if (m_visible != visible)
    {
        bool oldValue = m_visible;
        m_visible = visible;
        notifyListeners(Dw::Visibility, DwVariant(m_visible), DwVariant(oldValue) );
        update();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setEnabled(bool enabled)
{
    if(m_enabled == enabled) return;
    m_enabled = enabled;
    update();

    if (m_eventHander)
    {
        m_eventHander->setEnabled(m_enabled);
    }
}

//------------------------------------------------------------------------------
/**
	Returns the position of the item in parent coordinates. If the item has no
	parent, its position is given in scene coordinates.

	The position of the item describes its origin (local coordinate
	(0, 0)) in parent coordinates; this function returns the same as
	mapToParent(0, 0).
*/
DwPointF DwGraphicsItem::pos() const
{
    return m_pos;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setX(DwReal x)
{
    if (m_inDestructor)
        return;

    setPos(DwPointF(x, m_pos.y()));
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setY(DwReal y)
{
    if (m_inDestructor)
        return;

    setPos(DwPointF(m_pos.x(), y));
}

//------------------------------------------------------------------------------
/**
*/
DwPointF DwGraphicsItem::scenePos() const
{
    return mapToScene(DwPointF(0., 0.));
}

//------------------------------------------------------------------------------
/**
	pos to transform
*/
void DwGraphicsItem::setPos(const DwPointF &p)
{
    // as there is a transform problem in drawstuff, 
    // we only accept the rounded pos
    DwPointF pos = p.toPoint();
    if (m_pos == pos)
        return;

    if (m_inDestructor)
        return;

    // Gemory Change Prepare
    prepareGemortyChange();
    bool sendChange = !m_prepareGemoryChange;
    if (sendChange)
    {
        m_prepareGemoryChange = true;
    }
    DwRectF oldRect = rect();
    DwPointF oldPos = m_pos;

    // Update and repositition.
    m_pos = pos;
    m_transform.setTranslated(pos.x(), pos.y());

    // tell parent that the parent's childrenBoundingRect() will be dirty
    notifyParentRecursively(dw_pfn_entry_children_bounding_dirty);
    // tell children that the child's sceneTransform will be dirty
    m_dirtySceneTransform = 1;
    notifyChildrenRecursively(dw_pfn_entry_scene_transform_dirty);

    // Notify Change
    if (sendChange)
    {
        notifyListeners(Dw::Geometry, DwVariant(rect()), DwVariant(oldRect));
        m_prepareGemoryChange = false;
        itemChange(Dw::ItemGemoryChange, DwVariant(rect()));
        notifyItemChange(Dw::ItemGemoryChange, DwVariant(rect()), DwVariant(oldRect));
    }

    notifyListeners(Dw::Position, DwVariant(m_pos), DwVariant(oldPos));
    notifyItemChange(Dw::ItemPositionChange, DwVariant(m_pos), DwVariant(oldPos));
    update();
}

//------------------------------------------------------------------------------
/**
	NB! ONLY SUPPORT TRANSLATED IN TRANSFORM
	return the rect in parent coordinate system,
*/
DwRectF DwGraphicsItem::rect() const
{
    return boundingRect().translated(pos());
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setWidth(DwReal w)
{
    setRect(x(), y(), w, height());
}

//------------------------------------------------------------------------------
/**
*/
DwSizeF DwGraphicsItem::size() const
{
    return DwSizeF(width(), height());
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setHeight(DwReal h)
{
    setRect(x(), y(), width(), h);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setSize(DwReal w, DwReal h)
{
    setRect(x(), y(), w, h);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setSize(DwSizeF size)
{
    setSize(size.width(), size.height());
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setRect(const DwRectF &ret)
{
    // as there is a transform problem in drawstuff, 
    // we only accept the rounded rect
    DwRectF r = ret.toRect();
    // Gemory Change Prepare
    bool sendChange = !m_prepareGemoryChange;
    if (sendChange)
    {
        m_prepareGemoryChange = true;
    }
    DwRectF oldRect = rect();

    setPos(r.topLeft());
    setBoundingRect(r.translated(-r.topLeft()));

    // Notify Change
    if (sendChange)
    {
        notifyListeners(Dw::Geometry, DwVariant(rect()), DwVariant(oldRect));
        m_prepareGemoryChange = false;
        itemChange(Dw::ItemGemoryChange, DwVariant(rect()));
        notifyItemChange(Dw::ItemGemoryChange, DwVariant(rect()), DwVariant(oldRect));
    }
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwGraphicsItem::setGeometry(const DwRectF &rect)
{
    setRect(rect);
}

//------------------------------------------------------------------------------
/**
    transform item coordsytem to parent coordsytem
*/
const DwTransform & DwGraphicsItem::transform() const
{
    return m_transform;
}

//------------------------------------------------------------------------------
/**
    trranform item coordsytem to scene coordsystem
*/
const DwTransform & DwGraphicsItem::sceneTransform() const
{
    if (!m_dirtySceneTransform)
    {
        return m_sceneTransform;
    }

    const_cast<DwGraphicsItem*>(this)->m_sceneTransform = m_transform;
    if (m_parent)
    {
        const_cast<DwGraphicsItem*>(this)->m_sceneTransform *= m_parent->sceneTransform();
    }

    const_cast<DwGraphicsItem*>(this)->m_dirtySceneTransform = 0;
    return m_sceneTransform;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::resetTransform()
{
    setTransform(DwTransform());
}

//------------------------------------------------------------------------------
/**
	transform to pos
*/
void DwGraphicsItem::setTransform(const DwTransform &matrix, bool combine/* = false*/)
{
    DwTransform newTransform = combine ? matrix*m_transform : matrix;
    if (newTransform == m_transform)
    {
        return;
    }

    prepareGemortyChange();

    m_transform = newTransform;
    m_pos = m_transform.translated();

    // tell parent that the parent's childrenBoundingRect() will be dirty
    notifyParentRecursively(dw_pfn_entry_children_bounding_dirty);
    // tell children that the child's sceneTransform will be dirty
    m_dirtySceneTransform = 1;
    notifyChildrenRecursively(dw_pfn_entry_scene_transform_dirty);

    update();
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwGraphicsItem::zValue() const
{
    return m_z;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setZValue(DwReal z)
{
    if (m_z == z)
    {
        return;
    }
    DwReal oldOne = m_z;
    m_z = z;
    m_dirtySceneZ = 1;
    notifyChildrenRecursively(dw_pfn_entry_scene_z_dirty);
    notifyParentRecursively(dw_pfn_entry_child_z_dirty);
    notifyItemChange(Dw::ItemZValueChange, DwVariant(m_z), DwVariant(oldOne));

    update();
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwGraphicsItem::sceneZValue() const
{
    if (!m_dirtySceneZ)
    {
        return m_sceneZ;
    }
    const_cast<DwGraphicsItem*>(this)->resolveSceneZValue();
    return m_sceneZ;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::resolveSceneZValue()
{
    const DwReal IncrementalZ = 1.0;

    if (m_parent)
    {
        m_sceneZ = m_parent->sceneZValue() + m_z + IncrementalZ;
    }
    else
    {
        m_sceneZ = m_z;
    }
    m_dirtySceneZ = 0;
}

//------------------------------------------------------------------------------
/**
	When your children z value have change the siblingIndex will be reassigned
*/
void DwGraphicsItem::ensureSortedChildren()
{
    if (m_dirtyChildZ)
    {
        m_dirtyChildZ = 0;
        if(m_children.size() <= 0) return;

        ensureSequentialSiblingIndex();
        DwGraphicsUtil::sortChild(&m_children);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::ensureSequentialSiblingIndex()
{
    if(m_children.size() <= 0) return;
    // first sort by siblingindex
    DwGraphicsUtil::sortChildBySiblingIndex(&m_children, Dw::AscendingOrder);
    for (int index=0; index<m_children.size(); ++index)
    {
        m_children.at(index)->m_itemSiblingIndex = index;
    }
}

//------------------------------------------------------------------------------
/**
    The siblingIndex just mean the insert order in current children
    If you want the continuous siblingIndex, you just need to call ensureSortedChildren()
*/
int DwGraphicsItem::siblingIndex() const
{
    return m_itemSiblingIndex;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::stackBefore(const DwGraphicsItem *sibling)
{
    if (sibling == this)
        return;
    if (!sibling || m_parent != sibling->m_parent)
    {
        DW_WARNING("DwGraphicsItem::stackBefore: cannot stack under %p, which must be a sibling");
        return;
    }
    DwVector<DwGraphicsItem *> *siblings = m_parent
                                           ? &m_parent->m_children
                                           : (m_scene ? &m_scene->m_topLevelItems : 0);
    if (!siblings)
    {
        DW_WARNING("DwGraphicsItem::stackBefore: cannot stack under %p, which must be a sibling");
        return;
    }

    // First, make sure that the sibling indexes have no holes. This also
    // marks the children list for sorting.
    bool isTopLevelItem = false;
    if (m_parent)
    {
        m_parent->ensureSequentialSiblingIndex();
    }
    else
    {
        isTopLevelItem = true;
        m_scene->ensureSequentialTopLevelSiblingIndexes();
    }

    // Only move items with the same Z value, and that need moving.
    int siblingIndexx = sibling->siblingIndex();
    int myIndex = siblingIndex();
    if (myIndex >= siblingIndexx)
    {
        if (isTopLevelItem)
        {
            siblings->remove(siblings->size() - myIndex - 1);
            if ( 0 >= siblingIndexx)
            {
                siblings->append(this);
            }
            else
            {
                siblings->insert(siblings->size() - siblingIndexx + 1, this);
            }
            // Fixup the insertion ordering.
            for (int i = 0; i < siblings->size(); ++i)
            {
                int &index = siblings->at(i)->m_itemSiblingIndex;
                if (index >= siblingIndexx && index <= myIndex)
                    ++index;
            }
            m_itemSiblingIndex = siblingIndexx;
        }
        else
        {
            siblings->remove(myIndex);
            siblings->insert(siblingIndexx, this);
            // Fixup the insertion ordering.
            for (int i = 0; i < siblings->size(); ++i)
            {
                int &index = siblings->at(i)->m_itemSiblingIndex;
                if (i != siblingIndexx && index >= siblingIndexx && index <= myIndex)
                    ++index;
            }
            m_itemSiblingIndex = siblingIndexx;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwGraphicsItem::findChild(const DwString &id) const
{
    int pos = id.indexOf(L'.');
    DwString name;
    if (-1 == pos)
        name = id;
    else
        name = id.left(pos);

    DwVector<DwGraphicsItem* >::const_iterator it;
    DwGraphicsItem *childItem = 0;
    for (it = m_children.begin(); it != m_children.end(); ++ it)
    {
        if ((*it)->name() == name)
        {
            childItem = *it;
            break;
        }
    }

    if (!childItem) return 0;
    DW_ASSERT(childItem);
    if (-1 == pos) return childItem;
    return childItem->findChild(id.mid(pos + 1));
}

//------------------------------------------------------------------------------
/**
*/
const DwString DwGraphicsItem::fullId() const
{
    if (m_parent)
    {
        return m_parent->fullId() + "." + name();
    }
    else
    {
        DW_ASSERT(0 && "try get full id of a orphan item");
        return name();
    }
}

//------------------------------------------------------------------------------
/**
*/
DwVariant DwGraphicsItem::itemChange(Dw::GraphicsItemChange change, const DwVariant &val)
{
    DW_UNUSED(change);
    DW_UNUSED(val);
    return val;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::notifyItemChange(Dw::GraphicsItemChange change, const DwVariant& newOne, const DwVariant& oldOne)
{
    if (m_scene)
    {
        m_scene->onItemChange(this, change, newOne, oldOne);
    }

    if (m_sendChangedEvent)
    {
#if DW_ENABLE_EVENT_ITEM_INNER
        DwItemChangeEvent evt;
        evt.m_change = change;
        evt.m_oldOne = oldOne;
        evt.m_newOne = newOne;
        sendEvent(&evt);
#endif
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::notifyParentRecursively(pfnSimpleItemEntry visitor)
{
    DW_ASSERT(visitor);
    bool isNext = true;
    if (DwGraphicsItem* parent = m_parent)do
        {
            isNext = visitor(parent);
            parent = parent->m_parent;
        }
        while(parent && isNext);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::notifyChildrenRecursively(pfnSimpleItemEntry visitor)
{
    for (int i=0; i<m_children.size(); ++i)
    {
        DwGraphicsItem* child = m_children.at(i);
        bool isNext = visitor(child);
        if (isNext)
        {
            child->notifyChildrenRecursively(visitor);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
DwItemEventHandler* DwGraphicsItem::eventHandler() const
{
    if (!m_eventHander && !m_inDestructor)
    {
        const_cast<DwGraphicsItem*>(this)->m_eventHander 
            = new DwDefaultItemEventHandler((DwGraphicsItem*)this);
    }
    return m_eventHander;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setEventHandler(DwItemEventHandler* handler)
{
    if (m_eventHander)
    {
        delete m_eventHander;
    }
    m_eventHander = handler;

    if (m_eventHander)
    {
        m_eventHander->setEnabled(m_enabled);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::sendEvent(DwSceneEvent *evt) const
{
    sendEvent(this, evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::sendEvent(const DwGraphicsItem *item, DwSceneEvent *evt) const
{
    if (m_scene)
    {
        m_scene->sendEvent(item, evt);
    }
}


//------------------------------------------------------------------------------
/**
*/
DwEventSignalDispatch* DwGraphicsItem::signalDispatch() const
{
    DwItemEventHandler *handler = eventHandler();
    if (handler)
    {
        return handler->dispatcher();
    }
    return NULL;
}


//------------------------------------------------------------------------------
/**
*/
uint32_t DwGraphicsItem::flags() const
{
    return m_flags;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setFlag(Dw::GraphicsItemFlag flag, bool enabled/* = true*/)
{
    int oldOne = m_flags;
    if (enabled)
    {
        m_flags = m_flags | flag;
    }
    else
    {
        m_flags = m_flags & (~flag);
    }
    notifyItemChange(Dw::ItemFlagsChange, DwVariant(m_flags), DwVariant(oldOne));
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsItem::isFlag(Dw::GraphicsItemFlag flag) const
{
    return !!(m_flags & flag);
}

//------------------------------------------------------------------------------
/**
*/
uint32_t DwGraphicsItem::acceptedMouseButtons() const
{
    return m_acceptedMouseButtons;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setAcceptedMouseButtons(uint32_t buttons)
{
    m_acceptedMouseButtons = buttons;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setAcceptedMouseButton(Dw::MouseButton button, bool accept)
{
    if (accept)
    {
        m_acceptedMouseButtons = m_acceptedMouseButtons | button;
    }
    else
    {
        m_acceptedMouseButtons = m_acceptedMouseButtons & (~button);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::addListener(DwGraphicsItemListener* listener)
{
    DW_ASSERT(listener);
    listener->addDepend(this);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::removeListener(DwGraphicsItemListener* listener)
{
    DW_ASSERT(listener);
    listener->remDepend(this);
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsItem::hasListener(DwGraphicsItemListener* listener) const
{
    return m_listeners.indexOf(listener) != InvalidIndex;
}

//------------------------------------------------------------------------------
/**
*/
int  DwGraphicsItem::listenerSize() const
{
    return m_listeners.size();
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItemListener* DwGraphicsItem::listenerAt(int index) const
{
    DW_ASSERT(index > 0 && index < m_listeners.size());
    return m_listeners.at(index);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::removeAllListeners()
{
    DwVector<DwGraphicsItemListener *> listeners = m_listeners;
    for (int i = 0; i < listeners.size(); ++i)
    {
        removeListener(listeners.at(i));
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::notifyListeners(Dw::ChangeType change, const DwVariant& newOne, const DwVariant& oldOne)
{
    if (m_listeners.size() > 0)
    {
        for (int i=0; i < m_listeners.size(); ++i)
        {
            DwGraphicsItemListener* listener = m_listeners.at(i);
            DW_ASSERT(listener);
            if (listener->changes() & change)
            {
                listener->onChange(change, this, newOne, oldOne);
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
DwAnchors* DwGraphicsItem::anchors()
{
    if (!m_anchors)
    {
        m_anchors = new DwAnchors(this);
    }
    return m_anchors;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::clearAnchors()
{
    if (m_anchors)
    {
        m_anchors->remAllDepends();
        DW_SAFE_DELETE(m_anchors);
    }
};

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setFocusProxy(DwGraphicsItem *item)
{
    if (item == m_focusProxy)
        return;
    if (item == this)
    {
        DW_WARNING("DwGraphicsItem::setFocusProxy: cannot assign self as focus proxy");
        return;
    }
    if (item)
    {
        if (item->scene() != m_scene)
        {
            DW_WARNING("DwGraphicsItem::setFocusProxy: focus proxy must be in same scene");
            return;
        }
        for (DwGraphicsItem *f = item->focusProxy(); f != 0; f = f->focusProxy())
        {
            if (f == this)
            {
                DW_WARNING("DwGraphicsItem::setFocusProxy: %p is already in the focus proxy chain");
                return;
            }
        }
    }

    DwGraphicsItem *lastFocusProxy = m_focusProxy;
    if (lastFocusProxy)
    {
        lastFocusProxy->m_focusProxyRefs.removeOne(&m_focusProxy);
    }
    m_focusProxy = item;
    if (item)
    {
        item->m_focusProxyRefs.append(&m_focusProxy);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::resetFocusProxy()
{
    for (int i = 0; i < m_focusProxyRefs.size(); ++i)
        *m_focusProxyRefs.at(i) = 0;
    m_focusProxyRefs.clear();
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsItem::hasFocus() const
{
    if (!m_scene->hasFocus()) return false;

    if (m_scene->focusItem() == this || m_scene->focusItem() == m_focusProxy)
    {
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::clearFocus() const
{
    if (hasFocus())
    {
        m_scene->clearFocus();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setFocus()
{
    if (m_scene)
    {
        m_scene->setFocusItem(this, Dw::OtherFocusReason);
    }
}

//------------------------------------------------------------------------------
/**
	IN THE SUBCLASS MUST IMPLEMENT THIS, IF WANT DILL WITH FOCUS STUFF
*/
DwGraphicsItem* DwGraphicsItem::focusNext() const
{
    return m_focusNext;
}

//------------------------------------------------------------------------------
/**
	IN THE SUBCLASS MUST IMPLEMENT THIS, IF WANT DILL WITH FOCUS STUFF
*/
DwGraphicsItem* DwGraphicsItem::focusPrev() const
{
    return m_focusPrev;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setTabOrder(DwGraphicsItem *pre, DwGraphicsItem* next)
{
    setFocusNext(next);
    setFocusPre(pre);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setFocusNext(DwGraphicsItem *next)
{
    if (m_focusNext == next)
    {
        return;
    }

    if (next)
    {
        DW_ASSERT(next->scene() == scene());
        removeFromFocusLinkList(next);
        next->m_focusNext = m_focusNext;
        next->m_focusPrev = this;
    }

    if (m_focusNext)
    {
        m_focusNext->m_focusPrev = next;
    }

    m_focusNext = next;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setFocusPre(DwGraphicsItem *pre)
{
    if (m_focusPrev == pre)
    {
        return;
    }

    if (pre)
    {
        DW_ASSERT(pre->scene() == scene());
        removeFromFocusLinkList(pre);
        pre->m_focusNext = this;
        pre->m_focusPrev = m_focusPrev;
    }

    if (m_focusPrev)
    {
        m_focusPrev->m_focusNext = pre;
    }

    m_focusPrev = pre;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::removeFromFocusLinkList()
{
    removeFromFocusLinkList(this);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::grabMouse()
{
    m_scene->grabMouse(this);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::ungrabMouse()
{
    m_scene->ungrabMouse(this);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::grabKeyboard()
{
    m_scene->grabKeyboard(this);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::ungrabKeyboard()
{
    m_scene->unGrabKeyboard();
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwGraphicsItem::prepareGemortyChange()
{
    if (m_dirtyPrepareGemotry)  return;
    if (m_scene)
    {
        m_dirtyPrepareGemotry = 1;
        markDirty(m_dirtyPrepareGemotry);
        m_scene->update(sceneBoundingRect());
    }
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwGraphicsItem::markDirty(bool b)
{
    DW_UNUSED(b);
    if (m_dirty) return;
    if (m_scene)
    {
        m_dirty = 1;
        m_scene->markDirty(this, sceneBoundingRect());
    }
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwGraphicsItem::update(const DwRectF &updateRect/* = DwRectF()*/)
{
    DwRectF r = updateRect;
    _dw_ItemLib()->doHook(Dw::HookUpdate, this, DwVariant(r));

    if (m_scene)
    {
        if (r.isNull())
        {
            m_dirtyUpdate = 1;
            markDirty(m_dirtyUpdate);
            m_scene->update(sceneBoundingRect());
        }
        else
        {
            m_scene->update(mapToScene(r));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::debugString(DwDebug &stream)const
{
    stream  <<" " << this->rtti()->name() << "["
            <<" name :" << m_name
            << " pos :" << m_pos
            << " rect:" << boundingRect()
            << " visible: " << isVisible()
            << " enabled: " << isEnabled()
            << " z: " << m_z
            << " parent: " << ( m_parent ? m_parent->name() : L"NULL" )
            << " scene: " << (int)m_scene
            << "]";
}

//------------------------------------------------------------------------------
/**
    virtual
*/
DwString DwGraphicsItem::toolTips() const
{
    return m_toolTips;
}

//------------------------------------------------------------------------------
/**
    no virtual
*/
DwString DwGraphicsItem::toolTip() const
{
    return m_toolTips;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setToolTips(DwString tips)
{
    m_toolTips = tips;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::addAnimation(DwAbstractAnimation* animation)
{
    DW_CHECK(animation);
    animation->attachTo(this);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::removeAnimation(DwAbstractAnimation* animation)
{
    if (hasAnimation(animation))
    {
        animation->dettach();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsItem::hasAnimation(DwAbstractAnimation* animation) const
{
    return m_animations.indexOf(animation) != InvalidIndex;
}

//------------------------------------------------------------------------------
/**
*/
int DwGraphicsItem::animationNum() const
{
    return m_animations.size();
}

//------------------------------------------------------------------------------
/**
*/
DwAbstractAnimation* DwGraphicsItem::animationAt(int idx)const
{
    DW_CHECK_RET(idx > 0 && idx < m_animations.size(), NULL);
    return m_animations.at(idx);
}

//------------------------------------------------------------------------------
/**
*/
DwAbstractAnimation* DwGraphicsItem::animationOf(uint32_t uid)const
{
    for (int i=0; i<m_animations.size(); ++i)
    {
        DwAbstractAnimation* ani = m_animations.at(i);
        if (ani && ani->id() == uid)
        {
            return ani;
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::removeAllAnimation()
{
    DwVector<DwAbstractAnimation*> animations = m_animations;
    for (int idx=0; idx<animations.size(); ++idx)
    {
        animations.at(idx)->dettach();
    }
    m_animations.clear();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::clearAnimations()
{
    DwVector<DwAbstractAnimation*> animations = m_animations;
    for (int idx=0; idx<animations.size(); ++idx)
    {
        animations.at(idx)->stop();
        animations.at(idx)->dettach();
    }
    m_animations.clear();
}

//------------------------------------------------------------------------------
/**
*/
const DwRegion* DwGraphicsItem::clipRegion() const
{
    return m_clipRegion;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setClipRegion(DwRegion *r)
{
    DW_SAFE_DELETE(m_clipRegion);
    if (r)
    {
        m_clipRegion = r->clone();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsItem::sendChangedEvent() const
{
    return m_sendChangedEvent;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::setSendChangedEvent(bool b)
{
    m_sendChangedEvent = b;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwGraphicsItem::tr(const char * sourceText, const char * disambiguation/* = 0 */)
{
    return RttiTrWithIn(sourceText, disambiguation, rtti()->cname());
}

//------------------------------------------------------------------------------
/**
*/
DwString DwGraphicsItem::accessibleName() const
{
    return m_accessibleName;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwGraphicsItem::accessibleDescription() const
{
    return m_accessibleDescription;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwGraphicsItem::whatsThis() const
{
    return m_whatsThis;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem * DwGraphicsItem::findItem(uint32_t idx)
{
    return _dw_ItemLib()->findItem(idx);
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsItem::addHook(DwGraphicsItemHook *hook)
{
    return _dw_ItemLib()->addHook(hook);
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsItem::removeHook(DwGraphicsItemHook *hook)
{
    return _dw_ItemLib()->removeHook(hook);
}

//------------------------------------------------------------------------------
/**
    NB! ONLY SHOULD BE CALLED BY LISTENER IN ADDPEND() AND REMDEPEND()
*/
void DwGraphicsItem::attachListener(DwGraphicsItemListener* listener)
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
void DwGraphicsItem::detachListener(DwGraphicsItemListener* listener)
{
    DW_ASSERT(listener);
    DW_ASSERT(m_listeners.indexOf(listener) != InvalidIndex);

    listener->onRemoved(this);
    m_listeners.removeOne(listener);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::attachAnimation(DwAbstractAnimation* animation)
{
    animation->onAttachTo(this);
    m_animations.append(animation);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::dettachAnimation(DwAbstractAnimation* animation)
{
    animation->onDettachTo(this);
    m_animations.removeOne(animation);
}

//------------------------------------------------------------------------------
/**
	NB! do not call user
*/
void DwGraphicsItem::addChild(DwGraphicsItem* item)
{
    DW_ASSERT(m_children.indexOf(item) == InvalidIndex);
    item->m_itemSiblingIndex = m_children.size() > 0 ? m_children.back()->m_itemSiblingIndex + 1 : 0;
    m_children.append(item);

    // item change
    itemChange(Dw::ItemChildAddedChange, DwVariant((void*)item));
    notifyItemChange(Dw::ItemChildAddedChange, DwVariant((void*)item), DwVariant(NULL));

    // tell parent that the parent's childrenBoundingRect() will be dirty
    m_dirtyChildrenBoundingRect = 1;
    notifyParentRecursively(dw_pfn_entry_children_bounding_dirty);

    // tell children that the child's sceneTransform will be dirty
    item->m_dirtySceneTransform = 1;
    item->notifyChildrenRecursively(dw_pfn_entry_scene_transform_dirty);

    // tell item that the there is a new parent, so scene z is dirty
    item->m_dirtySceneZ = 1;
    item->notifyChildrenRecursively(dw_pfn_entry_scene_z_dirty);

    // tell parent that need sort child
    item->notifyParentRecursively(dw_pfn_entry_child_z_dirty);
}

//------------------------------------------------------------------------------
/**
	NB! do not call user
*/
void DwGraphicsItem::removeChild(DwGraphicsItem* item)
{
    DW_ASSERT(m_children.indexOf(item) != InvalidIndex);
    item->m_itemSiblingIndex = InvalidIndex;
    m_children.removeOne(item);

    // item change
    itemChange(Dw::ItemChildRemovedChange, DwVariant((void*)item));
    notifyItemChange(Dw::ItemChildRemovedChange, DwVariant((void*)item), DwVariant(NULL));

    // tell parent that the parent's childrenBoundingRect() will be dirty
    m_dirtyChildrenBoundingRect = 1;
    notifyParentRecursively(dw_pfn_entry_children_bounding_dirty);

    // tell children that the child's sceneTransform will be dirty
    item->m_dirtySceneTransform = 1;
    item->notifyChildrenRecursively(dw_pfn_entry_scene_transform_dirty);

    // tell item that the there is a new parent, so scene z is dirty
    item->m_dirtySceneZ = 1;
    item->notifyChildrenRecursively(dw_pfn_entry_scene_z_dirty);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItem::removeFromFocusLinkList(DwGraphicsItem* item)
{
    if (item)
    {
        if (item->m_focusPrev)
        {
            item->m_focusPrev->m_focusNext = item->m_focusNext;
        }
        if (item->m_focusNext)
        {
            item->m_focusNext->m_focusPrev = item->m_focusPrev;
        }
    }
}

//------------------------------------------------------------------------------
/**
	tell all of the parent , that a child have change the boundingrect
*/
bool dw_pfn_entry_children_bounding_dirty(DwGraphicsItem* item)
{
    if (item->m_dirtyChildrenBoundingRect|| item->m_inDestructor)
    {
        return false;
    }
    item->m_dirtyChildrenBoundingRect = 1;
    return true;
}

//------------------------------------------------------------------------------
/**
	tell all of the children , that a scene transform have change
*/
bool dw_pfn_entry_scene_transform_dirty(DwGraphicsItem* item)
{
    if (item->m_dirtySceneTransform || item->m_inDestructor)
    {
        return false;
    }
    item->m_dirtySceneTransform = 1;
    return true;
}

//------------------------------------------------------------------------------
/**
	tell all of the children , that a scene z have change
*/
bool dw_pfn_entry_scene_z_dirty(DwGraphicsItem* item)
{
    if (item->m_dirtySceneZ || item->m_inDestructor)
    {
        return false;
    }
    item->m_dirtySceneZ = 1;
    return true;
}

//------------------------------------------------------------------------------
/**
	tell parent, that a z have change
*/
bool dw_pfn_entry_child_z_dirty(DwGraphicsItem* item)
{
    item->m_dirtyChildZ = 1;
    return false;
}