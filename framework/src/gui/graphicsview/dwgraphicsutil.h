#pragma once
#include "dwnamespace.h"
#include "dwcore/dwvector.h"

//------------------------------------------------------------------------------
class DwGraphicsItem;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsUtil
{
public:
    // NB! THE ALGORITHM MUST BE STABLE
    static void sortChild(DwVector<DwGraphicsItem *> *oItems);

    static void sortSiblingsItems(DwVector<DwGraphicsItem *> *oItems, Dw::SortOrder order);

    static void sortChildBySiblingIndex(DwVector<DwGraphicsItem *> *oItems, Dw::SortOrder order);

    static void sortItems(DwVector<DwGraphicsItem *> *oItems, Dw::SortOrder order);

    static void sortItemsByStackOrder(DwVector<DwGraphicsItem *> *oItems, Dw::SortOrder order);

    static DwGraphicsItem* ancestorOfAccept(DwGraphicsItem * item, Dw::GraphicsItemFlag flag);

    static DwGraphicsItem* ancestorOfAccept(DwVector<DwGraphicsItem *> items, Dw::GraphicsItemFlag flag);
};// end of DwGraphicsUtil