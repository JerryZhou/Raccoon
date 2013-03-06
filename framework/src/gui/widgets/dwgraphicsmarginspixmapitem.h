#pragma once
#include "dwcore/dwmargins.h"
#include "dwgraphics/dwimage.h"
#include "dwgui/dwgraphicsitem.h"
#include "dwgui/dwurl.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsMarginsPixmapItem : public DwGraphicsItem
{
    DW_DECLARE_CLASS(DwGraphicsMarginsPixmapItem);
public:
    explicit DwGraphicsMarginsPixmapItem(DwGraphicsItem *parent = NULL);
    virtual ~DwGraphicsMarginsPixmapItem();

    const DwImage& pixmap() const;
    void setPixmap(const DwImage &pixmap);
    void setPixmap(const DwUrl &url);

    void setMargins(const DwMargins& m);
    inline const DwMargins& margins() const;

    void setDrawCenter(bool b);
    bool isDrawCenter() const;

    virtual void paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option);// = 0;

protected:
    DwMargins m_margins;
    DwImage m_pixmap;
    bool m_needDrawCenter : 1;
};// end of DwGraphicsMarginsPixmap
DW_REGISTER_CLASS(DwGraphicsMarginsPixmapItem);

//------------------------------------------------------------------------------
/**
*/
inline const DwMargins& DwGraphicsMarginsPixmapItem::margins() const
{
    return m_margins;
}