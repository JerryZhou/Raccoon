#include "stable.h"
#include "dwgraphicsscenelinearindex.h"
#include "dwgraphicsscene.h"
#include "dwgraphicsitem.h"
#include "dwgraphicsscenelinearIndex.h"
#include "dwgraphicsutil.h"
#include "dwdummys.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwGraphicsSceneLinearIndex, 'SLIX', DwGraphicsSceneIndex);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSceneLinearIndex::DwGraphicsSceneLinearIndex(DwGraphicsScene *scene/* = 0*/)
    : DwGraphicsSceneIndex(scene)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSceneLinearIndex::~DwGraphicsSceneLinearIndex()
{
    DW_ASSERT(m_items.isEmpty());
}

//------------------------------------------------------------------------------
/**
	NB! sort, very slow
*/
DwVector<DwGraphicsItem *> DwGraphicsSceneLinearIndex::items(Dw::SortOrder order/* = Dw::DescendingOrder*/) const
{
    DwVector<DwGraphicsItem *> oItems = m_items;
    if (oItems.size() > 0)
    {
        DwGraphicsUtil::sortItems(&oItems, order);
    }
    return oItems;
}

//------------------------------------------------------------------------------
/**
	NB! sort, very slow
*/
DwVector<DwGraphicsItem *> DwGraphicsSceneLinearIndex::estimateItems(const DwRectF &rect, Dw::SortOrder order) const
{
    DW_UNUSED(rect);
    DwVector<DwGraphicsItem *> oItems = m_items;
    if (oItems.size() > 0)
    {
        DwGraphicsUtil::sortItems(&oItems, order);
    }
    return oItems;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneLinearIndex::addItem(DwGraphicsItem* item)
{
    DW_CHECK_ASSERT(item);
    DW_ASSERT(m_items.indexOf(item) ==  InvalidIndex);

    m_items.append(item);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneLinearIndex::removeItem(DwGraphicsItem* item)
{
    DW_CHECK_ASSERT(item);
    DW_ASSERT(m_items.indexOf(item) !=  InvalidIndex);

    m_items.removeOne(item);
}