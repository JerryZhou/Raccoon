#include "stable.h"
#include "dwgraphics/dwpainter.h"
#include "dwbasepositioner.h"
#include "graphicsview/dwgraphicsutil.h"

DW_IMPLEMENT_CLASS(DwBasePositioner, 'GBPR', DwGraphicsItem);

// NB! THE ALGORITHEM FROM QT4.7(DECLARATIVE)
//------------------------------------------------------------------------------
/**
*/
DwBasePositioner::DwBasePositioner(DwGraphicsItem *parent/* = NULL*/, DwGraphicsScene *scene/* = NULL*/)
    : DwGraphicsItem(parent, scene)
    , m_spaceing(0.0)
    , m_queuedPositioning(0)
    , m_doingPositioning(0)
    , m_anchorConflict(0)
    , m_widthValid(0)
    , m_heightValid(0)
//, m_width(0.0)
//, m_height(0.0)
    , m_implicitWidth(0.0)
    , m_implicitHeight(0.0)
    , m_applyImplicitWidth(1)
    , m_applyImplicitHeight(1)
{
    m_positionType = DwBasePositioner::None;
    m_changes = (Dw::Geometry
                 | Dw::Destroyed	// TODO: xx
                 | Dw::SiblingOrder // TODO: xx
                 | Dw::Visibility
                 | Dw::Opacity); // TODO: xx

    setFlag(Dw::ItemHasNoContents, true);

    // accessible
    setAccessibleName(tr("raccoon Positioner"));
    setAccessibleDescription(tr("raccoon item container"));
    setWhatsThis(tr("raccoon engine's base positioner"));
}

//------------------------------------------------------------------------------
/**
*/
DwBasePositioner::~DwBasePositioner()
{
    unwatchAllChanges();
    //DW_ASSERT(m_positionedItems.size() == 0);
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwBasePositioner::spacing() const
{
    return m_spaceing;
}

//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::setSpacing(DwReal s)
{
    m_spaceing = s;

    schedulePositioning();
}

//------------------------------------------------------------------------------
/**
	In This Handler, will care about self's change
*/
DwVariant DwBasePositioner::itemChange(Dw::GraphicsItemChange change, const DwVariant &val)
{
    if (change == Dw::ItemChildAddedChange)
    {
        DwGraphicsItem *item = reinterpret_cast<DwGraphicsItem*>(val.toPtr());
        if (item)
        {
            prePositioning();
        }
    }
    else if (change == Dw::ItemChildRemovedChange)
    {
        DwGraphicsItem *item = reinterpret_cast<DwGraphicsItem*>(val.toPtr());
        if (item)
        {
            unwatchChanges(item);
            schedulePositioning();
        }
    }

    return DwGraphicsItem::itemChange(change, val);
}

//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option)
{
    DW_UNUSED(option);
    DW_UNUSED(painter);
    //painter->fillRect(boundingRect().toRect(), DwColor(32, 32, 128));
}

//------------------------------------------------------------------------------
/**
	In This Handler, will care all of child's change
*/
void DwBasePositioner::onChange(Dw::ChangeType change,
                                DwGraphicsItem* item, const DwVariant& newOne, const DwVariant& oldOne)
{
    DW_UNUSED(newOne);
    DW_UNUSED(oldOne);
    switch (change)
    {
    case Dw::Geometry :
        itemGeometryChanged(item, newOne.toRectF(), oldOne.toRectF());
        break;
    case Dw::Destroyed :
        itemDestroyed(item);
        break;
    case Dw::SiblingOrder :
        itemSiblingOrderChanged(item);
        break;
    case Dw::Visibility :
        itemVisibilityChanged(item);
        break;
    case Dw::Opacity :
        itemOpacityChanged(item);
        break;
    }
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwBasePositioner::onAdded(DwGraphicsItem* item)
{
    DW_UNUSED(item);
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwBasePositioner::onRemoved(DwGraphicsItem* item)
{
    DW_UNUSED(item);
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwBasePositioner::positionX(DwReal x, const PositionedItem &target)
{
    DW_UNUSED(x);
    DW_UNUSED(target);

    if(m_positionType == DwBasePositioner::Horizontal || m_positionType == DwBasePositioner::Both)
    {
        if (target.isNew)
        {
            target.item->setX(x);
        }
        else if (x != target.item->x())
        {
            target.item->setX(x);
        }
    }
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwBasePositioner::positionY(DwReal y,const PositionedItem &target)
{
    DW_UNUSED(y);
    DW_UNUSED(target);
    if(m_positionType == DwBasePositioner::Vertical || m_positionType == DwBasePositioner::Both)
    {
        if (target.isNew)
        {
            target.item->setY(y);
        }
        else if (y != target.item->y())
        {
            target.item->setY(y);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::setImplicitWidth(DwReal w)
{
    m_implicitWidth = w;

    if (isApplyImplicitWidth())
    {
        setWidth(w);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwBasePositioner::widthValid() const
{
    return m_widthValid;
}

//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::setImplicitHeight(DwReal h)
{
    m_implicitHeight = h;
    if (isApplayImplicitHeight())
    {
        setHeight(h);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwBasePositioner::heightValid() const
{
    return m_heightValid;
}

//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::setApplyImplicitWidth(bool b)
{
    m_applyImplicitWidth = b;
}

//------------------------------------------------------------------------------
/**
*/
bool DwBasePositioner::isApplyImplicitWidth()
{
    return m_applyImplicitWidth;
}

//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::setApplyImplicitHeight(bool b)
{
    m_applyImplicitHeight = b;
}

//------------------------------------------------------------------------------
/**
*/
bool DwBasePositioner::isApplayImplicitHeight()
{
    return m_applyImplicitHeight;
}


//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::watchChanges(DwGraphicsItem *other)
{
    DW_ASSERT(other);
    DW_ASSERT(other->parentItem() == this);
    addDepend(other);
}

//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::unwatchChanges(DwGraphicsItem* other)
{
    DW_ASSERT(other);
    DW_ASSERT(other->parentItem() == this);
    PositionedItem posItem(other);
    int idx = m_positionedItems.indexOf(posItem);
    if (idx >= 0)
    {

        m_positionedItems.remove(idx);
    }
    remDepend(other);
}

//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::unwatchAllChanges()
{
    remAllDepends();
}

//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::itemSiblingOrderChanged(DwGraphicsItem* other)
{
    DW_UNUSED(other);
    DW_ASSERT(other);
    DW_ASSERT(other->parentItem() == this);
    schedulePositioning();
}

//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::itemGeometryChanged(DwGraphicsItem *, const DwRectF &newGeometry, const DwRectF &oldGeometry)
{
    DW_UNUSED(newGeometry);
    DW_UNUSED(oldGeometry);
    schedulePositioning();
}

//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::itemVisibilityChanged(DwGraphicsItem *)
{
    schedulePositioning();
}

//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::itemOpacityChanged(DwGraphicsItem *)
{
    schedulePositioning();
}

//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::itemDestroyed(DwGraphicsItem *item)
{
    DW_UNUSED(item);
    schedulePositioning();
}

//------------------------------------------------------------------------------
/**
	TDOD: xx
*/
void DwBasePositioner::schedulePositioning()
{
    prePositioning();
}

//------------------------------------------------------------------------------
/**
*/
void DwBasePositioner::prePositioning()
{
    if (m_doingPositioning)
        return;

    m_queuedPositioning = false;
    m_doingPositioning = true;

    //Need to order children by creation order modified by stacking order
    DwVector<DwGraphicsItem *> children = this->children();
    if (children.size() > 1)
    {
        DwGraphicsUtil::sortChildBySiblingIndex(&children, Dw::AscendingOrder);
    }

    DwVector<PositionedItem> oldItems = m_positionedItems;
    m_positionedItems.clear();

    for (int ii = 0; ii < children.count(); ++ii)
    {
        DwGraphicsItem *child = children.at(ii);
        if (!child)
            continue;

        PositionedItem *item = 0;
        PositionedItem posItem(child);
        int wIdx = oldItems.indexOf(posItem);
        if (wIdx < 0)
        {
            watchChanges(child);
            m_positionedItems.append(posItem);
            item = &m_positionedItems[m_positionedItems.count()-1];
            item->isNew = true;
            if (!child->isVisible() || !child->width() || !child->height())
                item->isVisible = false;
        }
        else
        {
            item = &oldItems[wIdx];
            // Items are only omitted from positioning if they are explicitly hidden
            // i.e. their positioning is not affected if an ancestor is hidden.
            if (!child->isVisible() || !child->width() || !child->height())
            {
                item->isVisible = false;
            }
            else if (!item->isVisible)
            {
                item->isVisible = true;
                item->isNew = true;
            }
            else
            {
                item->isNew = false;
            }
            m_positionedItems.append(*item);
        }
    }

    DwSizeF contentSize;
    doPositioning(&contentSize);
    m_doingPositioning = false;
    //Set implicit size to the size of its children
    setImplicitHeight(contentSize.height());
    setImplicitWidth(contentSize.width());
}