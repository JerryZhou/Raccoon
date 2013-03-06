#include "stable.h"
#include "dwgraphicslayoutitem.h"

DW_IMPLEMENT_CLASS(DwGraphicsLayoutItem, 'GLTM', DwGraphicsItem);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsLayoutItem::DwGraphicsLayoutItem(DwGraphicsItem *parent/* = NULL*/, DwGraphicsScene *scene/* = NULL*/)
    : DwGraphicsItem(parent, scene)
{

}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsLayoutItem::~DwGraphicsLayoutItem()
{

}