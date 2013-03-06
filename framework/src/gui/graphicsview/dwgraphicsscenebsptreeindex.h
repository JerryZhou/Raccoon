#pragma once
#include "dwgraphicssceneindex.h"
#include "dwgraphicsscene_bsp_p.h"
#include "dwcore/dwset.h"
#include "dwcore/dwlinkedlist.h"
#include "dwcore/dwvector.h"
#include "dwgui/dwtimer.h"

//------------------------------------------------------------------------------
static const int DWGRAPHICSSCENE_INDEXTIMER_TIMEOUT = 2000;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsSceneBspTreeIndex : public DwGraphicsSceneIndex
{
    DW_DECLARE_CLASS(DwGraphicsSceneBspTreeIndex);
public:
    explicit DwGraphicsSceneBspTreeIndex(DwGraphicsScene* scene = NULL);
    virtual ~DwGraphicsSceneBspTreeIndex();

    int bspTreeDepth() const;
    void setBspTreeDepth(int depth);

    void initialize(const DwRectF &rect);

    virtual DwVector<DwGraphicsItem *> items(Dw::SortOrder order = Dw::DescendingOrder) const;

    virtual DwVector<DwGraphicsItem *> estimateItems(const DwRectF &rect, Dw::SortOrder order) const;
    virtual DwVector<DwGraphicsItem *> estimateTopLevelItems(const DwRectF &rect, Dw::SortOrder order) const;

protected:
    virtual void clear();

    virtual void addItem(DwGraphicsItem* item);
    virtual void removeItem(DwGraphicsItem* item);
    virtual void prepareBoundingRectChange(const DwGraphicsItem *);

    virtual void itemChange(const DwGraphicsItem *item, Dw::GraphicsItemChange change, const DwVariant& newOne, const DwVariant& oldOne);

    void updateSceneRect(const DwRectF &rect);
    //------------------------------------------------------------------------------
    void _dw_purgeRemovedItems();

    void _dw_updateIndex();
    void _dw_startIndexTimer(int interval = DWGRAPHICSSCENE_INDEXTIMER_TIMEOUT);
    void _dw_resetIndex();

    void _dw_updateSortCache();

    void _dw_invalidateSortCache();
    void _dw_addItem(DwGraphicsItem *item, bool recursive = false);
    void _dw_removeItem(DwGraphicsItem *item, bool recursive = false, bool moveToUnindexedItems = false);
    DwVector<DwGraphicsItem *> _dw_estimateItems(const DwRectF &, Dw::SortOrder, bool b = false)const;

    static void _dw_climbTree(DwGraphicsItem *item, int *stackingOrder);
    static void _dw_sortItems(DwVector<DwGraphicsItem *> *itemList, Dw::SortOrder order,
                              bool cached, bool onlyTopLevelItems = false);

    void _dw_onIndexTimerEvent();

private:
    friend class DwGraphicsScene;

    int m_bspTreeDepth;
    DwRectF m_sceneRect;
    DwGraphicsSceneBspTree m_bsp;
    DwVector<DwGraphicsItem *> m_indexedItems;
    DwVector<DwGraphicsItem *> m_unindexedItems;

    bool m_restartIndexTimer;
    bool m_regenerateIndex;
    int m_lastItemCount;

    DwLinkedList<int> m_freeItemIndexes;

    bool m_sortCacheEnabled;
    bool m_updatingSortCache;

    bool m_purgePending;
    DwSet<DwGraphicsItem *> m_removedItems;

    DwTimer m_indexTimer;
};// end of DwGraphicsSceneBspTreeIndex