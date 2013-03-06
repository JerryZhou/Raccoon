#include "stable.h"
#include "dwwidgetitem.h"
#include "dwgraphics/dwpainter.h"
#include "dwwidgeteventhandler.h"
#include "dwgui/dwgraphicsbackgrounditem.h"
#include "dwgui/dwanchors.h"

DW_IMPLEMENT_CLASS(DwWidgetItem, 'WTIM', DwGraphicsItem);
//------------------------------------------------------------------------------
/**
*/
DwWidgetItem::DwWidgetItem(DwGraphicsItem *parent/* = NULL*/)
    : DwGraphicsItem(parent)
    , m_dragPolicy(DwWidget::NoDrag)
{
    setFocusable(true);
    setHoverable(true);
    setFocusPolicy(Dw::ClickFocus);

    setFlag(Dw::ItemStopsClickFocusPropagation, true);
    setFlag(Dw::ItemStopsMousePropagation, true);
    setFlag(Dw::ItemClipsChildren, true);
    setFlag(Dw::ItemHasNoContents, true);

    setAcceptedMouseButton(Dw::LeftButton, true);

    m_background = new DwGraphicsBackgroundItem(this);
    m_background->setName(L"background");
    m_background->anchors()->setFill(this);

    setEventHandler(new DwWidgetEventHandler(this, m_background));

    // accessible
    setAccessible(true);
    setAccessibleName(tr("raccoon widget"));
    setAccessibleDescription(tr("raccoon base widget item"));
    setWhatsThis(tr("raccoon base widget item"));
}

//------------------------------------------------------------------------------
/**
*/
DwWidgetItem::~DwWidgetItem()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwWidgetItem::paint(DwPainter *, const DwStyleOptionGraphicsItem *)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwWidget::DragPolicy DwWidgetItem::dragPolicy() const
{
    return m_dragPolicy;
}

//------------------------------------------------------------------------------
/**
*/
void DwWidgetItem::setDragPolicy(DwWidget::DragPolicy policy)
{
    m_dragPolicy = policy;
}

//------------------------------------------------------------------------------
/**
*/
void DwWidgetItem::setBackground(const DwUrl &uri)
{
    m_background->setURL(uri);
}