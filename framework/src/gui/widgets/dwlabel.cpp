#include "stable.h"
#include "dwlabel.h"
#include "dwgraphicsframeitem.h"
#include "dwgui/dwgraphicssimpletextitem.h"
#include "dwgui/dwanchors.h"
#include "dwwidgeteventhandler.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwLabel, 'LAEL', DwWidgetItem);

//------------------------------------------------------------------------------
/**
*/
DwLabel::DwLabel(DwGraphicsItem *parent)
    : DwWidgetItem(parent)
    , m_text(NULL)
    , m_background(NULL)
    , m_margins(2, 2, 2, 2)
{
    setFocusable(false);
    setHoverable(false);
    setFocusPolicy(Dw::NoFocus);
    setFlag(Dw::ItemStopsClickFocusPropagation, false);
    setFlag(Dw::ItemStopsMousePropagation, false);
    setAcceptedMouseButton(Dw::LeftButton, false);

    setName(L"label");

    m_text = new DwGraphicsSimpleTextItem(this);
    m_text->setName(tr("label"));
    m_text->anchors()->setCenterIn(this);

    //accessible
    setAccessibleName(tr("raccoon label"));
    setAccessibleDescription(tr("raccoon label"));
    setWhatsThis(tr("the raccoon engine's inner label"));
}

//------------------------------------------------------------------------------
/**
*/
DwLabel::~DwLabel()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwLabel::setText(const DwString& txt)
{
    m_text->setText(txt);
    updateBoundingRect();

    // accessible
    setAccessibleDescription(txt);
}

//------------------------------------------------------------------------------
/**
*/
void DwLabel::updateBoundingRect()
{
    DwRectF textBoundingRect = m_text->boundingRect();
    textBoundingRect.adjust(0, 0, m_margins.left() + m_margins.right(), m_margins.top() + m_margins.bottom());
    m_text->anchors()->setHorizontalCenterOffset(m_margins.bottom() - m_margins.top());
    m_text->anchors()->setVerticalCenterOffset(m_margins.right() - m_margins.left());
    setBoundingRect(textBoundingRect);
}