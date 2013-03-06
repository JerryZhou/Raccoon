#include "stable.h"
#include "dwscrollbar.h"
#include "dwscrollarea.h"
#include "dwpushbutton.h"
#include "dwgui/dwanchors.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwcore/sigslot.h"
#include "dwwidgeteventhandler.h"
#include "dwgui/dweventsignaldispatch.h"
#include "dwgui/dwwidgetevent.h"
#include "dwgui/dwgraphicsbackgrounditem.h"

//------------------------------------------------------------------------------
#define DW_TINY 0.00001

//------------------------------------------------------------------------------
const char KNormalImage[] = "dwgui:scroll_bk.png";
const char KBtnNormalImage[] = "dwgui:slider_normal.png";
const char KBtnPressdownImage[] = "dwgui:slider_pressdown.png";
const char KBtnHoverImage[]  = "dwgui:slider_hover.png";
const char KBtnFocusImage[] = "dwgui:slider_hover.png";
const char KBtnDisableImage[] = "dwgui:slider_normal.png";

DW_IMPLEMENT_CLASS(DwScrollBar, 'SLBR', DwWidgetItem);

//------------------------------------------------------------------------------
/**
*/
void onThumbDragMoveEvent(DwGraphicsItem* item, DwEvent* evt)
{
    DW_UNUSED(item);
    DW_UNUSED(evt);
#if DW_ENABLE_EVENT_DRAGDROP
    DwPushButton* button = (DwPushButton*)(item);
    DW_ASSERT(evt->eventId() == DwDragMoveEvent::EventId());
    DW_ASSERT(button->isMovable());
    DwDragMoveEvent* drag = static_cast<DwDragMoveEvent*>(evt);

    DwScrollBar* scrollBar = (DwScrollBar*)(button->parentItem());
    int delta = 0;
    if (scrollBar->flow() == DwWidget::TopToBottom)
    {
        delta = drag->m_delta.y();
    }
    else
    {
        delta = drag->m_delta.x();
    }

    scrollBar->scroll(scrollBar->piexl2Range(delta));
    evt->accept();
#endif
}

//------------------------------------------------------------------------------
/**
*/
class DwScrollBarSlots : public sigslot::has_slots
{
public:
    DwScrollBarSlots(DwScrollBar* bar)
        : m_scrollBar(bar)
    {
        ;
    }

    void onMinHitClicked(DwRttiObject *, DwEvent* )
    {
        m_scrollBar->scrollUp(m_scrollBar->step());
    }

    void onMaxHitClicked(DwRttiObject *, DwEvent* )
    {
        m_scrollBar->scrollDown(m_scrollBar->step());
    }

    DwScrollBar* m_scrollBar;
};

//------------------------------------------------------------------------------
/**
*/
class DwScrollBarEventHandler : public DwWidgetEventHandler
{
    DW_DECLARE_CLASS(DwScrollBarEventHandler);
public:
    explicit DwScrollBarEventHandler(DwGraphicsItem* item = NULL, DwGraphicsBackgroundItem* back = NULL)
        : DwWidgetEventHandler(item, back)
    {
        ;
    }

    virtual void onMouseWheel(DwMouseWheelEvent* evt)
    {
        DwScrollBar* scrollBar = dwsafe_cast<DwScrollBar>(m_item);
        DW_ASSERT(scrollBar);
        if (evt->isRollUp())
        {
            scrollBar->scrollUp(scrollBar->step());
        }
        else
        {
            scrollBar->scrollDown(scrollBar->step());
        }
    }

    virtual void onMouseClicked(DwMouseClickedEvent* evt)
    {
        DwScrollBar* scrollBar = dwsafe_cast<DwScrollBar>(m_item);
        DW_ASSERT(scrollBar);

        if (scrollBar->flow() == DwWidget::LeftToRight)
        {
            if ( evt->m_itemPoint.x() > scrollBar->thumb()->x() +scrollBar->thumb()->width() )
            {
                scrollBar->scrollDown(scrollBar->step());
            }
            else if(evt->m_itemPoint.x() < scrollBar->thumb()->x())
            {
                scrollBar->scrollUp(scrollBar->step());
            }
        }
        else
        {
            DW_ASSERT(scrollBar->flow() == DwWidget::TopToBottom);
            if (evt->m_itemPoint.y() > scrollBar->thumb()->y() + scrollBar->thumb()->height())
            {
                scrollBar->scrollDown(scrollBar->step());
            }
            else if(evt->m_itemPoint.y() < scrollBar->thumb()->y())
            {
                scrollBar->scrollUp(scrollBar->step());
            }
        }
    }
protected:
private:
};
DW_IMPLEMENT_CLASS(DwScrollBarEventHandler, 'SBEH', DwWidgetEventHandler);

//------------------------------------------------------------------------------
/**
*/
DwScrollBar::DwScrollBar(DwGraphicsItem *parent/* = NULL*/)
    : DwWidgetItem(parent)
    , m_flow(DwWidget::LeftToRight)
    , m_scrollArea(NULL)
    , m_thumb(NULL)
    , m_minHit(NULL)
    , m_maxHit(NULL)
    , m_slots(NULL)
{
    setRange(0., 1.0, 0.1);

    //------------------------------------------------------------------------------
    // just for convenience
    DwScrollArea* parentArea = dwsafe_cast<DwScrollArea>(parent);
    if(parentArea)  setArea(parentArea);
    //------------------------------------------------------------------------------

    m_slots = new DwScrollBarSlots(this);

    //m_background->setURL(KNormalImage);
    m_background->setColor(Dw::white);

    m_minHit = new DwPushButton(this);
    m_minHit->setName(L"minHit");
    m_minHit->signalDispatch()->connect(DwMouseClickedEvent::EventId(), m_slots, &DwScrollBarSlots::onMinHitClicked);
    m_minHit->setFocusProxy(this);
    m_minHit->setBackground("dwgui:edit.png");

    m_maxHit = new DwPushButton(this);
    m_maxHit->setName(L"maxHit");
    m_maxHit->signalDispatch()->connect(DwMouseClickedEvent::EventId(), m_slots, &DwScrollBarSlots::onMaxHitClicked);
    m_maxHit->setFocusProxy(this);
    m_maxHit->setBackground("dwgui:edit.png");

    m_thumb = new DwPushButton(this);
    m_thumb->setName(L"thumb");
    m_thumb->setMovable(true);
    m_thumb->setFocusProxy(this);
    m_thumb->setBackground("dwgui:edit.png");
#if DW_ENABLE_EVENT_DRAGDROP
    m_thumb->eventHandler()->addEventCallBack(DwDragMoveEvent::EventId(), onThumbDragMoveEvent);
#endif

    setEventHandler(new DwScrollBarEventHandler(this, m_background));
    setFlow(DwWidget::TopToBottom);
    setCur(0.0);

    // accessible
    setAccessibleName(tr("raccoon scroll bar"));
    setAccessibleDescription(tr("raccoon scroll bar"));
    setWhatsThis(tr("raccoon base scroll bar"));
}

//------------------------------------------------------------------------------
/**
*/
DwScrollBar::~DwScrollBar()
{
    m_slots->disconnect_all();
    DW_SAFE_DELETE(m_slots);
}

//------------------------------------------------------------------------------
/**
*/
void DwScrollBar::setArea(DwScrollArea* area)
{
    m_scrollArea = area;

    updateState();
}

//------------------------------------------------------------------------------
/**
*/
void DwScrollBar::setFlow(DwWidget::Flow f)
{
    m_flow = f;

    if (m_flow == DwWidget::TopToBottom)
    {
        m_minHit->clearAnchors();
        m_minHit->setHeight(22.0);
        m_minHit->anchors()->setTopMargin(1.0);
        m_minHit->anchors()->setLeftMargin(1.0);
        m_minHit->anchors()->setRightMargin(1.0);
        m_minHit->anchors()->setTop(DwAnchorLine(this, Dw::Top));
        m_minHit->anchors()->setLeft(DwAnchorLine(this, Dw::Left));
        m_minHit->anchors()->setRight(DwAnchorLine(this, Dw::Right));

        m_maxHit->clearAnchors();
        m_maxHit->setHeight(22.0);
        m_maxHit->anchors()->setBottomMargin(1.0);
        m_maxHit->anchors()->setLeftMargin(1.0);
        m_maxHit->anchors()->setRightMargin(1.0);
        m_maxHit->anchors()->setBottom(DwAnchorLine(this, Dw::Bottom));
        m_maxHit->anchors()->setLeft(DwAnchorLine(this, Dw::Left));
        m_maxHit->anchors()->setRight(DwAnchorLine(this, Dw::Right));

        m_thumb->clearAnchors();
        m_thumb->setHeight(22.0);
        m_thumb->anchors()->setLeftMargin(1.0);
        m_thumb->anchors()->setRightMargin(1.0);
        m_thumb->anchors()->setLeft(DwAnchorLine(this, Dw::Left));
        m_thumb->anchors()->setRight(DwAnchorLine(this, Dw::Right));
    }
    else
    {
        DW_ASSERT(m_flow == DwWidget::LeftToRight);
        m_minHit->clearAnchors();
        m_minHit->setWidth(22.0);
        m_minHit->anchors()->setLeftMargin(1.0);
        m_minHit->anchors()->setTopMargin(1.0);
        m_minHit->anchors()->setBottomMargin(1.0);
        m_minHit->anchors()->setLeft(DwAnchorLine(this, Dw::Left));
        m_minHit->anchors()->setTop(DwAnchorLine(this, Dw::Top));
        m_minHit->anchors()->setBottom(DwAnchorLine(this, Dw::Bottom));

        m_maxHit->clearAnchors();
        m_maxHit->setWidth(22.0);
        m_maxHit->anchors()->setRightMargin(1.0);
        m_maxHit->anchors()->setTopMargin(1.0);
        m_maxHit->anchors()->setBottomMargin(1.0);
        m_maxHit->anchors()->setRight(DwAnchorLine(this, Dw::Right));
        m_maxHit->anchors()->setTop(DwAnchorLine(this, Dw::Top));
        m_maxHit->anchors()->setBottom(DwAnchorLine(this, Dw::Bottom));

        m_thumb->clearAnchors();
        m_thumb->setWidth(22.0);
        m_thumb->anchors()->setTopMargin(1.0);
        m_thumb->anchors()->setBottomMargin(1.0);
        m_thumb->anchors()->setTop(DwAnchorLine(this, Dw::Top));
        m_thumb->anchors()->setBottom(DwAnchorLine(this, Dw::Bottom));
    }

    updateThumb();
}

//------------------------------------------------------------------------------
/**
*/
void DwScrollBar::setRange(DwReal min, DwReal max, DwReal step)
{
    m_min = min;
    m_max = max;
    m_step = step;

    updateState();
}

//------------------------------------------------------------------------------
/**
*/
void DwScrollBar::scroll(DwReal delta)
{
    if (delta > 0)
    {
        scrollDown(delta);
    }
    else
    {
        scrollUp(-delta);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwScrollBar::scrollUp(DwReal delta)
{
    if (m_cur <= m_min)  return;
    m_cur = dwClamp(m_cur - delta, m_min, m_max);
    updateState();

    DwScrollBarEvent evt;
    evt.m_flow = m_flow;
    evt.m_cur = m_cur;
    evt.m_delta = delta;
    evt.m_up = true;
    evt.m_down = false;
    sendEvent(&evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwScrollBar::scrollDown(DwReal delta)
{
    if (m_cur >= m_max)  return;
    m_cur = dwClamp(m_cur + delta, m_min, m_max);
    updateState();

    DwScrollBarEvent evt;
    evt.m_flow = m_flow;
    evt.m_cur = m_cur;
    evt.m_delta = delta;
    evt.m_down = true;
    evt.m_up = false;
    sendEvent(&evt);
}

//------------------------------------------------------------------------------
/**
    TODO: set thumb width and height auto according to m_scrollArea
*/
void DwScrollBar::updateThumb()
{
    if(!m_thumb || !m_scrollArea || width()<= 0.0 || height() <= 0)  return;
    if (m_flow == DwWidget::TopToBottom)
    {
        DwReal h = height();
        DwReal t = thumbRange();
        DwReal sc = dwMax(m_scrollArea->contentHeight() - m_scrollArea->height(), 0.0);
        DwReal newH = dwClamp<DwReal>(t-sc+m_thumb->height(), dwMax(h*0.1, 0.0), h);
        m_thumb->setHeight(newH);
        m_thumb->setY(thumbStartPoint() + m_cur * thumbRange());
    }
    else
    {
        DwReal w = width();
        DwReal t = thumbRange();
        DwReal sc = dwMax(m_scrollArea->contentWidth() - m_scrollArea->width(), 0.0);
        DwReal newW = dwClamp<DwReal>(t-sc+m_thumb->width(), dwMax(w*0.1, 0.0), w);
        m_thumb->setWidth(newW);
        m_thumb->setX(thumbStartPoint() + m_cur * thumbRange());
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwScrollBar::updateArea()
{
    if (!m_scrollArea) return;
    if ((m_max - m_min) < DW_TINY) return;

    if (m_flow == DwWidget::TopToBottom)
    {
        m_scrollArea->scrollVerticalTo(m_cur / (m_max - m_min));
    }
    else
    {
        m_scrollArea->scrollHorizialTo(m_cur / (m_max - m_min));
    }
    update();
}

//------------------------------------------------------------------------------
/**
*/
void DwScrollBar::updateState()
{
    updateThumb();
    updateArea();
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwScrollBar::piexl2Range(int delta)
{
    if(delta == 0 || height() <= DW_TINY || width() <= DW_TINY)  return 0.;

    DwReal widthRange = thumbRange();
    if (widthRange <= DW_TINY) return 0.;
    return delta / widthRange;
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwScrollBar::thumbRange()
{
    if (m_flow == DwWidget::TopToBottom)
    {
        return height()
               - m_minHit->height()
               - m_minHit->anchors()->topMargin()
               - m_minHit->anchors()->bottomMargin()
               - m_maxHit->height()
               - m_maxHit->anchors()->topMargin()
               - m_maxHit->anchors()->bottomMargin()
               - m_thumb->height()
               - m_thumb->anchors()->topMargin()
               - m_thumb->anchors()->bottomMargin();
    }
    else
    {
        return width()
               - m_minHit->width()
               - m_minHit->anchors()->leftMargin()
               - m_minHit->anchors()->rightMargin()
               - m_maxHit->width()
               - m_maxHit->anchors()->leftMargin()
               - m_maxHit->anchors()->rightMargin()
               - m_thumb->width()
               - m_thumb->anchors()->leftMargin()
               - m_thumb->anchors()->rightMargin();
    }
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwScrollBar::thumbStartPoint()
{
    if (m_flow == DwWidget::TopToBottom)
    {
        return (m_minHit->height()
                + m_minHit->anchors()->topMargin()
                + m_minHit->anchors()->bottomMargin()
                + m_thumb->anchors()->topMargin());
    }
    else
    {
        return (m_minHit->width()
                + m_minHit->anchors()->leftMargin()
                + m_minHit->anchors()->rightMargin()
                + m_thumb->anchors()->leftMargin());
    }
}