#include "stable.h"
#include "dwgraphicssceneindex.h"
#include "dwgraphicsscene.h"
#include "dwgraphicsitem.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_ABSTRACT_CLASS(DwGraphicsSceneIndex, 'GSIX', DwRttiObject);

//------------------------------------------------------------------------------
class DwGraphicsSceneIndexRectIntersector : public DwGraphicsSceneIndexIntersector
{
public:
    // simple implement
    // TODO: think over no transformable item,
    // optimize bounding, more deep and more accurate intersect check
    bool intersect(const DwGraphicsItem *item, const DwRectF &exposeRect, Dw::ItemSelectionMode mode,
                   const DwTransform &deviceTransform) const
    {
        DW_ASSERT(item);
        DW_UNUSED(exposeRect);
        DW_UNUSED(deviceTransform);

        DwRectF brect = item->boundingRect();

        bool keep = true;
        const DwRectF itemSceneBoundingRect = item->sceneBoundingRect();
        if (mode == Dw::ContainsItemShape || mode == Dw::ContainsItemBoundingRect)
        {
            keep = sceneRect != brect && sceneRect.contains(itemSceneBoundingRect);
        }
        else
        {
            keep = sceneRect.intersects(itemSceneBoundingRect);
        }

        return keep;
    }

    DwRectF sceneRect;
};

//------------------------------------------------------------------------------
class DwGraphicsSceneIndexPointIntersector : public DwGraphicsSceneIndexIntersector
{
public:
    // simple implement
    // TODO: optimize, item's scene transform only deliver the translated
    bool intersect(const DwGraphicsItem *item, const DwRectF &exposeRect, Dw::ItemSelectionMode mode,
                   const DwTransform &deviceTransform) const
    {
        DW_ASSERT(item);
        DW_UNUSED(exposeRect);
        DW_UNUSED(mode);
        DW_UNUSED(deviceTransform);

        DwRectF sceneBoundingRect = item->sceneBoundingRect();
        bool keep = sceneBoundingRect.intersects(DwRectF(scenePoint, DwSizeF(1, 1)));
        if (keep)
        {
            DwPointF p = item->sceneTransform().inverted().map(scenePoint);
            keep = item->boundingRect().contains(p);
        }
        return keep;
    }

    DwPointF scenePoint;
};

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSceneIndex::DwGraphicsSceneIndex(DwGraphicsScene *scene/* = 0*/)
    : m_scene (NULL)
    , m_pointIntersector(NULL)
    , m_rectIntersector(NULL)
{
    m_scene = scene;
    m_pointIntersector = new DwGraphicsSceneIndexPointIntersector;
    m_rectIntersector = new DwGraphicsSceneIndexRectIntersector;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSceneIndex::~DwGraphicsSceneIndex()
{
    delete m_pointIntersector;
    m_pointIntersector = NULL;
    delete m_rectIntersector;
    m_rectIntersector = NULL;
    m_scene =NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsScene * DwGraphicsSceneIndex::scene() const
{
    return m_scene;
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwGraphicsItem *> DwGraphicsSceneIndex::items(const DwPointF &pos, Dw::ItemSelectionMode mode,
        Dw::SortOrder order, const DwTransform &deviceTransform/* = DwTransform()*/) const
{
    DwVector<DwGraphicsItem *> itemList;
    m_pointIntersector->scenePoint = pos;
    items_helper(DwRectF(pos, DwSizeF(1., 1.)), m_pointIntersector, &itemList, deviceTransform, mode, order);
    return itemList;
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwGraphicsItem *> DwGraphicsSceneIndex::items(const DwRectF &rect, Dw::ItemSelectionMode mode,
        Dw::SortOrder order, const DwTransform &deviceTransform/* = DwTransform()*/) const
{
    DwRectF exposeRect = rect;
    DwVector<DwGraphicsItem *> itemList;
    m_rectIntersector->sceneRect = rect;
    items_helper(exposeRect, m_rectIntersector, &itemList, deviceTransform, mode, order);
    return itemList;
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwGraphicsItem *> DwGraphicsSceneIndex::estimateItems(const DwPointF &point, Dw::SortOrder order) const
{
    return estimateItems(DwRectF(point, DwSizeF(1.0, 1.0)), order);
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwGraphicsItem *> DwGraphicsSceneIndex::estimateTopLevelItems(const DwRectF &rect, Dw::SortOrder order) const
{
    DW_UNUSED(rect);
    DW_UNUSED(order);
    // be scene top level item sorted as descending order
    m_scene->ensureSortedTopLevelItem();
    DwVector<DwGraphicsItem *> topItems = m_scene->topLevelItems();
    // if require ascending order, sorted it
    sort_items_helper(&topItems, order);
    return topItems;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneIndex::clear()
{
    DwVector<DwGraphicsItem *> allItems = items();
    for (int i = 0 ; i < allItems.size(); ++i)
    {
        DwGraphicsItem* item = allItems.at(i);
        allItems[i] = NULL;
        removeItem(item);
    }
}

//------------------------------------------------------------------------------
/**
	TODO: optimize according to special situation
*/
void DwGraphicsSceneIndex::deleteItem(DwGraphicsItem* item)
{
    removeItem(item);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneIndex::prepareBoundingRectChange(const DwGraphicsItem *)
{
    ;// TODO:
}

//------------------------------------------------------------------------------
/**
	TODO: item change
*/
void DwGraphicsSceneIndex::itemChange(const DwGraphicsItem *item, Dw::GraphicsItemChange change, const DwVariant& newOne, const DwVariant& oldOne)
{
    DW_UNUSED(item);
    DW_UNUSED(change);
    DW_UNUSED(newOne);
    DW_UNUSED(oldOne);
}

//------------------------------------------------------------------------------
/**
	TODO: opacity
	return the items is in descending order( the top at the front )
*/
void DwGraphicsSceneIndex::recursive_items_helper(DwGraphicsItem *item, DwRectF exposeRect,
        DwGraphicsSceneIndexIntersector *intersector, DwVector<DwGraphicsItem *> *items,
        const DwTransform &viewTransform,
        Dw::ItemSelectionMode mode, DwReal parentOpacity/* = 1.0*/) const
{
    DW_ASSERT(item);
    DW_UNUSED(parentOpacity);

    if (!item->isVisible())
        return;

    const DwReal opacity = parentOpacity;
    bool processItem = intersector->intersect(item, exposeRect, mode, viewTransform);

    const bool itemHasChildren = !item->children().isEmpty();
    if (processItem || itemHasChildren)
    {
        // NB! THE PROBLEM WILL BE AS THE CLIP RECT IS DIFFERENT FROM THE BOUNDING RECT
        const bool clipChilds = ((item->flags() & Dw::ItemClipsChildrenToShape) != 0);
        if (itemHasChildren && (!clipChilds || processItem))
        {
            // Process children behind
            // Children sort as asdencing order
            item->ensureSortedChildren();
            for (int i = 0; i < item->children().size(); ++i)
            {
                DwGraphicsItem *child = item->children().at(item->children().size() - i - 1);
                recursive_items_helper(child, exposeRect, intersector, items, viewTransform,
                                       mode, opacity);
            }
        }
        if (processItem)
        {
            items->append(item);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneIndex::items_helper(const DwRectF &rect, DwGraphicsSceneIndexIntersector *intersector,
                                        DwVector<DwGraphicsItem *> *oitems, const DwTransform &viewTransform,
                                        Dw::ItemSelectionMode mode, Dw::SortOrder order) const
{
    // collect item in descending order
    const DwVector<DwGraphicsItem *> tli = estimateTopLevelItems(rect, Dw::DescendingOrder);
    for (int i = 0; i < tli.size(); ++i)
    {
        recursive_items_helper(tli.at(i), rect, intersector, oitems, viewTransform, mode);
    }
    // sort item if require the ascending order
    sort_items_helper(oitems, order);
}

//------------------------------------------------------------------------------
/**
	NB! require oItems already sort in descending order
*/
void DwGraphicsSceneIndex::sort_items_helper(DwVector<DwGraphicsItem *> *oitems, Dw::SortOrder order)
{
    if (oitems->size() > 1)
    {
        if (order == Dw::AscendingOrder)
        {
            const int n = oitems->size();
            for (int i = 0; i < n / 2; ++i)
            {
                DwGraphicsItem* item = oitems->at(i);
                (*oitems)[i] = (*oitems)[n - i - 1];
                (*oitems)[n - i - 1] = item;
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneIndex::setGraphicsScene(DwGraphicsScene *scene)
{
    DW_ASSERT(items().size() == 0 && "SHOULD SET SCENE BEFRORE ANY ITEM ADD TO SCENE");
    m_scene = scene;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneIndex::debugString(DwDebug &stream)const
{
    stream  <<" " << this->rtti()->name() << "["
            <<" scene :" << (int)m_scene
            << " topLevelItemsNum :" << m_scene->topLevelItems().size()
            << " itemsNum:" << m_scene->items().size()
            << "]";
}