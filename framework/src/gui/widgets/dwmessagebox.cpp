#include "stable.h"
#include "dwmessagebox.h"
#include "dwgui/dwgraphicsframeitem.h"
#include "dwgui/dwgraphicsmarginspixmapitem.h"
#include "dwgui/dwanchors.h"
#include "dwgraphics/dwregion.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwpushbutton.h"
#include "dwgui/dwgraphicseventhandler.h"
#include "dwgui/dwpropertyanimation.h"
#include "dwgui/dwrowpositioner.h"
#include "dwgui/dwgraphicspixmapitem.h"
#include "dwgui/dwgraphicssimpletextitem.h"
#include "dwgui/dwgraphicsmarginspixmapitem.h"
#include "dwgui/dwurl.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwwidgetevent.h"
#include "dwgui/dwgraphicsninepatchitem.h"

const char KClosedNormalImage[] = "dwgui:close_normal.png";
const char KClosedHoverImage[] = "dwgui:close_hover.png";
const char KClosedDownImage[] = "dwgui:close_down.png";

DW_IMPLEMENT_CLASS(DwMessageBox, 'MBOX', DwWidgetItem);
DW_IMPLEMENT_PROPERTY(DwMessageBox);

//------------------------------------------------------------------------------
/**
*/
static void onButtonClicked(DwGraphicsItem *item, DwEvent *evt)
{
    DW_UNUSED(evt);
    DwPushButton *button = dwsafe_cast<DwPushButton>(item);
    DwMessageBox* messabebox = dwsafe_cast<DwMessageBox>(button->parentItem()->parentItem()->parentItem());
    if (messabebox)
    {
        messabebox->setVisible(false);

        DwMessageBoxEvent msgEvt;
        msgEvt.m_btn = button;
        messabebox->sendEvent(&msgEvt);

        messabebox->deleteLater();
    }
}

//------------------------------------------------------------------------------
/**
*/
static void onMessageBoxClicked(DwGraphicsItem *item, DwEvent *evt)
{
    DW_UNUSED(evt);
    DwMessageBox* messagebox = dwsafe_cast<DwMessageBox>(item);
    if (messagebox)
    {
        //::Beep( 750, 300 );

        if ( messagebox->animationNum() == 0)
        {
            DwPropertyAnimation *anims = new DwPropertyAnimation(NULL);
            anims->setTargetItem(messagebox);
            anims->setProperty("flash");
            anims->setStartValue(1);
            anims->setEndValue(30);
            anims->setDuration(1000);
            anims->start(Dw::DeleteWhenStopped);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
static void onContentGemotryChange(DwGraphicsItem *item, DwEvent *evt)
{
    DW_UNUSED(item);
    DW_UNUSED(evt);
#if DW_ENABLE_EVENT_ITEM_INNER
    DwItemChangeEvent *changEvt = dwsafe_cast<DwItemChangeEvent>(evt);
    DwWidgetItem *content = dwsafe_cast<DwWidgetItem>(item);
    if (changEvt && content)
    {
        content->setClipRegion(&DwRegion(content->boundingRect().toRect(), 6, 6));
    }
#endif
}

//------------------------------------------------------------------------------
/**
*/
DwMessageBox::DwMessageBox(DwGraphicsItem *parent/* = NULL*/)
    : DwWidgetItem(parent)
    , m_flash(0)
{
    DW_ASSERT(parent);
    setVisible(false);
    setZValue(10);
    setDragPolicy(DwWidget::AsParent);

    anchors()->setFill(parent);
    eventHandler()->addEventCallBack(DwMouseClickedEvent::EventId(), onMessageBoxClicked);

    m_content = new DwWidgetItem(this);
    m_content->setSize(DwSizeF(280, 200));
    m_content->anchors()->setCenterIn(this);
    m_content->setClipRegion(&DwRegion(m_content->boundingRect().toRect(), 6, 6));
    m_content->setFlag(Dw::ItemClipsChildrenToShape, true);
    m_content->setSendChangedEvent(true);
#if DW_ENABLE_EVENT_ITEM_INNER
    m_content->eventHandler()->addEventCallBack(DwItemChangeEvent::EventId(), onContentGemotryChange);
#endif

    m_background = new DwGraphicsMarginsPixmapItem(m_content);
    m_background->anchors()->setFill(m_content);

    // head Part
    DwGraphicsItem *headPart=new DwGraphicsItem(m_content);
    headPart->setHeight(38);
    headPart->anchors()->setLeft(DwAnchorLine(m_content, Dw::Left));
    headPart->anchors()->setTop(DwAnchorLine(m_content, Dw::Top));
    headPart->anchors()->setRight(DwAnchorLine(m_content, Dw::Right));

    m_headIcon = new DwGraphicsPixmapItem(headPart);
    m_headIcon->anchors()->setLeftMargin(8);
    m_headIcon->anchors()->setLeft(DwAnchorLine(headPart, Dw::Left));
    m_headIcon->anchors()->setVerticalCenter(DwAnchorLine(headPart, Dw::VCenter));

    m_headText = new DwGraphicsSimpleTextItem(headPart);
    m_headText->anchors()->setLeft(DwAnchorLine(m_headIcon, Dw::Right));
    m_headText->anchors()->setVerticalCenter(DwAnchorLine(headPart, Dw::VCenter));

    m_closed = new DwPushButton(headPart);
    m_closed->setName(L"closed");
    m_closed->setToolTips(tr("closed"));
    m_closed->eventHandler()->addEventCallBack(DwMouseClickedEvent::EventId(), onButtonClicked);
    m_closed->setBackground("dwgui:close.png");
    m_closed->anchors()->setTop(DwAnchorLine(headPart, Dw::Top));
    m_closed->anchors()->setRight(DwAnchorLine(headPart, Dw::Right));

    // bottom part
    DwGraphicsItem *bottomPart = new DwGraphicsItem(m_content);
    bottomPart->setHeight(40);
    bottomPart->anchors()->setLeft(DwAnchorLine(m_content, Dw::Left));
    bottomPart->anchors()->setBottom(DwAnchorLine(m_content, Dw::Bottom));
    bottomPart->anchors()->setRight(DwAnchorLine(m_content, Dw::Right));

    m_cancle = new DwPushButton(bottomPart);
    m_cancle->setName(L"cancle");
    m_cancle->eventHandler()->addEventCallBack(DwMouseClickedEvent::EventId(), onButtonClicked);
    m_cancle->setTextOnly();
    m_cancle->setText(tr("Cancle"));
    m_cancle->anchors()->setRightMargin(20);
    m_cancle->anchors()->setRight(DwAnchorLine(bottomPart, Dw::Right));
    m_cancle->anchors()->setVerticalCenter(DwAnchorLine(bottomPart, Dw::VCenter));

    m_ok = new DwPushButton(bottomPart);
    m_ok->setName(L"ok");
    m_ok->eventHandler()->addEventCallBack(DwMouseClickedEvent::EventId(), onButtonClicked);
    m_ok->setTextOnly();
    m_ok->setText(tr("OK"));
    m_ok->anchors()->setRightMargin(20);
    m_ok->anchors()->setRight(DwAnchorLine(m_cancle, Dw::Left));
    m_ok->anchors()->setVerticalCenter(DwAnchorLine(bottomPart, Dw::VCenter));

    // center part
    DwGraphicsItem *centerPart = new DwGraphicsItem(m_content);
    centerPart->anchors()->setLeft(DwAnchorLine(m_content, Dw::Left));
    centerPart->anchors()->setTop(DwAnchorLine(headPart, Dw::Bottom));
    centerPart->anchors()->setRight(DwAnchorLine(m_content, Dw::Right));
    centerPart->anchors()->setBottom(DwAnchorLine(bottomPart, Dw::Top));

    DwRowPositioner *centerContent = new DwRowPositioner(centerPart);
    centerContent->setSpacing(5);
    centerContent->anchors()->setLeftMargin(44);
    centerContent->anchors()->setVerticalCenter(DwAnchorLine(centerPart, Dw::VCenter));
    centerContent->anchors()->setLeft(DwAnchorLine(centerPart, Dw::Left));
    m_centerContent = centerContent;

    m_icon = new DwGraphicsPixmapItem(centerContent);
    m_text = new DwGraphicsSimpleTextItem(centerContent);
    m_icon->anchors()->setVerticalCenter(DwAnchorLine(centerContent, Dw::VCenter));
    m_text->anchors()->setVerticalCenter(DwAnchorLine(centerContent, Dw::VCenter));

    m_border = new DwGraphicsNinePatchItem(this);
    m_border->setZValue(1);
    m_border->setName(L"border");
    m_border->anchors()->setMargins(-5);// the 5 according to the KBorderMask
    m_border->anchors()->setFill(m_content);
    m_border->setPixmap("dwgui:shadow.png");
    m_border->setDrawCenter(false);

    // accessible
    headPart->setAccessible(true);
    bottomPart->setAccessible(true);
    centerPart->setAccessible(true);
    m_centerContent->setAccessible(true);

    m_content->setAccessibleName(tr("message box content"));
    headPart->setAccessibleName(tr("top part message box"));
    centerPart->setAccessibleName(tr("center part message box"));

    m_closed->setAccessibleName(m_closed->toolTips());

    //accessible
    setAccessibleName(tr("raccoon message box"));
    setAccessibleDescription(tr("raccoon message box"));
    setWhatsThis(tr("the raccoon engine's inner message box"));
}

//------------------------------------------------------------------------------
/**
*/
DwMessageBox::~DwMessageBox()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwMessageBox::show()
{
    setVisible(true);
}

//------------------------------------------------------------------------------
/**
*/
void DwMessageBox::setSize(const DwSizeF &s)
{
    m_content->setSize(s);
}

//------------------------------------------------------------------------------
/**
*/
void DwMessageBox::setCaption(const DwString &txt)
{
    m_headText->setText(txt);
}

//------------------------------------------------------------------------------
/**
*/
void DwMessageBox::setHeadIcon(const DwUrl &url)
{
    m_headIcon->setPixmap(DW_PIXMAP(url));
}

//------------------------------------------------------------------------------
/**
*/
void DwMessageBox::setIcon(const DwUrl &url)
{
    m_icon->setPixmap(DW_PIXMAP(url));
}

//------------------------------------------------------------------------------
/**
*/
void DwMessageBox::setText(const DwString &txt)
{
    m_text->setText(txt);

    // accessible
    setAccessibleDescription(txt);
}

//------------------------------------------------------------------------------
/**
*/
void DwMessageBox::setBackground(const DwUrl &url)
{
    m_background->setPixmap(url);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsMarginsPixmapItem *DwMessageBox::background() const
{
    return m_background;
}

//------------------------------------------------------------------------------
/**
*/
void DwMessageBox::setButtonText(DwWidget::MessageBoxButton b, const DwString &txt)
{
    switch (b)
    {
    case DwWidget::MBB_OK:
        m_ok->setText(txt);
        break;
    case DwWidget::MBB_CANCLE:
        m_cancle->setText(txt);
        break;
    case DwWidget::MBB_CLOSED:
        m_closed->setText(txt);
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
DwPushButton *DwMessageBox::buttonOf(DwWidget::MessageBoxButton b) const
{
    switch (b)
    {
    case DwWidget::MBB_OK:
        return m_ok;
        break;
    case DwWidget::MBB_CANCLE:
        return m_cancle;
        break;
    case DwWidget::MBB_CLOSED:
        return m_closed;
        break;
    }
    DW_ASSERT("wrong message box button index" && false);
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSimpleTextItem *DwMessageBox::headText() const
{
    return m_headText;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsPixmapItem *DwMessageBox::headIcon() const
{
    return m_headIcon;
}

//------------------------------------------------------------------------------
/**
*/
void DwMessageBox::setHeadIconTextSpacing(DwReal s)
{
    m_headText->anchors()->setLeftMargin(s);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSimpleTextItem *DwMessageBox::text() const
{
    return m_text;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsPixmapItem *DwMessageBox::icon() const
{
    return m_icon;
}

//------------------------------------------------------------------------------
/**
*/
DwRowPositioner *DwMessageBox::centerContent() const
{
    return m_centerContent;
}

//------------------------------------------------------------------------------
/**
*/
void DwMessageBox::setIconTextSpacing(DwReal s)
{
    m_centerContent->setSpacing(s);
}

//------------------------------------------------------------------------------
/**
*/
void DwMessageBox::setMode(DwWidget::MessageBoxMode m)
{
    switch (m)
    {
    case DwWidget::MBM_OK:
        m_ok->setVisible(true);
        m_cancle->setVisible(false);
        break;
    case DwWidget::MBM_OKCANCEL:
        m_ok->setVisible(true);
        m_cancle->setVisible(true);
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwMessageBox::setFlash(int i)
{
    m_flash = i;
    if (i%2 == 0)
    {
        m_border->setVisible(true);
    }
    else
    {
        m_border->setVisible(false);
    }
}

//------------------------------------------------------------------------------
/**
*/
int DwMessageBox::flash() const
{
    return m_flash;
}