#pragma once
#include "dwgui/dwwidgetitem.h"
#include "dwgui/dwgraphicsframeitem.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwImageWidget : public DwWidgetItem
{
    DW_DECLARE_CLASS(DwImageWidget);
public:
    DwImageWidget(DwGraphicsItem* parent = NULL);
    virtual ~DwImageWidget();

    inline DwGraphicsPixmapItem *pixmap() const;
    inline const DwImage& image() const;

    void setImage(const DwUrl& );

    inline bool isFixed() const;
    void setFixed(bool fixed) ;

    void updateBoundingRect();

protected:
    DwGraphicsPixmapItem *m_pixmap;
    bool m_fixed;
private:
};// end of DwImageWidget
DW_REGISTER_CLASS(DwImageWidget);

//------------------------------------------------------------------------------
/**
*/
inline DwGraphicsPixmapItem *DwImageWidget::pixmap() const
{
    return m_pixmap;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwImage& DwImageWidget::image() const
{
    return m_pixmap->pixmap();
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwImageWidget::isFixed() const
{
    return m_fixed;
}