#include "stable.h"
#include "dwscrollarea.h"
#include "dwscrollbar.h"
#include "dwgui/dwgraphicsitemlistener.h"
#include "dwgui/dwanchors.h"
#include "dwwidgeteventhandler.h"
#include "dwgui/dwwidgetevent.h"
#include "dwgui/dwgraphicsbackgrounditem.h"

//------------------------------------------------------------------------------
const char KNormalImage[] = "dwgui:edit.png";

DW_IMPLEMENT_CLASS(DwScrollArea, 'SLAA', DwWidgetItem);

//------------------------------------------------------------------------------
/**
*/
class DwContentGemoryListener : public DwGraphicsItemListener
{
public:
    DwContentGemoryListener(DwScrollArea* scrollArea)
        : m_area(scrollArea)
    {
        DW_ASSERT(m_area);
        setChange(Dw::Geometry, true);
    }

    virtual ~DwContentGemoryListener()
    {
        m_area = NULL;
        remAllDepends();
    }

    // TODO: Control The Thumb Size
    virtual void onChange(Dw::ChangeType change,
                          DwGraphicsItem* item, const DwVariant& newOne, const DwVariant& oldOne)
    {
        DW_UNUSED(item);
        DW_UNUSED(newOne);
        DW_UNUSED(oldOne);
        if(!m_area || !m_area->content()) return;
        DW_ASSERT(item == (DwGraphicsItem*)(m_area->content()) || item == m_area);
        if (change == Dw::Geometry)
        {
            m_area->updateScrollContent();
        }
    };

    virtual void onAdded(DwGraphicsItem* item)
    {
        DW_UNUSED(item);
    }

    virtual void onRemoved(DwGraphicsItem* item)
    {
        DW_UNUSED(item);
    }

    DwScrollArea* m_area;
};

//------------------------------------------------------------------------------
/**
*/
static void onContentWheelEvent(DwGraphicsItem *item, DwEvent *evt)
{
    DwScrollArea * area = dwsafe_cast<DwScrollArea>(item->parentItem());
    DwMouseWheelEvent *wheelEvt = dwsafe_cast<DwMouseWheelEvent>(evt);
    if (area && wheelEvt)
    {
        if (wheelEvt->isRollUp())
        {
            if (area->verticalScroll())
            {
                area->verticalScroll()->scrollUp(area->verticalScroll()->step());
            }
            else if (area->horizontalScroll())
            {
                area->horizontalScroll()->scrollUp(area->horizontalScroll()->step());
            }
        }
        else
        {
            if (area->verticalScroll())
            {
                area->verticalScroll()->scrollDown(area->verticalScroll()->step());
            }
            else if (area->horizontalScroll())
            {
                area->horizontalScroll()->scrollDown(area->horizontalScroll()->step());
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
DwScrollArea::DwScrollArea(DwGraphicsItem *parent/* = NULL*/)
    : DwWidgetItem(parent)
    , m_content(NULL)
    , m_horizontalScroll(NULL)
    , m_verticalScroll(NULL)
    , m_listener(NULL)
    , m_updateMe(false)
{
    setName(L"scrollarea");
    setFlag(Dw::ItemClipsChildrenToShape, true);
    m_listener = new DwContentGemoryListener(this);
    addListener(m_listener);

    m_background->setZValue(-2.0);// most back, then the content
    m_background->setURL(KNormalImage);

    // accessible
    setAccessibleName(tr("raccoon scroll area"));
    setAccessibleDescription(tr("raccoon scroll area"));
    setWhatsThis(tr("raccoon base scroll area"));
}

//------------------------------------------------------------------------------
/**
*/
DwScrollArea::~DwScrollArea()
{
    removeListener(m_listener);
    DW_SAFE_DELETE(m_listener);
}

//------------------------------------------------------------------------------
/**
*/
void DwScrollArea::setContent(DwGraphicsItem *content)
{
    DW_ASSERT(content);
    if (m_content != content)
    {
        DW_SAFE_DELETE(m_content);
        m_content = content;
        m_content->setParentItem(this);
        m_content->setZValue(-1.0); // to confirm at back
        m_content->addListener(m_listener);

        // scroll to cur, init state
        updateScrollContent();

        if (m_content->eventHandler())
        {
            m_content->eventHandler()->addEventCallBack(DwMouseWheelEvent::EventId(), onContentWheelEvent);
        }

        // accessible
        m_content->setAccessible(true);
    }
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwScrollArea::clipRect() const
{
    return DwWidgetItem::clipRect();
}

//------------------------------------------------------------------------------
/**
	TODO: Animation
*/
void DwScrollArea::scrollHorizialTo(DwReal percent)
{
    if(!m_content) return;
    if( contentWidth() <= width() )
    {
        m_content->setClipRect(DwRectF());
        m_content->setX(m_content->anchors()->leftMargin());
        return;
    }
    if(width() < 0.00001) return;

    DwReal range = width() - contentWidth();
    m_updateMe = true;
    m_content->setX(m_content->anchors()->leftMargin() + range * percent);
    updateContentClipRect();
    m_updateMe = false;

    DwScrollAreaEvent evt;
    evt.m_flow = DwWidget::LeftToRight;
    evt.m_percent = percent;
    sendEvent(&evt);
}

//------------------------------------------------------------------------------
/**
	TODO: Animation
*/
void DwScrollArea::scrollVerticalTo(DwReal percent)
{
    if(!m_content) return;
    if(contentHeight() <= height())
    {
        m_content->setClipRect(DwRectF());
        m_content->setY(m_content->anchors()->topMargin());
        return;
    }
    if(height() < 0.00001) return;

    DwReal range = height() - contentHeight();
    m_updateMe = true;
    m_content->setY(m_content->anchors()->topMargin() + range * percent);
    updateContentClipRect();
    m_updateMe = false;

    DwScrollAreaEvent evt;
    evt.m_flow = DwWidget::TopToBottom;
    evt.m_percent = percent;
    sendEvent(&evt);
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwScrollArea::contentWidth() const
{
    if (!m_content) return 0.0;
    return m_content->width() + m_content->anchors()->leftMargin()
           + m_content->anchors()->rightMargin();
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwScrollArea::contentHeight() const
{
    if (!m_content) return 0.0;
    return m_content->height() + m_content->anchors()->topMargin()
           + m_content->anchors()->bottomMargin();
}

//------------------------------------------------------------------------------
/**
*/
void DwScrollArea::updateScrollBar()
{
    if (m_updateMe) return;

    m_updateMe = true;
    if (contentHeight() > height())
    {
        if (! m_verticalScroll)
        {
            m_verticalScroll = new DwScrollBar(this);
            m_verticalScroll->setName(L"verticalscroll");
            m_verticalScroll->setWidth(15);
            m_verticalScroll->setVisible(false);
            m_verticalScroll->setFlow(DwWidget::TopToBottom);
            m_verticalScroll->anchors()->setTopMargin(1.0);
            m_verticalScroll->anchors()->setBottomMargin(1.0);
            m_verticalScroll->anchors()->setRightMargin(2.0);
            m_verticalScroll->anchors()->setTop(DwAnchorLine(this, Dw::Top));
            m_verticalScroll->anchors()->setBottom(DwAnchorLine(this, Dw::Bottom));
            m_verticalScroll->anchors()->setRight(DwAnchorLine(this, Dw::Right));
            m_verticalScroll->updateState();
        }

        if (! m_verticalScroll->isVisible())
        {
            m_verticalScroll->setVisible(true);
            if (m_horizontalScroll && m_horizontalScroll->isVisible())
            {
                m_verticalScroll->anchors()->setBottom(DwAnchorLine(m_horizontalScroll, Dw::Top));
            }
            else
            {
                m_verticalScroll->anchors()->setBottom(DwAnchorLine(this, Dw::Bottom));
            }
        }
    }
    else
    {
        if (m_verticalScroll && m_verticalScroll->isVisible())
        {
            m_verticalScroll->setVisible(false);
            m_verticalScroll->setCur(0.0);// fix
        }

        if (m_horizontalScroll && m_horizontalScroll->isVisible())
        {
            if (m_verticalScroll && m_horizontalScroll->anchors()->right().item == m_verticalScroll)
            {
                m_horizontalScroll->anchors()->setRight(DwAnchorLine(this, Dw::Right));
            }
        }
    }

    if (contentWidth() > width())
    {
        if (!m_horizontalScroll)
        {
            m_horizontalScroll = new DwScrollBar(this);
            m_horizontalScroll->setName(L"horizontalscroll");
            m_horizontalScroll->setHeight(15);
            m_horizontalScroll->setVisible(false);
            m_horizontalScroll->setFlow(DwWidget::LeftToRight);
            m_horizontalScroll->anchors()->setLeftMargin(2.0);
            m_horizontalScroll->anchors()->setBottomMargin(2.0);
            m_horizontalScroll->anchors()->setRightMargin(1.0);
            m_horizontalScroll->anchors()->setLeft(DwAnchorLine(this, Dw::Left));
            m_horizontalScroll->anchors()->setRight(DwAnchorLine(m_verticalScroll, Dw::Left));
            m_horizontalScroll->anchors()->setBottom(DwAnchorLine(this, Dw::Bottom));
            m_horizontalScroll->updateState();
        }

        if (! m_horizontalScroll->isVisible())
        {
            m_horizontalScroll->setVisible(true);
            if (m_verticalScroll && m_verticalScroll->isVisible())
            {
                m_horizontalScroll->anchors()->setRight(DwAnchorLine(m_verticalScroll, Dw::Left));
            }
            else
            {
                m_horizontalScroll->anchors()->setRight(DwAnchorLine(this, Dw::Right));
            }
        }
    }
    else
    {
        if (m_horizontalScroll && m_horizontalScroll->isVisible())
        {
            m_horizontalScroll->setVisible(false);
            m_horizontalScroll->setCur(0.0);// fix
        }

        if (m_verticalScroll && m_verticalScroll->isVisible())
        {
            if (m_horizontalScroll && m_verticalScroll->anchors()->bottom().item == m_horizontalScroll)
            {
                m_verticalScroll->anchors()->setBottom(DwAnchorLine(this, Dw::Bottom));
            }
        }
    }

    if (m_horizontalScroll && m_horizontalScroll->isVisible())
    {
        setFocusProxy(m_horizontalScroll);
    }
    else if (m_verticalScroll && m_verticalScroll->isVisible())
    {
        setFocusProxy(m_verticalScroll);
    }
    m_updateMe = false;
}

//------------------------------------------------------------------------------
/**
*/
void DwScrollArea::updateContentClipRect()
{
    if(!m_content) return;

    // clip rect
    if (m_content->flags() & Dw::ItemClipsChildrenToShape
            || m_content->flags() & Dw::ItemClipsToShape)
    {
        DwPointF p0(m_content->anchors()->leftMargin(), m_content->anchors()->topMargin());
        p0 = m_content->transform().inverted().map(p0);
        DwPointF p1(
            dwMin(m_content->anchors()->leftMargin() + m_content->width(), width()
                  - m_content->anchors()->rightMargin()),
            dwMin(m_content->anchors()->topMargin() + m_content->height(), height()
                  - m_content->anchors()->bottomMargin()));
        p1 = m_content->transform().inverted().map(p1);
        m_content->setClipRect( DwRectF( p0 , p1));
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwScrollArea::updateScrollContent()
{
    if(m_updateMe) return;

    // update scroll bar state
    updateScrollBar();
    // scroll to cur
    if(m_horizontalScroll) m_horizontalScroll->updateState();
    if(m_verticalScroll) m_verticalScroll->updateState();
    // update clip rect
    updateContentClipRect();
}