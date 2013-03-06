#include "stable.h"
#include "dwprogressbar.h"
#include "dwgraphicsframeitem.h"
#include "dwgui/dwgraphicsrectitem.h"
#include "dwgui/dwgraphicsbackgrounditem.h"
#include "dwgui/dwanchors.h"
#include "dwwidgeteventhandler.h"

//------------------------------------------------------------------------------
#ifndef DW_CHECK
#define DW_CHECK(con) if (!(con)) {return;}
#define DW_CHECK_RET(con, ret) if(!(con)) {return (ret);}
#define DW_CHECK_ASSERT(con) if(!(con)) {assert(false); return;}
#define DW_CHECK_ASSERT_RET(con) if(!(con)) {assert(false); return (ret);}
#endif
//------------------------------------------------------------------------------
const char KNormalImage[] = "dwgui:edit.png";

int lerp(int l0, int l1, DwReal l0t, DwReal l1t)
{
    return (int)(l0*l0t + l1*l1t);
}

//------------------------------------------------------------------------------
/**
*/
DwColor lerpColor(const DwColor& c0, const DwColor& c1, DwReal r0, DwReal r1, DwReal t)
{
    DwReal ct = (t - r0)/(r1-r0);
    return DwColor(lerp(c0.red(), c1.red(), (1.0-ct), ct)
                   ,lerp(c0.green(), c1.green(), (1.0-ct), ct)
                   ,lerp(c0.blue(), c1.blue(), (1.0-ct), ct)
                   ,lerp(c0.alpha(), c1.alpha(), (1.0-ct), ct));
}

DW_IMPLEMENT_CLASS(DwProgressBar, 'PSBR', DwWidgetItem);

//------------------------------------------------------------------------------
/**
*/
DwProgressBar::DwProgressBar(DwGraphicsItem *parent/* = NULL*/)
    : DwWidgetItem(parent)
    , m_margins(0, 0, 0, 0)
    , m_rect(NULL)
    , m_cur(0.0)
    , m_min(0.0)
    , m_max(1.0)
{
    setName(L"progressBar");
    setRange(0.0, 1.0);
    setCur(0.0);
    setMargins(2, 2, 2, 2);
    setFlow(DwWidget::LeftToRight);
    setBoundingRect(DwRectF(0., 0., 120., 22.));

    setColor(DwWidget::PhaseLeft, DwColor(230, 66, 26));
    setPhase(DwWidget::PhaseLeft, 0.1);
    setColor(DwWidget::PhaseMiddle, DwColor(170, 128, 26));
    setPhase(DwWidget::PhaseMiddle, 0.3);
    setColor(DwWidget::PhaseRight, DwColor(149, 200, 26));
    setPhase(DwWidget::PhaseRight, 0.8);
    setColor(DwWidget::PhaseMax, DwColor(21, 234, 43));

    m_background->setURL(KNormalImage);

    m_rect = new DwGraphicsRectItem(this);
    m_rect->setName(L"rect");

    updateBar();

    // accessible
    setAccessibleName(tr("raccoon progressbar"));
    setAccessibleDescription(tr("raccoon progressbar"));
    setWhatsThis(tr("raccoon base progressbar"));
}

//------------------------------------------------------------------------------
/**
*/
DwProgressBar::~DwProgressBar()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwProgressBar::setBackground(const DwString& back)
{
    m_background->setURL(back);
}

//------------------------------------------------------------------------------
/**
*/
DwVariant DwProgressBar::itemChange(Dw::GraphicsItemChange change, const DwVariant &val)
{
    if (change == Dw::ItemGemoryChange)
    {
        updateBar();
    }
    return DwWidgetItem::itemChange(change, val);
}

//------------------------------------------------------------------------------
/**
*/
DwString DwProgressBar::toolTips() const
{
    DwString cur = DwString::number((int)(m_cur * 100));
    return DwString(const_cast<DwProgressBar*>(this)->tr("current progress:")) + cur + DwString("%");
}

//------------------------------------------------------------------------------
/**
*/
DwColor DwProgressBar::curColor() const
{
    if (cur() <= m_phases[DwWidget::PhaseLeft])
    {
        return m_colors[DwWidget::PhaseLeft];
    }
    else if (cur() <= m_phases[DwWidget::PhaseMiddle])
    {
        return lerpColor(m_colors[DwWidget::PhaseLeft]
                         , m_colors[DwWidget::PhaseMiddle]
                         , m_phases[DwWidget::PhaseLeft]
                         , m_phases[DwWidget::PhaseMiddle]
                         , cur());
    }
    else if (cur() <= m_phases[DwWidget::PhaseRight])
    {
        return lerpColor(m_colors[DwWidget::PhaseMiddle]
                         , m_colors[DwWidget::PhaseRight]
                         , m_phases[DwWidget::PhaseMiddle]
                         , m_phases[DwWidget::PhaseRight]
                         , cur());
    }

    return lerpColor(m_colors[DwWidget::PhaseRight]
                     , m_colors[DwWidget::PhaseMax]
                     , m_phases[DwWidget::PhaseRight]
                     , 1.0
                     , cur());
}

//------------------------------------------------------------------------------
/**
*/
void DwProgressBar::updateBar()
{
    DW_CHECK(m_rect);
    DW_CHECK(this->max() > 0.000001);

    DwRect destRect = boundingRect().toRect();
    destRect.adjust(m_margins.left(), m_margins.top(), -m_margins.right(), -m_margins.bottom());
    DW_CHECK(destRect.width() > 0 && destRect.height() > 0);

    if (m_flow == DwWidget::LeftToRight)
    {
        destRect.setWidth((int)(destRect.width() * this->cur() / this->max()));
    }
    else
    {
        DW_ASSERT(m_flow == DwWidget::TopToBottom);
        destRect.setHeight((int)(destRect.height() * this->cur() / this->max()));
    }

    m_rect->setRect(destRect);
    m_rect->setColor(curColor());

    // accessible
    setAccessibleDescription(toolTips());
}