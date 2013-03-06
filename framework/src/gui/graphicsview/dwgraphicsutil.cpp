#include "stable.h"
#include "dwgraphicsutil.h"
#include "dwgraphicsitem.h"


//------------------------------------------------------------------------------
/**
*/
static inline bool dw_sort_by_siblingIndex_ascending(const DwGraphicsItem *lfs, const DwGraphicsItem *rfs)
{
    return lfs->siblingIndex() < rfs->siblingIndex();
}

//------------------------------------------------------------------------------
/**
*/
static inline bool dw_sort_by_siblingIndex_descending(const DwGraphicsItem *lfs, const DwGraphicsItem *rfs)
{
    return dw_sort_by_siblingIndex_ascending(rfs, lfs);
}

//------------------------------------------------------------------------------
/**
*/
static inline bool dw_sort_by_z_ascending(const DwGraphicsItem *lfs, const DwGraphicsItem *rfs)
{
    if (lfs->zValue() < rfs->zValue())
    {
        return true;
    }
    else
    {
        if (dwFuzzyCompare(lfs->zValue() , rfs->zValue()))
        {
            return dw_sort_by_siblingIndex_ascending(lfs, rfs);
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
static inline bool dw_sort_by_z_descending(const DwGraphicsItem *lfs, const DwGraphicsItem *rfs)
{
    //return (lfs->zValue() > rfs->zValue()) || (lfs->siblingIndex() > rfs->siblingIndex());
    return dw_sort_by_z_ascending(rfs, lfs);
}

//------------------------------------------------------------------------------
/**
*/
static inline bool dw_sort_by_leaf_ascending(const DwGraphicsItem *lfs, const DwGraphicsItem *rfs)
{
    if (lfs->parentItem() == rfs->parentItem())
    {
        dw_sort_by_z_ascending(lfs, rfs);
    }

    // Find common ancestor, and each item's ancestor closest to the common
    // ancestor.
    int item1Depth = lfs->depth();
    int item2Depth = rfs->depth();
    const DwGraphicsItem *p = lfs;
    const DwGraphicsItem *t1 = lfs;
    while (item1Depth > item2Depth && (p = p->parentItem()))
    {
        if (p == rfs)
        {
            // rfs is one of lfs's ancestors; lfs is on top, lfs > rfs
            return false;
        }
        t1 = p;
        --item1Depth;
    }
    p = rfs;
    const DwGraphicsItem *t2 = rfs;
    while (item2Depth > item1Depth && (p = p->parentItem()))
    {
        if (p == lfs)
        {
            // lfs is one of rfs's ancestors; rfs is on top, rfs > lfs
            return true;
        }
        t2 = p;
        --item2Depth;
    }

    // lfsAncestor is now at the same level as rfsAncestor, but not the same.
    const DwGraphicsItem *p1 = t1;
    const DwGraphicsItem *p2 = t2;
    while (t1 && t1 != t2)
    {
        p1 = t1;
        p2 = t2;
        t1 = t1->parentItem();
        t2 = t2->parentItem();
    }
    return dw_sort_by_z_ascending(p1, p2);
}

//------------------------------------------------------------------------------
/**
*/
static inline bool dw_sort_by_leaf_descending(const DwGraphicsItem *lfs, const DwGraphicsItem *rfs)
{
    return dw_sort_by_leaf_ascending(rfs, lfs);
}

//------------------------------------------------------------------------------
/**
*/
static inline bool dw_sort_by_stackingorder_ascending(const DwGraphicsItem *lfs, const DwGraphicsItem *rfs)
{
    return lfs->globalStackingOrder() < rfs->globalStackingOrder();
}

//------------------------------------------------------------------------------
/**
*/
static inline bool dw_sort_by_stackingorder_descending(const DwGraphicsItem *lfs, const DwGraphicsItem *rfs)
{
    return dw_sort_by_stackingorder_ascending(rfs, lfs);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsUtil::sortChild(DwVector<DwGraphicsItem *> *oItems)
{
    sortSiblingsItems(oItems, Dw::AscendingOrder);
}

//------------------------------------------------------------------------------
/**
*/
#include <algorithm>
#define dwSort std::sort
void DwGraphicsUtil::sortSiblingsItems(DwVector<DwGraphicsItem *> *oItems, Dw::SortOrder order)
{
    DW_ASSERT(oItems->size() > 0);
    if (order == Dw::AscendingOrder)
    {
        dwSort(oItems->begin(), oItems->end(), dw_sort_by_z_ascending);
    }
    else
    {
        dwSort(oItems->begin(), oItems->end(), dw_sort_by_z_descending);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsUtil::sortChildBySiblingIndex(DwVector<DwGraphicsItem *> *oItems, Dw::SortOrder order)
{
    DW_ASSERT(oItems->size() > 0);
    if (order == Dw::AscendingOrder)
    {
        dwSort(oItems->begin(), oItems->end(), dw_sort_by_siblingIndex_ascending);
    }
    else
    {
        dwSort(oItems->begin(), oItems->end(), dw_sort_by_siblingIndex_descending);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsUtil::sortItems(DwVector<DwGraphicsItem *> *oItems, Dw::SortOrder order)
{
    DW_ASSERT(oItems->size() > 0);
    if (order == Dw::AscendingOrder)
    {
        dwSort(oItems->begin(), oItems->end(), dw_sort_by_leaf_ascending);
    }
    else
    {
        dwSort(oItems->begin(), oItems->end(), dw_sort_by_leaf_descending);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsUtil::sortItemsByStackOrder(DwVector<DwGraphicsItem *> *oItems, Dw::SortOrder order)
{
    DW_ASSERT(oItems->size() > 0);
    if (order == Dw::AscendingOrder)
    {
        dwSort(oItems->begin(), oItems->end(), dw_sort_by_stackingorder_ascending);
    }
    else
    {
        dwSort(oItems->begin(), oItems->end(), dw_sort_by_stackingorder_descending);
    }
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsUtil::ancestorOfAccept(DwGraphicsItem * item, Dw::GraphicsItemFlag flag)
{
    if(!item) return NULL;
    if (item->flags() & flag)
    {
        return item;
    }
    return ancestorOfAccept(item->parentItem(), flag);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsUtil::ancestorOfAccept(DwVector<DwGraphicsItem *> items, Dw::GraphicsItemFlag flag)
{
    for (int i=0; i<items.size(); ++i)
    {
        DwGraphicsItem * item = items.at(i);
        if (item && (item->flags() & flag))
        {
            return item;
        }
    }

    if (items.size() > 0)
    {
        return ancestorOfAccept(items.front(), flag);
    }

    return NULL;
}