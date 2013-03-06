#include "stable.h"
#include "dwanchorline.h"
#include "graphicsview/dwgraphicsitem.h"

// NB! THE ALGORITHEM FROM QT4.7(DECLARATIVE)

//------------------------------------------------------------------------------
/**
*/
DwAnchorLine::~DwAnchorLine()
{
    item = 0;
    anchorLine = Dw::Invalid;
}

//------------------------------------------------------------------------------
/**
*/
bool DwAnchorLine::operator == (const DwAnchorLine &other) const
{
    return other.anchorLine == anchorLine && other.item == item;
}