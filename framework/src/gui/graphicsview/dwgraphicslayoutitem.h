#pragma once
#include "dwgui/dwgraphicsitem.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsLayoutItem : public DwGraphicsItem
{
    DW_DECLARE_CLASS(DwGraphicsLayoutItem);
public:
    explicit DwGraphicsLayoutItem(DwGraphicsItem *parent = NULL, DwGraphicsScene *scene = NULL);
    virtual ~DwGraphicsLayoutItem();
protected:
private:
};// end of DwGraphicsLayoutItem
DW_REGISTER_CLASS(DwGraphicsLayoutItem);