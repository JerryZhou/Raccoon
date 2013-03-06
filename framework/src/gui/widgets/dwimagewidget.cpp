#include "stable.h"
#include "dwimagewidget.h"
#include "dwwidgeteventhandler.h"
#include "dwgui/dwanchors.h"
#include "dwgui/dwresourcelib.h"
#include "dwgui/dwgraphicsbackgrounditem.h"

//------------------------------------------------------------------------------
const char KNormalImage[] = "dwgui:edit.png";

DW_IMPLEMENT_CLASS(DwImageWidget, 'IEWT', DwWidgetItem);

//------------------------------------------------------------------------------
/**
*/
DwImageWidget::DwImageWidget(DwGraphicsItem* parent/* = NULL*/)
    : DwWidgetItem(parent)
    , m_pixmap(NULL)
    , m_fixed(false)
{
    setName(L"imagewidget");

    m_background->setName(L"background");
    m_background->setURL(KNormalImage);

    // accessible
    setAccessibleName(tr("raccoon image widget"));
    setAccessibleDescription(tr("raccoon image widget"));
    setWhatsThis(tr("raccoon base image widget"));
}

//------------------------------------------------------------------------------
/**
*/
DwImageWidget::~DwImageWidget()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwImageWidget::setImage(const DwUrl& i)
{
    if (!m_pixmap)
    {
        m_pixmap = new DwGraphicsPixmapItem(this);
        m_pixmap->setName(L"image");
        m_pixmap->anchors()->setMargins(2.0);
        m_pixmap->anchors()->setCenterIn(this);
    }
    m_pixmap->setPixmap(DwResourceInterface::acquireImage(i));
    updateBoundingRect();

    // accessible
    setAccessibleDescription(i.localPath());
}

//------------------------------------------------------------------------------
/**
*/
void DwImageWidget::setFixed(bool fixed)
{
    m_fixed = fixed;
    updateBoundingRect();
}

//------------------------------------------------------------------------------
/**
*/
void DwImageWidget::updateBoundingRect()
{
    if (!m_fixed && m_pixmap)
    {
        DwRectF newRect = m_pixmap->boundingRect().adjusted(0, 0
                          ,m_pixmap->anchors()->leftMargin() + m_pixmap->anchors()->rightMargin()
                          ,m_pixmap->anchors()->topMargin() + m_pixmap->anchors()->bottomMargin());
        m_pixmap->anchors()->setHorizontalCenterOffset(m_pixmap->anchors()->rightMargin() - m_pixmap->anchors()->leftMargin());
        m_pixmap->anchors()->setVerticalCenterOffset(m_pixmap->anchors()->bottomMargin() - m_pixmap->anchors()->topMargin());
        setBoundingRect(newRect);
    }
}