#include "stable.h"
#include "dwpushbutton.h"
#include "dwgui/dwgraphicspixmapitem.h"
#include "dwgui/dwgraphicssimpletextitem.h"
#include "dwgui/dwgridpositioner.h"
#include "dwgui/dwanchors.h"
#include "dwgui/dwgraphicsninepatchitem.h"
#include "dwgraphics/dwimage.h"
#include "dwgraphicsframeitem.h"
#include "dwwidgeteventhandler.h"
#include "dwgui/dwgraphicsbackgrounditem.h"

const char KNormalImage[] = "dwgui:btn_4word_normal.png";
const char KPressdownImage[] = "dwgui:btn_4word_pushed.png";
const char KHoverImage[]  = "dwgui:btn_4word_hover.png";

DW_IMPLEMENT_CLASS(DwPushButton, 'PBTN', DwWidgetItem);

//------------------------------------------------------------------------------
/**
*/
static void onMouseDown(DwGraphicsItem *item, DwEvent*)
{
    DwPushButton *btn = dwsafe_cast<DwPushButton>(item);
    if (btn)
    {
        btn->setContentOffset(btn->offset());
    }
}

//------------------------------------------------------------------------------
/**
*/
static void onMouseUp(DwGraphicsItem *item, DwEvent *)
{
    DwPushButton *btn = dwsafe_cast<DwPushButton>(item);
    if (btn)
    {
        btn->setContentOffset(DwPointF(0, 0));
    }
}

//------------------------------------------------------------------------------
/**
*/
DwPushButton::DwPushButton(DwGraphicsItem *parent/* = NULL*/)
    : DwWidgetItem(parent)
    , m_horizable(true)
    , m_icon(NULL)
    , m_text(NULL)
{
    setName(L"pushButton");
    setBoundingRect(DwRectF(0, 0, 71, 28));
    setBackground("dwgui:button.png");

    m_content = new DwGraphicsItem(this);
    m_content->anchors()->setFill(this);

    setHorizable(true);
    setOffset(DwPointF(1, 1));

    eventHandler()->addEventCallBack(DwMouseDownEvent::EventId(), onMouseDown);
    eventHandler()->addEventCallBack(DwMouseUpEvent::EventId(), onMouseUp);

    //accessible
    setAccessibleName(tr("raccoon push button"));
    setAccessibleDescription(tr("raccoon push button"));
    setWhatsThis(tr("the raccoon engine's inner push button"));
}

//------------------------------------------------------------------------------
/**
*/
DwPushButton::~DwPushButton()
{
    ;
}

//------------------------------------------------------------------------------
/**
    if background is not a nine path, we will set the button size 
*/
void DwPushButton::setBackground(const DwUrl& backImage)
{
    m_background->setURL(backImage);

    DwNinePatch::NinePatchInfo info = DwNinePatch::ninePatchInfo(DW_PIXMAP(backImage), 0);
    if (info.margins.isZero())
    {
        setSize(info.srcRect.size());
    }
    else 
    {
        if(!info.margins.left() && !info.margins.right())
        {
            setWidth(info.srcRect.width());
        }
        if (!info.margins.top() && !info.margins.bottom())
        {
            setHeight(info.srcRect.height());
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwPushButton::setIcon(const DwUrl& icon, int idx/*=0*/)
{
    if (!m_icon)
    {
        m_icon = new DwGraphicsNinePatchItem(m_content);
        m_icon->setVisible(false);
        m_icon->setName(L"icon");
        setHorizable(m_horizable);
    }
    m_icon->setNinePatch(icon, idx);
    m_icon->setVisible(true);
}

//------------------------------------------------------------------------------
/**
*/
void DwPushButton::setText(const DwString& text)
{
    if (!m_text)
    {
        m_text = new DwGraphicsSimpleTextItem(m_content);
        m_text->setVisible(false);
        m_text->setName(L"text");
        setHorizable(m_horizable);
    }
    m_text->setText(text);
    m_text->setVisible(true);

    // accessible
    setAccessibleDescription(text);
}

//------------------------------------------------------------------------------
/**
*/
void DwPushButton::setHorizable(bool h)
{
    m_horizable = h;
    if (h)
    {
        if (m_icon)
        {
            m_icon->clearAnchors();
            m_icon->anchors()->setLeft(DwAnchorLine(m_content, Dw::Left));
            m_icon->anchors()->setVerticalCenter(DwAnchorLine(m_content, Dw::VCenter));
        }

        if (m_text)
        {
            m_text->clearAnchors();
            if(m_icon)
            {
                m_text->anchors()->setLeft(DwAnchorLine(m_icon, Dw::Right));
            }
            else
            {
                m_text->anchors()->setLeft(DwAnchorLine(m_content, Dw::Left));
            }

            m_text->anchors()->setVerticalCenter(DwAnchorLine(m_content, Dw::VCenter));
        }
    }
    else
    {
        if (m_icon)
        {
            m_icon->clearAnchors();
            m_icon->anchors()->setTop(DwAnchorLine(m_content, Dw::Top));
            m_icon->anchors()->setHorizontalCenter(DwAnchorLine(m_content, Dw::HCenter));
        }

        if (m_text)
        {
            m_text->clearAnchors();
            if (m_icon)
            {
                m_text->anchors()->setTop(DwAnchorLine(m_icon, Dw::Bottom));
            }
            else
            {
                m_text->anchors()->setTop(DwAnchorLine(m_content, Dw::Top));
            }

            m_text->anchors()->setHorizontalCenter(DwAnchorLine(m_content, Dw::HCenter));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwPushButton::setButtonMargins(DwReal iconLeft, DwReal textLeft, DwReal textRight)
{
    if (!m_icon || !m_text)
    {
        DW_ASSERT("only for icon and text button" && false);
        return;
    }

    if (m_horizable)
    {
        m_icon->anchors()->setLeftMargin(iconLeft);
        m_text->anchors()->setLeftMargin(textLeft);
        m_text->anchors()->setRightMargin(textRight);
    }
    else
    {
        m_icon->anchors()->setTopMargin(iconLeft);
        m_text->anchors()->setTopMargin(textLeft);
        m_text->anchors()->setRightMargin(textRight);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwPushButton::setTextOnly()
{
    DW_SAFE_DELETE(m_icon);
    if (!m_text)
    {
        m_text = new DwGraphicsSimpleTextItem(m_content);
        m_text->setVisible(false);
        m_text->setName(L"text");
    }
    m_text->clearAnchors();
    m_text->anchors()->setCenterIn(m_content);
}

//------------------------------------------------------------------------------
/**
*/
void DwPushButton::setIconOnly()
{
    DW_SAFE_DELETE(m_text);
    if (!m_icon)
    {
        m_icon = new DwGraphicsNinePatchItem(m_content);
        m_icon->setVisible(false);
        m_icon->setName(L"icon");
    }
    m_icon->clearAnchors();
    m_icon->anchors()->setCenterIn(m_content);
}

//------------------------------------------------------------------------------
/**
*/
void DwPushButton::adjustSize()
{
    if (!m_icon || !m_text)
    {
        return;
    }

    if (m_horizable)
    {
        DwReal contentWidth = m_icon->width() + m_text->width();
        DwReal spaceWidth = m_icon->anchors()->leftMargin()
                            + m_text->anchors()->leftMargin()
                            + m_text->anchors()->rightMargin();
        setWidth(contentWidth + spaceWidth);
    }
    else
    {
        DwReal contentHeight = m_icon->height() + m_text->height();
        DwReal spaceHeight = m_icon->anchors()->topMargin()
                             + m_text->anchors()->topMargin()
                             + m_text->anchors()->bottomMargin();
        setHeight(contentHeight + spaceHeight);
    }
}

//------------------------------------------------------------------------------
/**
*/
DwPointF DwPushButton::offset() const
{
    return m_offset;
}

//------------------------------------------------------------------------------
/**
*/
void DwPushButton::setOffset(const DwPointF &o)
{
    m_offset = o;
}

//------------------------------------------------------------------------------
/**
*/
void DwPushButton::setContentOffset(const DwPointF &o)
{
    m_content->anchors()->setLeftMargin(o.x());
    m_content->anchors()->setRightMargin(-o.x());

    m_content->anchors()->setTopMargin(o.y());
    m_content->anchors()->setBottomMargin(-o.y());
}