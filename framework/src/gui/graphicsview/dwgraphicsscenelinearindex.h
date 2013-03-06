#pragma once
#include "dwgraphicssceneindex.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsSceneLinearIndex : public DwGraphicsSceneIndex
{
    DW_DECLARE_CLASS(DwGraphicsSceneLinearIndex);
public:
    DwGraphicsSceneLinearIndex(DwGraphicsScene* scene = NULL);
    virtual ~DwGraphicsSceneLinearIndex();

    virtual DwVector<DwGraphicsItem *> items(Dw::SortOrder order = Dw::DescendingOrder) const;

    virtual DwVector<DwGraphicsItem *> estimateItems(const DwRectF &rect, Dw::SortOrder order) const;

protected:
    virtual void addItem(DwGraphicsItem* item);
    virtual void removeItem(DwGraphicsItem* item);

private:
    DwVector<DwGraphicsItem*> m_items;
};// end of DwGraphicsSceneLinearIndex