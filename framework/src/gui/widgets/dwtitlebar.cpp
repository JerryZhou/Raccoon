#include "stable.h"
#include "dwtitlebar.h"
#include "dwlabel.h"
#include "dwimagewidget.h"
#include "dwpushbutton.h"
#include "dwgui/dwrowpositioner.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwanchors.h"
#include "dwgui/dwgraphicsview.h"
#include "dwgui/dwwindow.h"
#include "dwgui/dwgraphicssimpletextitem.h"
#include "dwcore/sigslot.h"
#include "dwwidgeteventhandler.h"
#include "dwframewindow.h"

//------------------------------------------------------------------------------
const char KHeadImage[] = "dwgui:head.png";

const char KClosedNormalImage[] = "dwgui:close.png";

const char KMaxNormalImage[] = "dwgui:max.png";

const char KMinNormalImage[] = "dwgui:min.png";

const char KRestoreNormalImage[] = "dwgui:restore.png";

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwTitleBar, 'TEAR', DwWidgetItem);

//------------------------------------------------------------------------------
/**
*/
static void onButtonClicked(DwGraphicsItem *item, DwEvent *evt)
{
    DW_UNUSED(evt);
    DwPushButton *button = (DwPushButton*)(item);
    DwTitleBar* titleBar = (DwTitleBar*)(button->parentItem()->parentItem());
    DwFrameWindow* frameWindow = titleBar->window();
    if (frameWindow)
    {
        if (button->name() == L"closed")
        {
            frameWindow->close();
        }
        else if (button->name() == L"maximum")
        {
            frameWindow->show(Dw::SW_ShowMaximized);
        }
        else if (button->name() == L"minimum")
        {
            frameWindow->show(Dw::SW_ShowMinimized);
        }
        else if (button->name() == L"restore")
        {
            frameWindow->show(Dw::SW_Restore);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
static void onResizeEvt(DwGraphicsItem *item, DwEvent *evt)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(item);
    DwResizeEvent *resizeEvt = dwsafe_cast<DwResizeEvent>(evt);
    if (window && resizeEvt)
    {
        if(window->titleBar())
        {
            window->titleBar()->checkResizePattern(resizeEvt->m_pattern);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
DwTitleBar::DwTitleBar(DwFrameWindow *window)
    : DwWidgetItem((DwGraphicsItem*)window)
    , m_window(NULL)
    , m_caption(NULL)
    , m_icon(NULL)
    , m_buttons(NULL)
    , m_closed(NULL)
    , m_maximum(NULL)
    , m_minimum(NULL)
    , m_restore(NULL)
{
    setHeight(50);
    setMovable(false);
    setFocusable(false);
    setHoverable(false);
    setFlag(Dw::ItemStopsMousePropagation, false);
    setAcceptedMouseButton(Dw::LeftButton, false);
    setDragPolicy(DwWidget::DragWindow);

    setName(L"titlebar");

    m_icon = new DwImageWidget(this);
    m_icon->setName(L"icon");
    m_icon->anchors()->setLeftMargin(3);
    m_icon->anchors()->setLeft(DwAnchorLine(this, Dw::Left));
    m_icon->anchors()->setVerticalCenter(DwAnchorLine(this, Dw::VCenter));
    m_icon->setDragPolicy(DwWidget::DragWindow);

    m_caption = new DwLabel(this);
    m_caption->setName(L"caption");
    m_caption->text()->setFontSize(14);
    m_caption->anchors()->setLeftMargin(2);
    m_caption->anchors()->setLeft(DwAnchorLine(m_icon, Dw::Right));
    m_caption->anchors()->setVerticalCenter(DwAnchorLine(this, Dw::VCenter));
    m_caption->setDragPolicy(DwWidget::DragWindow);

    setIcon(KHeadImage);

    if (window)
    {
        setWindow(window);
    }

    //accessible
    setAccessibleName(tr("raccoon title bar"));
    setAccessibleDescription(tr("raccoon title bar"));
    setWhatsThis(tr("the raccoon engine's inner title bar"));
}

//------------------------------------------------------------------------------
/**
*/
DwTitleBar::~DwTitleBar()
{
    setWindow(NULL);
}

//------------------------------------------------------------------------------
/**
*/
void DwTitleBar::setCaption(const DwString &s)
{
    m_caption->setText(s);
}

//------------------------------------------------------------------------------
/**
*/
void DwTitleBar::setIcon(const DwUrl &icon)
{
    m_icon->setImage(icon);
}

//------------------------------------------------------------------------------
/**
*/
void DwTitleBar::checkResizePattern(DwResizeEvent::ResizePattern pattern)
{
    switch(pattern)
    {
    case DwResizeEvent::Restored:
        if(m_maximum) maximumButton()->setVisible(true);
        if(m_restore) restoreButton()->setVisible(false);
        break;
    case DwResizeEvent::Minimized:
        break;
    case DwResizeEvent::Maximized:
        if(m_maximum) maximumButton()->setVisible(false);
        if(m_restore) restoreButton()->setVisible(true);
        break;
    case DwResizeEvent::MaxShow:
        break;
    case DwResizeEvent::MaxHide:
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwTitleBar::notifyItemChange(Dw::GraphicsItemChange change, const DwVariant& newOne, const DwVariant& oldOne)
{
    if (change == Dw::ItemParentChange)
    {
        DwGraphicsItem *p = (DwGraphicsItem *)(newOne.toPtr());
        if (p)
        {
            DwFrameWindow *win = dwsafe_cast<DwFrameWindow>(p);
            if (win)
            {
                setWindow(win);
            }
        }
    }

    __super::notifyItemChange(change, newOne, oldOne);
}

//------------------------------------------------------------------------------
/**
*/
void DwTitleBar::setWindow(DwFrameWindow * window)
{
    if (m_window == window)
    {
        return;
    }

    if (m_window)
    {
        DW_SAFE_DELETE(m_buttons);
        m_window->setTitleBar(NULL);
        m_window->eventHandler()->removeEventCallBack(DwResizeEvent::EventId(), onResizeEvt);
    }

    if (window)
    {
        DW_SAFE_DELETE(m_buttons);

        m_buttons = new DwRowPositioner(this);
        m_buttons->setName(L"buttonList");
        m_buttons->anchors()->setTopMargin(1);
        m_buttons->anchors()->setRight(DwAnchorLine(this, Dw::Right));
        m_buttons->anchors()->setTop(DwAnchorLine(this, Dw::Top));

        // accessible
        m_buttons->setAccessible(true);
        m_buttons->setAccessibleName(tr("title bar button List"));

#define _apply_accessible_from_tips(w) w->setAccessibleName(w->toolTips());
        if (window->isAttribute(Dw::WA_ResizeAble))
        {
            m_minimum = new DwPushButton(m_buttons);
            m_minimum->setName(L"minimum");
            m_minimum->setToolTips(tr("minimum"));
            m_minimum->eventHandler()->addEventCallBack(DwMouseClickedEvent::EventId(), onButtonClicked);
            m_minimum->setBackground(KMinNormalImage);

            m_restore = new DwPushButton(m_buttons);
            m_restore->setName(L"restore");
            m_restore->setToolTips(tr("restore"));
            m_restore->eventHandler()->addEventCallBack(DwMouseClickedEvent::EventId(), onButtonClicked);
            m_restore->setBackground(KRestoreNormalImage);
            m_restore->setVisible(false);

            m_maximum = new DwPushButton(m_buttons);
            m_maximum->setName(L"maximum");
            m_maximum->setToolTips(tr("maximum"));
            m_maximum->eventHandler()->addEventCallBack(DwMouseClickedEvent::EventId(), onButtonClicked);
            m_maximum->setBackground(KMaxNormalImage);

            // accessible
            _apply_accessible_from_tips(m_minimum);
            _apply_accessible_from_tips(m_restore);
            _apply_accessible_from_tips(m_maximum);
        }

        m_closed = new DwPushButton(m_buttons);
        m_closed->setName(L"closed");
        m_closed->setToolTips(tr("closed"));
        m_closed->eventHandler()->addEventCallBack(DwMouseClickedEvent::EventId(), onButtonClicked);
        m_closed->setBackground(KClosedNormalImage);
        
        _apply_accessible_from_tips(m_closed);

        setCaption(window->caption());
        anchors()->setLeft(DwAnchorLine(window, Dw::Left));
        anchors()->setRight(DwAnchorLine(window, Dw::Right));
        anchors()->setTop(DwAnchorLine(window, Dw::Top));
        window->setTitleBar(this);
        window->eventHandler()->addEventCallBack(DwResizeEvent::EventId(), onResizeEvt);
    }

    m_window = window;
}