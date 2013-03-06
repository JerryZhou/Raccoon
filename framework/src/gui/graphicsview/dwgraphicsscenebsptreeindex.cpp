#include "stable.h"
#include "dwdummys.h"
#include "dwgraphicsscenebsptreeindex.h"
#include "dwgraphicsscene.h"
#include "dwgraphicsitem.h"
#include "dwgraphicsutil.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwGraphicsSceneBspTreeIndex, 'GBSP', DwGraphicsSceneIndex);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSceneBspTreeIndex::DwGraphicsSceneBspTreeIndex(DwGraphicsScene* scene)
    : DwGraphicsSceneIndex(scene)
    , m_bspTreeDepth(0)
    , m_restartIndexTimer(false)
    , m_regenerateIndex(true)
    , m_lastItemCount(0)
    , m_sortCacheEnabled(false)
    , m_updatingSortCache(false)
    , m_purgePending(false)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSceneBspTreeIndex::~DwGraphicsSceneBspTreeIndex()
{
    for (int i = 0; i < m_indexedItems.size(); ++i)
    {
        // Ensure item bits are reset properly.
        if (DwGraphicsItem *item = m_indexedItems.at(i))
        {
            DW_ASSERT(!item->m_itemDiscovered);
            item->m_index = InvalidIndex;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
int DwGraphicsSceneBspTreeIndex::bspTreeDepth() const
{
    return m_bspTreeDepth;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::setBspTreeDepth(int depth)
{
    if(m_bspTreeDepth == depth) return;
    m_bspTreeDepth = depth;
    _dw_resetIndex();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::initialize(const DwRectF &rect)
{
    DW_ASSERT(m_bspTreeDepth > 0);
    m_bsp.initialize(rect, m_bspTreeDepth);
}

//------------------------------------------------------------------------------
/**
	TODO: append m_unindexedItems
*/
DwVector<DwGraphicsItem *> DwGraphicsSceneBspTreeIndex::items(Dw::SortOrder order/* = Dw::DescendingOrder*/) const
{
    DW_UNUSED(order);
    const_cast<DwGraphicsSceneBspTreeIndex*>(this)->_dw_purgeRemovedItems();
    DwVector<DwGraphicsItem *> itemList;

    // If freeItemIndexes is empty, we know there are no holes in indexedItems and
    // unindexedItems.
    if (m_freeItemIndexes.isEmpty())
    {
        if (m_unindexedItems.isEmpty())
        {
            itemList = m_indexedItems;
        }
        else
        {
            itemList = m_indexedItems;
            for (int i = 0; i < m_unindexedItems.size(); ++i)
            {
                itemList.append(m_unindexedItems.at(i));
            }
        }
    }
    else
    {
        // Rebuild the list of items to avoid holes. ### We could also just
        // compress the item lists at this point.
        for (int i = 0; i < m_indexedItems.size(); ++i)
        {
            if (m_indexedItems.at(i))
            {
                itemList.append(m_indexedItems.at(i));
            }
        }
        for (int i = 0; i < m_unindexedItems.size(); ++i)
        {
            if (m_unindexedItems.at(i))
            {
                itemList.append(m_unindexedItems.at(i));
            }
        }
    }
    if (order != -1 && itemList.size() > 0)
    {
        //We sort descending order
        _dw_sortItems(&itemList, order, m_sortCacheEnabled);
    }
    return itemList;
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwGraphicsItem *> DwGraphicsSceneBspTreeIndex::estimateItems(const DwRectF &rect, Dw::SortOrder order) const
{
    return _dw_estimateItems(rect, order);
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwGraphicsItem *> DwGraphicsSceneBspTreeIndex::estimateTopLevelItems(const DwRectF &rect, Dw::SortOrder order) const
{
    return _dw_estimateItems(rect, order, true);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::clear()
{
    m_bsp.clear();
    m_lastItemCount = 0;
    m_freeItemIndexes.clear();
    for (int i = 0; i < m_indexedItems.size(); ++i)
    {
        // Ensure item bits are reset properly.
        if (DwGraphicsItem *item = m_indexedItems.at(i))
        {
            DW_ASSERT(!item->m_itemDiscovered);
            item->m_index = InvalidIndex;
        }
    }
    m_indexedItems.clear();
    m_unindexedItems.clear();
    m_regenerateIndex = true;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::addItem(DwGraphicsItem* item)
{
    _dw_addItem(item);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::removeItem(DwGraphicsItem* item)
{
    _dw_removeItem(item);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::prepareBoundingRectChange(const DwGraphicsItem *item)
{
    if (!item || item->m_index == InvalidIndex )
        return; // Item is not in BSP tree; nothing to do.

    DwGraphicsItem *thatItem = const_cast<DwGraphicsItem *>(item);
    _dw_removeItem(thatItem, /*recursive=*/false, /*moveToUnindexedItems=*/true);
    for (int i = 0; i < item->children().size(); ++i)  // ### Do we really need this?
        prepareBoundingRectChange(item->children().at(i));
}

//------------------------------------------------------------------------------
/**
	THE ITEM"S GEMOTRY CHANGE IS DEALED BY PREPAREBOUNDINGRECTCHANGE
	AND THE CLIP THINGS IS DELIVER THE ESTIMATE ALGORITHM
*/
void DwGraphicsSceneBspTreeIndex::itemChange(const DwGraphicsItem *item,
        Dw::GraphicsItemChange change,
        const DwVariant& newOne, const DwVariant& oldOne)
{
    DW_UNUSED(item);
    DW_UNUSED(change);

    switch (change)
    {
    case Dw::ItemFlagsChange:
    {
        // Handle ItemIgnoresTransformations
        int newFlags = newOne.toInt();
        bool clipsChildren = !!(oldOne.toInt() & Dw::ItemClipsChildrenToShape);
        bool willClipChildren = !!(newFlags & Dw::ItemClipsChildrenToShape);
        if (clipsChildren != willClipChildren)
        {
            DwGraphicsItem *thatItem = const_cast<DwGraphicsItem *>(item);
            // Remove item and its descendants from the index and append
            // them to the list of unindexed items. Then, when the index
            // is updated, they will be put into the bsp-tree or the list
            // of untransformable items.
            _dw_removeItem(thatItem, /*recursive=*/true, /*moveToUnidexedItems=*/true);
        }
        break;
    }
    case Dw::ItemZValueChange:
        _dw_invalidateSortCache();
        break;
    case Dw::ItemParentChange:
    {
        _dw_invalidateSortCache();
        DwGraphicsItem *thatItem = const_cast<DwGraphicsItem *>(item);
        _dw_removeItem(thatItem, /*recursive=*/true, /*moveToUnidexedItems=*/true);
        break;
    }
    default:
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::updateSceneRect(const DwRectF &rect)
{
    m_sceneRect = rect;
    _dw_resetIndex();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::_dw_purgeRemovedItems()
{
    if (!m_purgePending && m_removedItems.isEmpty())
        return;

    // Remove stale items from the BSP tree.
    m_bsp.removeItems(m_removedItems);
    // Purge this list.
    m_removedItems.clear();
    m_freeItemIndexes.clear();
    for (int i = 0; i < m_indexedItems.size(); ++i)
    {
        if (!m_indexedItems.at(i))
            m_freeItemIndexes.append(i);
    }
    m_purgePending = false;
}

//------------------------------------------------------------------------------
/**
*/
#define dwLn ::log
static inline int intmaxlog(int n)
{
    return  (n > 0 ? dwMax(dwCeil(dwLn(DwReal(n)) / dwLn(DwReal(2))), 5) : 0);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::_dw_updateIndex()
{
    if (!m_indexTimer.isActive())
        return;

    m_indexTimer.stop();

    _dw_purgeRemovedItems();

    // Add unindexedItems to indexedItems
    for (int i = 0; i < m_unindexedItems.size(); ++i)
    {
        if (DwGraphicsItem *item = m_unindexedItems.at(i))
        {
            DW_ASSERT(!item->m_itemDiscovered);
            if (!m_freeItemIndexes.isEmpty())
            {
                int freeIndex = m_freeItemIndexes.front();
                item->m_index = freeIndex;
                m_indexedItems[freeIndex] = item;
                m_freeItemIndexes.pop_front();
            }
            else
            {
                item->m_index = m_indexedItems.size();
                m_indexedItems.append(item);
            }
        }
    }

    // Determine whether we should regenerate the BSP tree.
    if (m_bspTreeDepth == 0)
    {
        int oldDepth = intmaxlog(m_lastItemCount);
        m_bspTreeDepth = intmaxlog(m_indexedItems.size());
        static const int slack = 100;
        if (m_bsp.leafCount() == 0
                || (oldDepth != m_bspTreeDepth && dwAbs(m_lastItemCount - m_indexedItems.size()) > slack))
        {
            // ### Crude algorithm.
            m_regenerateIndex = true;
        }
    }

    // Regenerate the tree.
    if (m_regenerateIndex)
    {
        m_regenerateIndex = false;
        m_bsp.initialize(m_sceneRect, m_bspTreeDepth);
        m_unindexedItems = m_indexedItems;
        m_lastItemCount = m_indexedItems.size();
    }

    // Insert all unindexed items into the tree.
    for (int i = 0; i < m_unindexedItems.size(); ++i)
    {
        if (DwGraphicsItem *item = m_unindexedItems.at(i))
        {
            m_bsp.insertItem(item, item->sceneBoundingRect());
        }
    }
    m_unindexedItems.clear();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::_dw_startIndexTimer( int interval /*= DWGRAPHICSSCENE_INDEXTIMER_TIMEOUT*/ )
{
    if (m_indexTimer.isActive())
    {
        m_restartIndexTimer = true;
    }
    else
    {
        m_indexTimer.start(interval);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::_dw_resetIndex()
{
    _dw_purgeRemovedItems();
    for (int i = 0; i < m_indexedItems.size(); ++i)
    {
        if (DwGraphicsItem *item = m_indexedItems.at(i))
        {
            item->m_index = InvalidIndex;
            DW_ASSERT(!item->m_itemDiscovered);
            m_unindexedItems.append(item);
        }
    }
    m_indexedItems.clear();
    m_freeItemIndexes.clear();

    m_regenerateIndex = true;
    _dw_startIndexTimer();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::_dw_climbTree( DwGraphicsItem *item, int *stackingOrder )
{
    if (!item->children().isEmpty())
    {
        DwVector<DwGraphicsItem *> childList = item->children();
        DwGraphicsUtil::sortSiblingsItems(&childList, Dw::DescendingOrder);

        item->m_globalStackingOrder = (*stackingOrder)++;
        for (int i = 0; i < childList.size(); ++i)
        {
            _dw_climbTree(childList.at(i), stackingOrder);
        }
    }
    else
    {
        item->m_globalStackingOrder = (*stackingOrder)++;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::_dw_updateSortCache()
{
    _dw_updateIndex();

    if (!m_sortCacheEnabled || !m_updatingSortCache)
        return;

    m_updatingSortCache = false;
    int stackingOrder = 0;

    // the scene can quick fetch topLevelItems
    DwVector<DwGraphicsItem *> topLevels;
    DwVector<DwGraphicsItem *> itemList = items();
    for (int i = 0; i < itemList.size(); ++i)
    {
        DwGraphicsItem *item = itemList.at(i);
        if (item && !item->parentItem())
        {
            topLevels.append(item);
        }
    }

    DwGraphicsUtil::sortSiblingsItems(&itemList, Dw::DescendingOrder);
    for (int i = 0; i < topLevels.size(); ++i)
    {
        _dw_climbTree(topLevels.at(i), &stackingOrder);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::_dw_invalidateSortCache()
{
    if (!m_sortCacheEnabled || m_updatingSortCache)
        return;

    m_updatingSortCache = true;

    //TODO: queue connection invoke
    _dw_updateSortCache();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::_dw_addItem( DwGraphicsItem *item, bool recursive /*= false*/ )
{
    if (!item)
        return;

    // Prevent reusing a recently deleted pointer: purge all removed item from our lists.
    _dw_purgeRemovedItems();

    // Invalidate any sort caching; arrival of a new item means we need to resort.
    // Update the scene's sort cache settings.
    item->m_globalStackingOrder = InvalidIndex;
    _dw_invalidateSortCache();

    // Indexing requires sceneBoundingRect(), but because \a item might
    // not be completely constructed at this point, we need to store it in
    // a temporary list and schedule an indexing for later.
    if (item->m_index == InvalidIndex)
    {
        DW_ASSERT(m_unindexedItems.indexOf(item) == InvalidIndex);
        m_unindexedItems.append(item);
        _dw_startIndexTimer(0);
    }
    else
    {
        DW_ASSERT(m_unindexedItems.indexOf(item) != InvalidIndex);
        DW_WARNING("DwGraphicsSceneBspTreeIndex::_dw_addItem: item has already been added to this BSP");
    }

    if (recursive)
    {
        for (int i = 0; i < item->children().size(); ++i)
        {
            _dw_addItem(item->children().at(i), recursive);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::_dw_removeItem( DwGraphicsItem *item, bool recursive /*= false*/, bool moveToUnindexedItems /*= false*/ )
{
    if (!item)
        return;

    if (item->m_index != -1)
    {
        DW_ASSERT(item->m_index < m_indexedItems.size());
        DW_ASSERT(m_indexedItems.at(item->m_index) == item);
        DW_ASSERT(!item->m_itemDiscovered);
        m_freeItemIndexes.append(item->m_index);
        m_indexedItems[item->m_index] = 0;
        item->m_index = InvalidIndex;

        if (item->m_inDestructor)
        {
            // Avoid virtual function calls from the destructor.
            m_purgePending = true;
            m_removedItems.insert(item);
        }
        else
        {
            m_bsp.removeItem(item, item->sceneBoundingRect());
        }
    }
    else
    {
        m_unindexedItems.removeOne(item);
    }

    _dw_invalidateSortCache(); // ### Only do this when removing from BSP?

    DW_ASSERT(item->m_index == InvalidIndex);
    DW_ASSERT(m_indexedItems.indexOf(item) == InvalidIndex);
    DW_ASSERT(m_unindexedItems.indexOf(item) == InvalidIndex);

    if (moveToUnindexedItems)
    {
        _dw_addItem(item);
    }

    if (recursive)
    {
        for (int i = 0; i < item->children().size(); ++i)
        {
            _dw_removeItem(item->children().at(i), recursive, moveToUnindexedItems);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwGraphicsItem *> DwGraphicsSceneBspTreeIndex::_dw_estimateItems( const DwRectF &rect, Dw::SortOrder order, bool onlyTopLevelItems /*= false*/ )const
{
    if (onlyTopLevelItems && rect.isNull())
        return DwGraphicsSceneIndex::estimateTopLevelItems(rect, order);

    const_cast<DwGraphicsSceneBspTreeIndex*>(this)->_dw_purgeRemovedItems();
    const_cast<DwGraphicsSceneBspTreeIndex*>(this)->_dw_updateSortCache();
    DW_ASSERT(m_unindexedItems.isEmpty());

    DwVector<DwGraphicsItem *> rectItems = m_bsp.items(rect, onlyTopLevelItems);

    _dw_sortItems(&rectItems, order, m_sortCacheEnabled, onlyTopLevelItems);
    return rectItems;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::_dw_sortItems( DwVector<DwGraphicsItem *> *itemList, Dw::SortOrder order, bool sortCacheEnabled, bool onlyTopLevelItems /*= false*/ )
{
    if (order == Dw::SortOrder(-1) || itemList->size() <= 0)
        return;

    if (onlyTopLevelItems)
    {
        DwGraphicsUtil::sortSiblingsItems(itemList, order);
        return;
    }

    if (sortCacheEnabled)
    {
        DwGraphicsUtil::sortItemsByStackOrder(itemList, order);
    }
    else
    {
        DwGraphicsUtil::sortItems(itemList, order);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneBspTreeIndex::_dw_onIndexTimerEvent()
{
    if (m_restartIndexTimer)
    {
        m_restartIndexTimer = false;
    }
    else
    {
        // this call will kill the timer
        _dw_updateIndex();
    }
}
