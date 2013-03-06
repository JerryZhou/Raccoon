#pragma once
#include "dwnamespace.h"

//------------------------------------------------------------------------------
class DwGraphicsItem;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAnchorLine
{
public:
    DwAnchorLine() : item(0), anchorLine(Dw::Invalid) {}
    DwAnchorLine(DwGraphicsItem* i, Dw::AnchorLine l) : item(i), anchorLine(l) {}
    ~DwAnchorLine();

    bool operator == (const DwAnchorLine &other) const;

    DwGraphicsItem *item;
    Dw::AnchorLine anchorLine;
};// end of DwAnchorLine