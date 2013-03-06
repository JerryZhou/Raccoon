#pragma once

#include "dwgraphicsitem.h"
#include "dwcore/dwrect.h"
#include "dwgraphics/dwcolor.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsRectItem : public DwGraphicsItem
{
    DW_DECLARE_CLASS(DwGraphicsRectItem);
public:
    DwGraphicsRectItem(DwGraphicsItem *parent = NULL, DwGraphicsScene *scene = NULL);
    explicit DwGraphicsRectItem(const DwRectF& rect, DwGraphicsItem *parent = NULL, DwGraphicsScene *scene = NULL);
    DwGraphicsRectItem(DwReal x, DwReal y, DwReal w, DwReal h, DwGraphicsItem *parent = NULL, DwGraphicsScene *scene = NULL);
    virtual ~DwGraphicsRectItem();

    DwColor color() const;
    void setColor(const DwColor &color);

    virtual void paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option);

protected:
    DwColor m_color;
private:
};// end of DwGraphicsRectItem
DW_REGISTER_CLASS(DwGraphicsRectItem);