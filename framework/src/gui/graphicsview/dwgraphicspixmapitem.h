#pragma once
#include "dwgraphicsitem.h"
#include "dwgraphics/dwimage.h"
#include "dwgui/dwurl.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsPixmapItem : public DwGraphicsItem
{
    DW_DECLARE_CLASS(DwGraphicsPixmapItem);
public:
    DwGraphicsPixmapItem(DwGraphicsItem *parent = NULL, DwGraphicsScene *scene = NULL);
    explicit DwGraphicsPixmapItem(const DwImage& image, DwGraphicsItem *parent = NULL, DwGraphicsScene *scene = NULL);
    virtual ~DwGraphicsPixmapItem();

    const DwImage& pixmap() const;
    void setPixmap(const DwImage &pixmap, bool force = false);
    void setPixmap(const DwUrl &url);

    const DwPointF& offset() const;
    void setOffset(const DwPointF &offset);
    inline void setOffset(DwReal x, DwReal y);

    virtual void paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option);// = 0;

    // accessible
    virtual DwString accessibleDescription() const;

private:
    DwImage m_image;
    DwPointF m_offset;
};// end of DwGraphicsPixmapItem
DW_REGISTER_CLASS(DwGraphicsPixmapItem);

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsPixmapItem::setOffset(DwReal x, DwReal y)
{
    setOffset(DwPointF(x, y));
};