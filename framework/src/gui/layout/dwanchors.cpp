#include "stable.h"
#include "dwanchors.h"
#include "graphicsview/dwgraphicsitem.h"
#include "graphicsview/dwdummys.h"

// NB! THE ALGORITHEM FROM QT4.7(DECLARATIVE)
//------------------------------------------------------------------------------
/**
	Internal Link
*/
static Dw::AnchorLine _dw_anchorIndexToLine(DwAnchors::AnchorIndex index)
{
#undef _Index_Of_Anchor
#define _Index_Of_Anchor(index) \
    case DwAnchors::##index :\
    return Dw::##index;\
    break
    switch (index)
    {
        _Index_Of_Anchor(Left);
        _Index_Of_Anchor(Right);
        _Index_Of_Anchor(Top);
        _Index_Of_Anchor(Bottom);
        _Index_Of_Anchor(HCenter);
        _Index_Of_Anchor(VCenter);
        _Index_Of_Anchor(Baseline);
    }
    return Dw::Invalid;
}

//------------------------------------------------------------------------------
/**
	Internal Link
*/
static DwAnchors::AnchorIndex _dw_anchorLineToIndex(Dw::AnchorLine line)
{
#undef _Index_Of_Anchor
#define _Index_Of_Anchor(line) \
	case Dw::##line :\
	return DwAnchors::##line;\
	break
    switch (line)
    {
        _Index_Of_Anchor(Left);
        _Index_Of_Anchor(Right);
        _Index_Of_Anchor(Top);
        _Index_Of_Anchor(Bottom);
        _Index_Of_Anchor(HCenter);
        _Index_Of_Anchor(VCenter);
        _Index_Of_Anchor(Baseline);
    }
    return DwAnchors::Invalid;
}

//------------------------------------------------------------------------------
/**
	Internal Link
*/
static Dw::Anchor _dw_anchorIndexToAnchor(DwAnchors::AnchorIndex index)
{
#undef _Index_Of_Anchor
#define _Index_Of_Anchor(index) \
    case DwAnchors::##index :\
    return Dw::##index##Anchor;\
    break
    switch (index)
    {
        _Index_Of_Anchor(Left);
        _Index_Of_Anchor(Right);
        _Index_Of_Anchor(Top);
        _Index_Of_Anchor(Bottom);
        _Index_Of_Anchor(HCenter);
        _Index_Of_Anchor(VCenter);
        _Index_Of_Anchor(Baseline);
    }
    return Dw::InvalidAnchor;
}

//------------------------------------------------------------------------------
/**
	Internal Link
*/
static DwAnchors::AnchorIndex _dw_anchorToAnchorIndex(Dw::Anchor anchor)
{
#undef _Index_Of_Anchor
#define _Index_Of_Anchor(anchor) \
	case Dw::##anchor##Anchor :\
	return DwAnchors::##anchor;\
	break
    switch (anchor)
    {
        _Index_Of_Anchor(Left);
        _Index_Of_Anchor(Right);
        _Index_Of_Anchor(Top);
        _Index_Of_Anchor(Bottom);
        _Index_Of_Anchor(HCenter);
        _Index_Of_Anchor(VCenter);
        _Index_Of_Anchor(Baseline);
    }
    return DwAnchors::Invalid;
}

//------------------------------------------------------------------------------
/**
*/
DwAnchors::DwAnchors(DwGraphicsItem* item)
    : m_item(item)
    , m_fill(NULL)
    , m_centerIn(NULL)
    , m_usedAnchors(0)
    , m_componentComplete(true), m_updatingMe(false), m_updatingHorizontalAnchor(0)
    , m_updatingVerticalAnchor(0), m_updatingFill(0), m_updatingCenterIn(0)
{
    m_changes = (Dw::Geometry|Dw::Destroyed|Dw::Position);

    // the anchor is the pod struct
    memset((void*)m_anchors, 0, sizeof(m_anchors));
    // as the real 0 just equal to 0x00000000;
    memset((void*)m_margins, 0, sizeof(m_margins));

    // depend self
    addDepend(m_item);
}

//------------------------------------------------------------------------------
/**
*/
DwAnchors::~DwAnchors()
{
    remAllDepends();
}

//------------------------------------------------------------------------------
/**
*/
DwAnchorLine DwAnchors::left() const
{
    return m_anchors[Left];
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setLeft(const DwAnchorLine &edge)
{
    setAnchor(Dw::LeftAnchor, edge);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::resetLeft()
{
    setLeft(DwAnchorLine());
}

//------------------------------------------------------------------------------
/**
*/
DwAnchorLine DwAnchors::right() const
{
    return m_anchors[Right];
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setRight(const DwAnchorLine &edge)
{
    setAnchor(Dw::RightAnchor, edge);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::resetRight()
{
    setRight(DwAnchorLine());
}

//------------------------------------------------------------------------------
/**
*/
DwAnchorLine DwAnchors::horizontalCenter() const
{
    return m_anchors[HCenter];
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setHorizontalCenter(const DwAnchorLine &edge)
{
    setAnchor(Dw::HCenterAnchor, edge);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::resetHorizontalCenter()
{
    setHorizontalCenter(DwAnchorLine());
}

//------------------------------------------------------------------------------
/**
*/
DwAnchorLine DwAnchors::top() const
{
    return m_anchors[Top];
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setTop(const DwAnchorLine &edge)
{
    setAnchor(Dw::TopAnchor, edge);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::resetTop()
{
    setTop(DwAnchorLine());
}

//------------------------------------------------------------------------------
/**
*/
DwAnchorLine DwAnchors::bottom() const
{
    return m_anchors[Bottom];
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setBottom(const DwAnchorLine &edge)
{
    setAnchor(Dw::BottomAnchor, edge);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::resetBottom()
{
    setBottom(DwAnchorLine());
}

//------------------------------------------------------------------------------
/**
*/
DwAnchorLine DwAnchors::verticalCenter() const
{
    return m_anchors[VCenter];
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setVerticalCenter(const DwAnchorLine &edge)
{
    setAnchor(Dw::VCenterAnchor, edge);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::resetVerticalCenter()
{
    setVerticalCenter(DwAnchorLine());
}

//------------------------------------------------------------------------------
/**
*/
DwAnchorLine DwAnchors::baseline() const
{
    return m_anchors[Baseline];
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setBaseline(const DwAnchorLine &edge)
{
    setAnchor(Dw::BaselineAnchor, edge);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::resetBaseline()
{
    setBaseline(DwAnchorLine());
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwAnchors::leftMargin() const
{
    return marginOf(Dw::LeftAnchor);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setLeftMargin(DwReal margin)
{
    setMargin(Dw::LeftAnchor, margin);
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwAnchors::rightMargin() const
{
    return marginOf(Dw::RightAnchor);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setRightMargin(DwReal margin)
{
    setMargin(Dw::RightAnchor, margin);
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwAnchors::horizontalCenterOffset() const
{
    return marginOf(Dw::HCenterAnchor);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setHorizontalCenterOffset(DwReal margin)
{
    setMargin(Dw::HCenterAnchor, margin);
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwAnchors::topMargin() const
{
    return marginOf(Dw::TopAnchor);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setTopMargin(DwReal margin)
{
    setMargin(Dw::TopAnchor, margin);
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwAnchors::bottomMargin() const
{
    return marginOf(Dw::BottomAnchor);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setBottomMargin(DwReal margin)
{
    setMargin(Dw::BottomAnchor, margin);
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwAnchors::margins() const
{
    return m_margins[IndexMax];
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setMargins(DwReal offset)
{
    if (m_margins[IndexMax] == offset)
        return;

    //###Is it significantly faster to set them directly so we can call fillChanged only once?
    if(!m_margins[Right] || m_margins[Right] == m_margins[IndexMax])
        setRightMargin(offset);
    if(!m_margins[Left] || m_margins[Left] == m_margins[IndexMax])
        setLeftMargin(offset);
    if(!m_margins[Top] || m_margins[Top] == m_margins[IndexMax])
        setTopMargin(offset);
    if(!m_margins[Bottom] || m_margins[Bottom] == m_margins[IndexMax])
        setBottomMargin(offset);
    m_margins[IndexMax] = offset;
    //emit marginsChanged();
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwAnchors::verticalCenterOffset() const
{
    return marginOf(Dw::VCenterAnchor);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setVerticalCenterOffset(DwReal margin)
{
    setMargin(Dw::VCenterAnchor, margin);
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwAnchors::baselineOffset() const
{
    return marginOf(Dw::BaselineAnchor);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setBaselineOffset(DwReal margin)
{
    setMargin(Dw::BaselineAnchor, margin);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwAnchors::fill() const
{
    return m_fill;
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setFill(DwGraphicsItem * f)
{
    if (m_fill == f)
        return;

    if (!f)
    {
        remAnchorDepend(m_fill, Fill);
        m_fill = f;
        //emit fillChanged();
        return;
    }
    if (f != m_item->parentItem() && f->parentItem() != m_item->parentItem())
    {
        DW_UI_LOGER() << m_item->name() << DW_TR("Cannot anchor to an item that isn't a parent or sibling.");
        return;
    }
    if(m_fill) remAnchorDepend(m_fill, Fill);
    m_fill = f;
    if(f) addAnchorDepend(f, Fill);
    //emit fillChanged();
    fillChanged();
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::resetFill()
{
    setFill(NULL);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwAnchors::centerIn() const
{
    return m_centerIn;
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setCenterIn(DwGraphicsItem *c)
{
    if (m_centerIn == c)
        return;

    if (!c)
    {
        remAnchorDepend(m_centerIn, CenterIn);
        m_centerIn = c;
        //emit centerInChanged();
        return;
    }
    if (c != m_item->parentItem() && c->parentItem() != m_item->parentItem())
    {
        DW_UI_LOGER() << m_item->name() << DW_TR("Cannot anchor to an item that isn't a parent or sibling.");
        return;
    }

    if(m_centerIn) remAnchorDepend(m_centerIn, CenterIn);
    m_centerIn = c;
    if(c) addAnchorDepend(c, CenterIn);
    //emit centerInChanged();
    centerInChanged();
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::resetCenterIn()
{
    setCenterIn(NULL);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::onChange(Dw::ChangeType change,
                         DwGraphicsItem* item, const DwVariant& newOne, const DwVariant& oldOne)
{
    if (change == Dw::Geometry)
    {
        DwRectF newRect = newOne.toRectF();
        DwRectF oldRect = oldOne.toRectF();

        // we only care about parent width and height change
        if (item == m_item->parentItem()
                && dwFuzzyCompare(newRect.width() , oldRect.width())
                && dwFuzzyCompare(newRect.height() , oldRect.height()))
        {
            return;
        }

        //itemGeometryChanged(item, newRect, oldRect);
        updateMe();
    }
    else if (change == Dw::Position)
    {
        // we only care about parent width and height change
        if ( item == m_item->parentItem() )
        {
            return;
        }

        /*DwRectF newRect = DwRectF(newOne.toPointF(), item->size());
        DwRectF oldRect = DwRectF(oldOne.toPointF(), item->size());
        itemGeometryChanged(item, newRect, oldRect);*/
        updateMe();
    }
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwAnchors::onAdded(DwGraphicsItem* item)
{
    DW_UNUSED(item);
    //DW_ASSERT(item != m_item);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::onRemoved(DwGraphicsItem* item)
{
    clearItem(item);

    if (m_dependsMap.contains(item))
    {
        m_dependsMap.remove(item);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setAnchor(Dw::Anchor anchor, const DwAnchorLine &edge)
{
    AnchorIndex index = _dw_anchorToAnchorIndex(anchor);
    DW_ASSERT(index >=0 && index < IndexMax);

    // no change
    if ( m_anchors[index] == edge )
        return;

    // check valid
    if (anchor & Dw::HorizontalAnchor_Mask)
    {
        if (!checkHAnchorValid(edge))
            return;
    }
    else
    {
        DW_ASSERT(anchor & Dw::VerticalAnchor_Mask);
        if (!checkVAnchorValid(edge) || !checkVValid())
            return;
    }

    m_usedAnchors |= anchor;

    // if not valid anchor, clear it
    if (anchor & Dw::HorizontalAnchor_Mask)
    {
        if ( !checkHValid())
        {
            m_usedAnchors &= ~anchor;
            return;
        }
    }
    else
    {
        DW_ASSERT(anchor & Dw::VerticalAnchor_Mask);
        if ( !checkVValid())
        {
            m_usedAnchors &= ~anchor;
            return;
        }
    }

    // set anchor
    if(m_anchors[index].item) remAnchorDepend(m_anchors[index].item, index);
    m_anchors[index] = edge;
    if( edge.item ) addAnchorDepend(edge.item, index);
    // TODO: emit change

    // update
    if (anchor & Dw::HorizontalAnchor_Mask)
    {
        updateHorizontalAnchors();
    }
    else
    {
        DW_ASSERT(anchor & Dw::VerticalAnchor_Mask);
        updateVerticalAnchors();
    }
}

//------------------------------------------------------------------------------
/**
*/
DwAnchorLine DwAnchors::anchorOf(Dw::Anchor anchor) const
{
    AnchorIndex index = _dw_anchorToAnchorIndex(anchor);
    DW_ASSERT(index >=0 && index < IndexMax);
    return m_anchors[index];
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setMargin(Dw::Anchor anchor, DwReal margin)
{
    AnchorIndex index = _dw_anchorToAnchorIndex(anchor);
    DW_ASSERT(index >=0 && index < IndexMax);

    if (m_margins[index] == margin)
    {
        return;
    }

    m_margins[index] = margin;

    if (anchor & Dw::CenterAnchor_Mask)
    {
        if (m_centerIn)
        {
            centerInChanged();
        }
        else
        {
            if (anchor & Dw::HorizontalAnchor_Mask)
            {
                updateHorizontalAnchors();
            }
            else
            {
                DW_ASSERT(anchor & Dw::VerticalAnchor_Mask);
                updateVerticalAnchors();
            }
        }
    }
    else
    {
        if (m_fill)
        {
            fillChanged();
        }
        else
        {
            if (anchor & Dw::HorizontalAnchor_Mask)
            {
                updateHorizontalAnchors();
            }
            else
            {
                DW_ASSERT(anchor & Dw::VerticalAnchor_Mask);
                updateVerticalAnchors();
            }
        }
    }
    //TODO emit Change
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwAnchors::marginOf(Dw::Anchor anchor) const
{
    AnchorIndex index = _dw_anchorToAnchorIndex(anchor);
    DW_ASSERT(index >=0 && index < IndexMax);
    return m_margins[index];
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::addAnchorDepend(DwGraphicsItem* item, AnchorIndex index)
{
    if (index >= Left && index < IndexMax)
    {
        Dw::Anchor anchor = _dw_anchorIndexToAnchor(index);
        m_usedAnchors |= anchor;
    }

    if (!m_dependsMap.contains(item))
    {
        addDepend(item);
        m_dependsMap[item] = 1;
    }
    else
    {
        ++m_dependsMap[item];
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::remAnchorDepend(DwGraphicsItem* item, AnchorIndex index)
{
    DW_ASSERT(m_dependsMap.contains(item));

    if (index >= Left && index < IndexMax)
    {
        Dw::Anchor anchor = _dw_anchorIndexToAnchor(index);
        m_usedAnchors &= ~anchor;
    }

    int& refCount = m_dependsMap[item];
    if (refCount == 1)
    {
        m_dependsMap.remove(item);
        remDepend(item);
    }
    else
    {
        --refCount;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::clearItem(DwGraphicsItem *item)
{
    if (!item)
        return;
    if (m_fill == item)
        m_fill = 0;
    if (m_centerIn == item)
        m_centerIn = 0;

    for (int i=0; i<IndexMax; ++i)
    {
        if (m_anchors[i].item == item)
        {
            m_anchors[i].item = NULL;
            m_usedAnchors &= ~(_dw_anchorIndexToAnchor((AnchorIndex)i));
        }
    }
}

void DwAnchors::clearItem(DwGraphicsItem *item, AnchorIndex index)
{
    DW_UNUSED(item);
    if (index == Fill)
    {
        DW_ASSERT(m_fill == item);
        m_fill = 0;
    }
    else if (index == CenterIn)
    {
        DW_ASSERT(m_centerIn == item);
        m_centerIn = NULL;
    }
    else if (index >= Left && index < IndexMax)
    {
        DW_ASSERT(m_anchors[index].item == item);
        m_anchors[index].item = NULL;
        m_usedAnchors &= ~(_dw_anchorIndexToAnchor(index));
    }

}

//------------------------------------------------------------------------------
/**
*/
bool DwAnchors::isItemComplete() const
{
    return m_componentComplete;
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::classBegin()
{
    m_componentComplete = false;
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::componentComplete()
{
    m_componentComplete = true;
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setItemHeight(DwReal h)
{
    DW_ASSERT(m_item);
    m_updatingMe = true;
    m_item->setHeight(h);
    m_updatingMe = false;
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setItemWidth(DwReal w)
{
    DW_ASSERT(m_item);
    m_updatingMe = true;
    m_item->setWidth(w);
    m_updatingMe = false;
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setItemX(DwReal x)
{
    DW_ASSERT(m_item);
    m_updatingMe = true;
    m_item->setX(x);
    m_updatingMe = false;
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setItemY(DwReal y)
{
    DW_ASSERT(m_item);
    m_updatingMe = true;
    m_item->setY(y);
    m_updatingMe = false;
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::setItemPos(const DwPointF & p)
{
    DW_ASSERT(m_item);
    m_updatingMe = true;
    m_item->setPos(p);
    m_updatingMe = false;
}

//------------------------------------------------------------------------------
/**
	TODO: scale orginal boundingrect
*/
void DwAnchors::setItemSize(const DwSizeF &r)
{
    DW_ASSERT(m_item);
    m_updatingMe = true;
    m_item->setSize(r);
    m_updatingMe = false;
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwAnchors::itemGeometryChanged(DwGraphicsItem *, const DwRectF &newG, const DwRectF &oldG)
{
    fillChanged();
    centerInChanged();
    if (newG.x() != oldG.x() || newG.width() != oldG.width())
        updateHorizontalAnchors();
    if (newG.y() != oldG.y() || newG.height() != oldG.height())
        updateVerticalAnchors();
}

//------------------------------------------------------------------------------
/**
*/
bool DwAnchors::checkHValid() const
{
    if (m_usedAnchors & Dw::LeftAnchor &&
            m_usedAnchors & Dw::RightAnchor &&
            m_usedAnchors & Dw::HCenterAnchor)
    {
        DW_UI_LOGER() << m_item->name() << DW_TR("Cannot specify left, right, and hcenter anchors.");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwAnchors::checkVValid() const
{
    if (m_usedAnchors & Dw::TopAnchor &&
            m_usedAnchors & Dw::BottomAnchor &&
            m_usedAnchors & Dw::VCenterAnchor)
    {
        DW_UI_LOGER() << m_item->name() << DW_TR("Cannot specify top, bottom, and vcenter anchors.");
        return false;
    }
    else if (m_usedAnchors & Dw::BaselineAnchor &&
             (m_usedAnchors & Dw::TopAnchor ||
              m_usedAnchors & Dw::BottomAnchor ||
              m_usedAnchors & Dw::VCenterAnchor))
    {
        DW_UI_LOGER() << m_item->name() << DW_TR("Baseline anchor cannot be used in conjunction with top, bottom, or vcenter anchors.");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwAnchors::checkHAnchorValid(DwAnchorLine anchor) const
{
    if (!anchor.item)
    {
        DW_UI_LOGER() << m_item->name() << DW_TR("Cannot anchor to a null item.");
        return false;
    }
    else if (anchor.anchorLine & Dw::Vertical_Mask)
    {
        DW_UI_LOGER() << m_item->name() << DW_TR("Cannot anchor a horizontal edge to a vertical edge.");
        return false;
    }
    else if (anchor.item != m_item->parentItem() && anchor.item->parentItem() != m_item->parentItem())
    {
        DW_UI_LOGER() << m_item->name() << DW_TR("Cannot anchor to an item that isn't a parent or sibling.");
        return false;
    }
    else if (anchor.item == m_item)
    {
        DW_UI_LOGER() << m_item->name() << DW_TR("Cannot anchor item to self.");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwAnchors::checkVAnchorValid(DwAnchorLine anchor) const
{
    if (!anchor.item)
    {
        DW_UI_LOGER() << m_item->name() << DW_TR("Cannot anchor to a null item.");
        return false;
    }
    else if (anchor.anchorLine & Dw::Horizontal_Mask)
    {
        DW_UI_LOGER() << m_item->name() << DW_TR("Cannot anchor a vertical edge to a horizontal edge.");
        return false;
    }
    else if (anchor.item != m_item->parentItem() && anchor.item->parentItem() != m_item->parentItem())
    {
        DW_UI_LOGER() << m_item->name() << DW_TR("Cannot anchor to an item that isn't a parent or sibling.");
        return false;
    }
    else if (anchor.item == m_item)
    {
        DW_UI_LOGER() << m_item->name() << DW_TR("Cannot anchor item to self.");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
	Internal Linkage
*/
static DwReal _dw_hcenter(DwGraphicsItem *item)
{
    DwReal width = item->width();
    int iw = (int)width;
    if (iw % 2)
        return (width + 1) / 2;
    else
        return width / 2;
}

//------------------------------------------------------------------------------
/**
	Internal Linkage
*/
static DwReal _dw_vcenter(DwGraphicsItem *item)
{
    DwReal height = item->height();
    int ih = (int)height;
    if (ih % 2)
        return (height + 1) / 2;
    else
        return height / 2;
}

//------------------------------------------------------------------------------
/**
	Internal Linkage, local position
*/
//### const item?
static DwReal _dw_position(DwGraphicsItem *item, Dw::AnchorLine anchorLine)
{
    DwReal ret = 0.0;
    switch(anchorLine)
    {
    case Dw::Left:
        ret = item->x();
        break;
    case Dw::Right:
        ret = item->x() + item->width();
        break;
    case Dw::Top:
        ret = item->y();
        break;
    case Dw::Bottom:
        ret = item->y() + item->height();
        break;
    case Dw::HCenter:
        ret = item->x() + _dw_hcenter(item);
        break;
    case Dw::VCenter:
        ret = item->y() + _dw_vcenter(item);
        break;
    case Dw::Baseline:
        ret = item->y() + item->baselineOffset();
        break;
    default:
        break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/**
	Internal Linkage, position when origin is 0,0
*/
static DwReal _dw_adjustedPosition(DwGraphicsItem *item, Dw::AnchorLine anchorLine)
{
    DwReal ret = 0.0;
    switch(anchorLine)
    {
    case Dw::Left:
        ret = 0.0;
        break;
    case Dw::Right:
        ret = item->width();
        break;
    case Dw::Top:
        ret = 0.0;
        break;
    case Dw::Bottom:
        ret = item->height();
        break;
    case Dw::HCenter:
        ret = _dw_hcenter(item);
        break;
    case Dw::VCenter:
        ret = _dw_vcenter(item);
        break;
    case Dw::Baseline:
        ret = item->baselineOffset();
        break;
    default:
        break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/**
*/
bool DwAnchors::calcStretch(const DwAnchorLine &edge1, const DwAnchorLine &edge2,
                            DwReal offset1, DwReal offset2, Dw::AnchorLine line, DwReal &stretch)
{
    bool edge1IsParent = (edge1.item == m_item->parentItem());
    bool edge2IsParent = (edge2.item == m_item->parentItem());
    bool edge1IsSibling = (edge1.item->parentItem() == m_item->parentItem());
    bool edge2IsSibling = (edge2.item->parentItem() == m_item->parentItem());

    bool invalid = false;
    if ((edge2IsParent && edge1IsParent) || (edge2IsSibling && edge1IsSibling))
    {
        stretch = (_dw_position(edge2.item, edge2.anchorLine) + offset2)
                  - (_dw_position(edge1.item, edge1.anchorLine) + offset1);
    }
    else if (edge2IsParent && edge1IsSibling)
    {
        stretch = (_dw_position(edge2.item, edge2.anchorLine) + offset2)
                  - (_dw_position(m_item->parentItem(), line)
                     + _dw_position(edge1.item, edge1.anchorLine) + offset1);
    }
    else if (edge2IsSibling && edge1IsParent)
    {
        stretch = (_dw_position(m_item->parentItem(), line)
                   + _dw_position(edge2.item, edge2.anchorLine) + offset2)
                  - (_dw_position(edge1.item, edge1.anchorLine) + offset1);
    }
    else
        invalid = true;

    return invalid;
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::updateMe()
{
    if (m_updatingMe)
    {
        m_updatingMe = false;
        return;
    }

    fillChanged();
    centerInChanged();
    updateHorizontalAnchors();
    updateVerticalAnchors();
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::updateOnComplete()
{
    fillChanged();
    centerInChanged();
    updateHorizontalAnchors();
    updateVerticalAnchors();
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwAnchors::updateHorizontalAnchors()
{
    if (m_fill || m_centerIn || !isItemComplete())
        return;

    DwAnchorLine& left = m_anchors[Left];
    DwAnchorLine& right = m_anchors[Right];
    DwAnchorLine& hCenter = m_anchors[HCenter];
    DwReal& leftMargin = m_margins[Left];
    DwReal& rightMargin = m_margins[Right];
    DwReal& hCenterOffset = m_margins[HCenter];

    if (m_updatingHorizontalAnchor < 2)
    {
        ++m_updatingHorizontalAnchor;
        if (m_usedAnchors & Dw::LeftAnchor)
        {
            //Handle stretching
            bool invalid = true;
            DwReal width = 0.0;
            if (m_usedAnchors & Dw::RightAnchor)
            {
                invalid = calcStretch(left, right, leftMargin, -rightMargin, Dw::Left, width);
            }
            else if (m_usedAnchors & Dw::HCenterAnchor)
            {
                invalid = calcStretch(left, hCenter, leftMargin, hCenterOffset, Dw::Left, width);
                width *= 2;
            }
            if (!invalid)
                setItemWidth(width);

            //Handle left
            if (left.item == m_item->parentItem())
            {
                setItemX(_dw_adjustedPosition(left.item, left.anchorLine) + leftMargin);
            }
            else if (left.item->parentItem() == m_item->parentItem())
            {
                setItemX(_dw_position(left.item, left.anchorLine) + leftMargin);
            }
        }
        else if (m_usedAnchors & Dw::RightAnchor)
        {
            //Handle stretching (left + right case is handled in updateLeftAnchor)
            if (m_usedAnchors & Dw::HCenterAnchor)
            {
                DwReal width = 0.0;
                bool invalid = calcStretch(hCenter, right, hCenterOffset, -rightMargin,
                                           Dw::Left, width);
                if (!invalid)
                    setItemWidth(width*2);
            }

            //Handle right
            if (right.item == m_item->parentItem())
            {
                setItemX(_dw_adjustedPosition(right.item, right.anchorLine) - m_item->width() - rightMargin);
            }
            else if (right.item->parentItem() == m_item->parentItem())
            {
                setItemX(_dw_position(right.item, right.anchorLine) - m_item->width() - rightMargin);
            }
        }
        else if (m_usedAnchors & Dw::HCenterAnchor)
        {
            //Handle hCenter
            if (hCenter.item == m_item->parentItem())
            {
                setItemX(_dw_adjustedPosition(hCenter.item, hCenter.anchorLine) - _dw_hcenter(m_item) + hCenterOffset);
            }
            else if (hCenter.item->parentItem() == m_item->parentItem())
            {
                setItemX(_dw_position(hCenter.item, hCenter.anchorLine) - _dw_hcenter(m_item) + hCenterOffset);
            }
        }

        --m_updatingHorizontalAnchor;
    }
    else
    {
        // ### Make this certain :)
        DW_UI_LOGER() << m_item->name() << DW_TR("Possible anchor loop detected on horizontal anchor.");
    }
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwAnchors::updateVerticalAnchors()
{
    if (m_fill || m_centerIn || !isItemComplete())
        return;

    DwAnchorLine& top = m_anchors[Top];
    DwAnchorLine& bottom = m_anchors[Bottom];
    DwAnchorLine& vCenter = m_anchors[VCenter];
    DwAnchorLine& baseline = m_anchors[Baseline];

    DwReal& topMargin = m_margins[Top];
    DwReal& bottomMargin = m_margins[Bottom];
    DwReal& vCenterOffset = m_margins[VCenter];
    DwReal& baselineOffset = m_margins[Baseline];

    if (m_updatingVerticalAnchor < 2)
    {
        ++m_updatingVerticalAnchor;
        if (m_usedAnchors & Dw::TopAnchor)
        {
            //Handle stretching
            bool invalid = true;
            DwReal height = 0.0;
            if (m_usedAnchors & Dw::BottomAnchor)
            {
                invalid = calcStretch(top, bottom, topMargin, -bottomMargin, Dw::Top, height);
            }
            else if (m_usedAnchors & Dw::VCenterAnchor)
            {
                invalid = calcStretch(top, vCenter, topMargin, vCenterOffset, Dw::Top, height);
                height *= 2;
            }
            if (!invalid)
                setItemHeight(height);

            //Handle top
            if (top.item == m_item->parentItem())
            {
                setItemY(_dw_adjustedPosition(top.item, top.anchorLine) + topMargin);
            }
            else if (top.item->parentItem() == m_item->parentItem())
            {
                setItemY(_dw_position(top.item, top.anchorLine) + topMargin);
            }
        }
        else if (m_usedAnchors & Dw::BottomAnchor)
        {
            //Handle stretching (top + bottom case is handled above)
            if (m_usedAnchors & Dw::VCenterAnchor)
            {
                DwReal height = 0.0;
                bool invalid = calcStretch(vCenter, bottom, vCenterOffset, -bottomMargin,
                                           Dw::Top, height);
                if (!invalid)
                    setItemHeight(height*2);
            }

            //Handle bottom
            if (bottom.item == m_item->parentItem())
            {
                setItemY(_dw_adjustedPosition(bottom.item, bottom.anchorLine) - m_item->height() - bottomMargin);
            }
            else if (bottom.item->parentItem() == m_item->parentItem())
            {
                setItemY(_dw_position(bottom.item, bottom.anchorLine) - m_item->height() - bottomMargin);
            }
        }
        else if (m_usedAnchors & Dw::VCenterAnchor)
        {
            //(stetching handled above)

            //Handle vCenter
            if (vCenter.item == m_item->parentItem())
            {
                setItemY(_dw_adjustedPosition(vCenter.item, vCenter.anchorLine)
                         - _dw_vcenter(m_item) + vCenterOffset);
            }
            else if (vCenter.item->parentItem() == m_item->parentItem())
            {
                setItemY(_dw_position(vCenter.item, vCenter.anchorLine) - _dw_vcenter(m_item) + vCenterOffset);
            }
        }
        else if (m_usedAnchors & Dw::BaselineAnchor)
        {
            //Handle baseline
            if (baseline.item == m_item->parentItem())
            {
                setItemY(_dw_adjustedPosition(baseline.item, baseline.anchorLine)
                         - m_item->baselineOffset() + baselineOffset);
            }
            else if (baseline.item->parentItem() == m_item->parentItem())
            {
                setItemY(_dw_position(baseline.item, baseline.anchorLine)
                         - m_item->baselineOffset() + baselineOffset);
            }
        }
        --m_updatingVerticalAnchor;
    }
    else
    {
        // ### Make this certain :)
        DW_UI_LOGER() << m_item->name() << DW_TR("Possible anchor loop detected on vertical anchor.");
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::fillChanged()
{
    if (!m_fill || !isItemComplete())
        return;

    DwReal& topMargin = m_margins[Top];
    DwReal& bottomMargin = m_margins[Bottom];
    DwReal& leftMargin = m_margins[Left];
    DwReal& rightMargin = m_margins[Right];

    if (m_updatingFill < 2)
    {
        ++m_updatingFill;

        if (m_fill == m_item->parentItem())
        {
            //child-parent
            setItemPos(DwPointF(leftMargin, topMargin));
        }
        else if (m_fill->parentItem() == m_item->parentItem())
        {
            //siblings
            setItemPos(DwPointF(m_fill->x()+leftMargin, m_fill->y()+topMargin));
        }
        setItemSize(DwSizeF(m_fill->width()-leftMargin-rightMargin, m_fill->height()-topMargin-bottomMargin));

        --m_updatingFill;
    }
    else
    {
        // ### Make this certain :)
        DW_UI_LOGER() << m_item->name() << DW_TR("Possible anchor loop detected on fill.");
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAnchors::centerInChanged()
{
    if (!m_centerIn || m_fill || !isItemComplete())
        return;

    DwReal& vCenterOffset = m_margins[VCenter];
    DwReal& hCenterOffset = m_margins[HCenter];

    if (m_updatingCenterIn < 2)
    {
        ++m_updatingCenterIn;
        if (m_centerIn == m_item->parentItem())
        {
            DwPointF p(_dw_hcenter(m_item->parentItem()) - _dw_hcenter(m_item) + hCenterOffset,
                       _dw_vcenter(m_item->parentItem()) - _dw_vcenter(m_item) + vCenterOffset);
            setItemPos(p);

        }
        else if (m_centerIn->parentItem() == m_item->parentItem())
        {
            DwPointF p(m_centerIn->x() + _dw_hcenter(m_centerIn) - _dw_hcenter(m_item) + hCenterOffset,
                       m_centerIn->y() + _dw_vcenter(m_centerIn) - _dw_vcenter(m_item) + vCenterOffset);
            setItemPos(p);
        }

        --m_updatingCenterIn;
    }
    else
    {
        // ### Make this certain :)
        DW_UI_LOGER() << m_item->name() << DW_TR("Possible anchor loop detected on centerIn.");
    }
}