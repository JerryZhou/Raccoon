#pragma once
#include "dwgui/dwgraphicsitem.h"
#include "dwgraphics/dwimage.h"
#include "dwgui/dwurl.h"

//------------------------------------------------------------------------------
class DwPainter;

//NB! : The Minimum center area(both width and height) must be lareger and equal than 2 pixel
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsNinePatchItem : public DwGraphicsItem
{
    DW_DECLARE_CLASS(DwGraphicsNinePatchItem);
public:
    explicit DwGraphicsNinePatchItem(DwGraphicsItem *parent=NULL);
    DwGraphicsNinePatchItem(const DwUrl& uri, int i=0, DwGraphicsItem *parent=NULL);

    void setPixmap(const DwImage& img, bool force = false);
    void setPixmap(const DwUrl &uri);
    DwImage pixmap() const;
    void setIdx(int i);
    int idx() const;
    void setDrawCenter(bool b);
    bool isDrawCenter() const;
    void setNinePatch(const DwUrl &uri, int i);

    virtual void paint(DwPainter *painter, const DwStyleOptionGraphicsItem *);

protected:
    void updateSize();

    DwImage m_pixmap;
    int m_idx;
    bool m_drawCenter;
};// end of DwGraphicsNinePatchItem
DW_REGISTER_CLASS(DwGraphicsNinePatchItem);

//------------------------------------------------------------------------------
/**
*/
inline DwImage DwGraphicsNinePatchItem::pixmap() const
{
    return m_pixmap;
}

//------------------------------------------------------------------------------
/**
*/
inline int DwGraphicsNinePatchItem::idx() const
{
    return m_idx;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsNinePatchItem::isDrawCenter() const
{
    return m_drawCenter;
}