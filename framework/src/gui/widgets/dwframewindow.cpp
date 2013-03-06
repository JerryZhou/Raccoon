#include "stable.h"
#include "dwframewindow.h"
#include "graphicsview/dwdummys.h"
#include "dwgui/dwgraphicsview.h"
#include "dwgui/dwgraphicsscene.h"
#include "dwgui/dwwindow.h"
#include "dwgui/dwtimer.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwdefaultsceneeventhandler.h"
#include "dwgui/dwdefaultvieweventhandler.h"
#include "dwgui/dwanchors.h"
#include "dwgui/dwgraphicssimpletextitem.h"
#include "dwgui/dwgraphicsframeitem.h"
#include "dwgui/dwwidgetitem.h"
#include "dwgui/dwwidgeteventhandler.h"
#include "dwgui/dwpushbutton.h"
#include "dwgui/dwcursor.h"
#include "dwgui/dwwidgetevent.h"
#include "dwgui/dweventsignaldispatch.h"
#include "dwgui/dwgraphicsmarginspixmapitem.h"
#include "dwgui/dwgraphicsbackgrounditem.h"
#include "dwframewindow_p.h"

//------------------------------------------------------------------------------
const char KDialogMask[] = "dwgui:mask_dialog.png";
const char KPopupMask[] = "dwgui:mask_popup.png";
const char KMainframeMask[]  = "dwgui:mask_mainframe.png";
const char KBorderMask[] = "dwgui:shadow.png";

//------------------------------------------------------------------------------
/**
*/
DwFrameWindow::DwFrameWindow(DwObject *parent)
    : DwObject(parent)
    , m_view(NULL)
    , m_scene(NULL)
    , m_titleBar(NULL)
    , m_icon(0)
    , m_border(NULL)
    , m_rect(0, 0, 640, 480)
    , m_isCenter(true)
    , m_roundEliptic(6)
    , m_shadowEdage(5)
{
    memset((void*)m_attrbutes, 0, sizeof(m_attrbutes));

    setAttribute(Dw::WA_ResizeAble);
    setBoundingRect(m_rect);
    setDragPolicy(DwWidget::DragWindow);
    setName(L"frameWindow");
    setMovable(true);
    setHoverable(false);

    // accessible
    setAccessibleName(tr("raccoon window"));
    setAccessibleDescription(tr("raccoon frame window"));
    setWhatsThis(tr("raccoon engine's window"));

    m_scene = new DwGraphicsScene(this);
    m_scene->setEventHandler(new DwDefaultSceneEventHandler(m_scene));
    m_scene->clearTopItemsWhenDestruct(true);// ONLY CAN ADD ITEM IN HEAP

    m_view = new DwGraphicsView(m_scene, this, NULL);
    m_view->setEventHandler(new DwDefaultViewEventHandler(m_view));
    m_view->eventHandler()->addEventCallBack(DwResizeEvent::EventId(), onResizeEvt);
    m_view->eventHandler()->addEventCallBack(DwMoveEvent::EventId(), onMoveEvt);
    m_view->eventHandler()->addEventCallBack(DwNcHitTestEvent::EventId(), onNcHitTest);
    m_view->eventHandler()->addEventCallBack(DwWindowActiveEvent::EventId(), onActiveEvt);
#if DW_ENABLE_EVENT_TIPS
    m_view->eventHandler()->addEventCallBack(DwShowToolTipEvent::EventId(), onShowToolTip);
    m_view->eventHandler()->addEventCallBack(DwHideToolTipEvent::EventId(), onHideToolTip);
#endif
    m_view->eventHandler()->addEventCallBack(DwWinOnShowEvent::EventId(), onWinShow);
    m_view->eventHandler()->addEventCallBack(DwMouseNcToClientEvent::EventId(), onNc2Client);
    m_view->eventHandler()->addEventCallBack(DwKeyUpEvent::EventId(), onKeyUp);

    m_fullContentLayer = new DwGraphicsItem(NULL);
    m_fullContentLayer->setName("_fullcontentlayer");
    m_fullContentLayer->setRect(m_rect);

    m_contentContainerLayer = new DwGraphicsItem(m_fullContentLayer);
    m_contentContainerLayer->setName("contentcontainerlayer");
    m_contentContainerLayer->anchors()->setMargins(m_shadowEdage);
    m_contentContainerLayer->anchors()->setFill(m_fullContentLayer);

    // background
    m_background->clearAnchors();
    m_background->setParentItem(m_contentContainerLayer);
    m_background->setZValue(1);

    // content
    // THIS
    this->setName("window");
    this->clearAnchors();
    this->setParentItem(m_contentContainerLayer);
    this->setZValue(2);
    this->anchors()->setFill(m_contentContainerLayer);

    // popup layer
    m_popupLayer = new DwGraphicsItem(m_contentContainerLayer);
    m_popupLayer->setName("popupLayer");
    m_popupLayer->anchors()->setFill(m_contentContainerLayer);
    m_popupLayer->setZValue(3);

    // border of frame
    m_border = new DwGraphicsNinePatchItem(m_fullContentLayer);
    m_border->setZValue(1);
    m_border->setName(L"border");
    m_border->anchors()->setFill(m_fullContentLayer);
    m_border->setPixmap(KBorderMask);
    m_border->setDrawCenter(false);

    setCaption(tr("caption"));
    setWinIcon(101);

    setBackgroundColor(DwColor(215, 222, 215, 100));
    //setBackgroundImage(KDialogImage);
#if DW_ENABLE_EVENT_DRAGDROP
    eventHandler()->addEventCallBack(DwDragMoveEvent::EventId(), onDragMoveEvt);
#endif

    _dw_frameWindowLib()->registerFrame(this);

    m_scene->addItem(m_fullContentLayer);
    m_scene->clearTopItemsWhenDestruct(true);
    m_view->setIndicator(Dw::WindowItem, this);
}

//------------------------------------------------------------------------------
/**
*/
DwFrameWindow::~DwFrameWindow()
{
#if DW_ENABLE_EVENT_TIPS
    dwClearToolTips();
#endif

    m_parentWindow = NULL;

    // clear scene
    DwGraphicsItem * item = this;
    if (item->scene() == m_scene)
    {
        m_scene->removeItem(this);
    }

    _dw_frameWindowLib()->unRegisterFrame(this);
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::show(Dw::ShowCommand nCmd/* = Dw::SW_Normal*/)
{
    bool needConnected = !(m_view->window());

    m_view->show(nCmd, m_rect.toRect());

    if (needConnected && m_view->window())
    {
        m_view->window()->sigDestroyed.connect(this, &DwFrameWindow::onWindowDestroyed);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::hide()
{
    show(Dw::SW_Hide);
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::close()
{
    setVisible(false);

    if (m_view->window())
    {
        m_view->window()->sigDestroyed.disconnect(this);
        m_view->window()->sigDestroyed.connect(this, &DwFrameWindow::onWindowDestroyed);
        m_view->window()->close();
    }
    else
    {
        onWindowDestroyed();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::update()
{
    if (m_view->window())
    {
        m_view->window()->update();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setBackgroundColor(const DwColor &c)
{
    m_background->setColor(c);
}

//------------------------------------------------------------------------------
/**
*/
DwColor DwFrameWindow::backgroundColor() const
{
    return m_background->color();
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setBackgroundImage(const DwUrl &uri)
{
    // try to accept the async event
    DW_PIXMAP_ASYNC(uri, this);
    // set the url to the background
    m_background->setURL(uri);
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setCaption(const DwString& s)
{
    m_caption = s;
    if (m_view->window())
    {
        m_view->window()->setCaption(s);
    }
}


//------------------------------------------------------------------------------
/**
*/
DwString DwFrameWindow::caption()const
{
    return m_caption;
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setWinIcon(int s)
{
    m_icon = s;

    if (m_view->window())
    {
        m_view->window()->setIcon(s);
    }
}

//------------------------------------------------------------------------------
/**
*/
int DwFrameWindow::icon() const
{
    return m_icon;
}

//------------------------------------------------------------------------------
/**
*/
bool DwFrameWindow::isEnabled() const
{
    if (m_view->window())
    {
        return m_view->window()->isWindowEnabled();
    }

    return __super::isEnabled();
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setEnabled(bool enabled)
{
    if (m_view->window())
    {
        m_view->window()->enableWindow(enabled);
        return;
    }

    __super::setEnabled(enabled);
    return;
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setParentWindow(DwFrameWindow *win)
{
    if (m_parentWindow == win)
    {
        return;
    }

    if (!m_view->window())
    {
        m_parentWindow = win;
        return;
    }

    checkParentWindow(win);

    m_parentWindow = win;
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::checkParentWindow(DwFrameWindow *win)
{
    if (m_parentWindow && !win)
    {
        m_view->window()->setParentWindow(NULL);
    }
    else if (win)
    {
        m_view->window()->setParentWindow(win->view()->window());
    }
}

//------------------------------------------------------------------------------
/**
*/
DwFrameWindow *DwFrameWindow::parentWindow() const
{
    return m_parentWindow;
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setAttribute(Dw::WidgetAttribute attribute, bool b/* = true*/)
{
    int n = attribute / 32;
    int m = attribute % 32;

    if (b)
    {
        m_attrbutes[n] |= (1 << m);
    }
    else
    {
        m_attrbutes[n] &= ~(1 << m);
    }

}

//------------------------------------------------------------------------------
/**
*/
bool DwFrameWindow::isAttribute(Dw::WidgetAttribute attribute)
{
    int n = attribute / 32;
    int m = attribute % 32;

    return !!(m_attrbutes[n] & (1 << m));
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::onWindowDestroyed()
{
    DwWidgetWindowEvent evt(DwWidgetWindowEvent::Close);
    m_view->sendEvent(this, &evt);

    if (m_view->window())
    {
        m_view->window()->sigDestroyed.disconnect_all();
    }

    if (isAttribute(Dw::WA_DeleteOnClose))
    {
        deleteLater();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setTitleBar(DwTitleBar *bar)
{
    m_titleBar = bar;
}

//------------------------------------------------------------------------------
/**
*/
DwPointF DwFrameWindow::pos()const
{
    return m_rect.topLeft();
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setPos(const DwPointF &p)
{
    m_rect.moveTopLeft(p);

    if (m_view && m_view->window())
    {
        setRect(DwRectF(p, m_rect.size()));
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setPos(DwReal x, DwReal y)
{
    setPos(DwPointF(x, y));
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwFrameWindow::x() const
{
    return pos().x();
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setX(DwReal x)
{
    setPos(x, y());
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwFrameWindow::y() const
{
    return pos().y();
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setY(DwReal y)
{
    setPos(x(), y);
}

//------------------------------------------------------------------------------
/**
*/
DwSizeF DwFrameWindow::size() const
{
    return m_rect.size();
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setSize(const DwSizeF &s)
{
    setRect(DwRectF(pos(), s));
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setSize(DwReal w, DwReal h)
{
    setSize(DwSizeF(w, h));
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwFrameWindow::width() const
{
    return size().width();
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setWidth(DwReal w)
{
    setSize(w, height());
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwFrameWindow::height() const
{
    return size().height();
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setHeight(DwReal h)
{
    setSize(width(), h);
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwFrameWindow::rect() const
{
    return m_rect;
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setRect(const DwRectF &r)
{
    m_rect = r;

    if (m_view && m_view->window())
    {
        m_view->window()->setRect(r.toRect());
    }
    else
    {
        m_fullContentLayer->setRect(r);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setRect(DwReal x, DwReal y, DwReal w, DwReal h)
{
    setRect(DwRectF(x, y, w, h));
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::checkRect(const DwRectF &r)
{
    m_rect = r;
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwFrameWindow::opacity() const
{
    if (m_fullContentLayer)
    {
        return m_fullContentLayer->opacity();
    }

    return DwGraphicsItem::opacity();
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setOpacity(DwReal s)
{
    if (m_fullContentLayer)
    {
        m_fullContentLayer->setOpacity(s);
    }
    else
    {
        DwGraphicsItem::setOpacity(s);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setCenterWindowOnShow(bool center)
{
    m_isCenter = center;
}

//------------------------------------------------------------------------------
/**
*/
bool DwFrameWindow::isCenterWindowOnShow() const
{
    return m_isCenter;
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::centerWindow() const
{
    if (m_view && m_view->window())
    {
        return m_view->window()->centerWindow();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setWinStyle(int style)
{
    m_winStyle = style;
}

//------------------------------------------------------------------------------
/**
*/
int DwFrameWindow::winStyle()
{
    return m_winStyle;
}

//------------------------------------------------------------------------------
/**
*/
int DwFrameWindow::roundEliptic() const
{
    return m_roundEliptic;
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setRoundEliptic(int n)
{
    m_roundEliptic = n;
}

//------------------------------------------------------------------------------
/**
*/
int DwFrameWindow::shadowEdage() const
{
    return m_shadowEdage;
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::setShadowEdage(int n)
{
    m_shadowEdage = n;
}

//------------------------------------------------------------------------------
/**
    0 is a invalid winId
*/
WinId DwFrameWindow::winId() const
{
    if (m_view && m_view->window())
    {
        return m_view->window()->winId();
    }
    return 0;
}

const DwString DwFrameWindow::fullId() const
{
    return "w" + DwString::number((int)this, 16);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwFrameWindow::backgrondLayer() const
{
    return m_background;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwFrameWindow::contentLayer() const
{
    return (DwGraphicsItem *)(this);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwFrameWindow::popupLayer() const
{
    return m_popupLayer;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwFrameWindow::fullContentLayer() const
{
    return m_fullContentLayer;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwFrameWindow::contentContainerLayer() const
{
    return m_contentContainerLayer;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwFrameWindow::borderLayer() const
{
    return m_border;
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::showPopup()
{

}

//------------------------------------------------------------------------------
/**
*/
DwFrameWindow* windowFromPoint(const DwPoint &p)
{
    DwGraphicsView* view = DwGraphicsView::viewFromPoint(p);
    if (view)
    {
        return dwsafe_cast<DwFrameWindow>(view->indicator(Dw::WindowItem));
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwFrameWindow* windowFromPoint(int x, int y)
{
    DwGraphicsView* view = DwGraphicsView::viewFromPoint(x, y);
    if (view)
    {
        return dwsafe_cast<DwFrameWindow>(view->indicator(Dw::WindowItem));
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwFrameWindow* windowFromWinId(int winId)
{
    DwGraphicsView* view = DwGraphicsView::viewFromWinId(winId);
    if (view)
    {
        return dwsafe_cast<DwFrameWindow>(view->indicator(Dw::WindowItem));
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindow::debugString(DwDebug &stream)const
{
    stream  <<" " << this->rtti()->name() << "["
            <<" caption :" << caption()
            << " icon :" << m_icon
            << " scene: " << (int)m_scene
            << " view: " << (int)m_view
            << " rect:" << m_rect
            << " have real window: " << (m_view->window() ? true : false)
            << "]";
}