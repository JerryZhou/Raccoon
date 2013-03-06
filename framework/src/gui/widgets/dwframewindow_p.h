#pragma once
#include "dwgui/dwpropertyanimation.h"

static int winStyles[Dw::WinStyleMax] =
{
    0,
    WS_EX_TOOLWINDOW,
    0,
};

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwFrameWindow, 'FWOW', DwWidgetItem);
DW_IMPLEMENT_PROPERTY(DwFrameWindow);

//------------------------------------------------------------------------------
/**
*/
static void onWinShow(DwGraphicsView* view, DwEvent *evt)
{
    DW_UNUSED(view);
    DW_ASSERT(evt->eventId() == DwWinOnShowEvent::EventId());
    DwWinOnShowEvent *onShowEvt = dwsafe_cast<DwWinOnShowEvent>(evt);
    DW_CHECK_ASSERT(onShowEvt && onShowEvt->m_window);

    // content fill the window
    DwFrameWindow* frameWindow = dwsafe_cast<DwFrameWindow>(view->indicator(Dw::WindowItem));

    if (onShowEvt->m_showCommand == Dw::SW_Hide || !frameWindow)
    {
        return;
    }

    // apply the window setting to real window
    onShowEvt->m_window->setCaption(frameWindow->caption());
    onShowEvt->m_window->setIcon(frameWindow->icon());
    onShowEvt->m_window->setRect(frameWindow->rect().toRect());
    if (frameWindow->parentWindow())
    {
        onShowEvt->m_window->setParentWindow(frameWindow->parentWindow()->view()->window());
    }

    // center window on desktop
    if (frameWindow->isCenterWindowOnShow())
    {
        onShowEvt->m_window->centerWindow();
    }

    // set a toolwindow
    if (frameWindow->winStyle() == Dw::ToolWindow ||
            frameWindow->winStyle() == Dw::PopupWindow)
    {
        DWORD dwExStyle = ::GetWindowLong((HWND)onShowEvt->m_winId, GWL_EXSTYLE);
        dwExStyle |= WS_EX_TOOLWINDOW;
        dwExStyle |= WS_EX_TOPMOST;
        //dwExStyle |= WS_EX_NOACTIVATE;
        ::SetWindowLong((HWND)onShowEvt->m_winId, GWL_EXSTYLE, dwExStyle & ~WS_EX_APPWINDOW);

        // {{ commect
        // as we need active event to imp popup
        //DWORD dwStyle = ::GetWindowLong((HWND)onShowEvt->m_winId, GWL_STYLE);
        //dwStyle |= WS_POPUP;
        //dwStyle |= TTS_NOPREFIX | TTS_ALWAYSTIP;
        //::SetWindowLong((HWND)onShowEvt->m_winId, GWL_STYLE, dwStyle);
        // }}end commect
    }

    // popup should be active
    if (frameWindow->winStyle() == Dw::PopupWindow)
    {
        ::SetWindowPos((HWND)onShowEvt->m_winId, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        ::SetForegroundWindow((HWND)onShowEvt->m_winId);
    }

    if (!frameWindow->isAttribute(Dw::WA_ResizeAble))
    {
        DWORD dwStyle = ::GetWindowLong((HWND)onShowEvt->m_winId, GWL_STYLE);
        dwStyle &= ~WS_MAXIMIZEBOX;
        dwStyle &= ~WS_THICKFRAME;
        ::SetWindowLong((HWND)onShowEvt->m_winId, GWL_STYLE, dwStyle);
    }

    // hijack to window
    onShowEvt->setInterestItem(frameWindow);
}

//------------------------------------------------------------------------------
/**
*/
static void onResizeEvt(DwGraphicsView* view, DwEvent *evt)
{
    DW_ASSERT(evt->eventId() == DwResizeEvent::EventId());
    DwResizeEvent * resizeEvt = (DwResizeEvent*)(evt);

    // content fill the window
    DwFrameWindow* frameWindow = dwsafe_cast<DwFrameWindow>(view->indicator(Dw::WindowItem));
    DwGraphicsItem *fullContentLayer = (DwGraphicsItem *)frameWindow->fullContentLayer();
    DwGraphicsItem *contentContainerLayer = (DwGraphicsItem*)frameWindow->contentContainerLayer();
    DwGraphicsItem *borderLayer = (DwGraphicsItem*)frameWindow->borderLayer();

    fullContentLayer->setRect(DwRectF(DwPointF(0, 0), resizeEvt->m_newSize));
    frameWindow->checkRect(resizeEvt->m_clientRect);

    if (resizeEvt->m_pattern == DwResizeEvent::Maximized || resizeEvt->m_pattern == DwResizeEvent::MaxShow)
    {
        contentContainerLayer->anchors()->setMargins(0);
        contentContainerLayer->setFlag(Dw::ItemClipsChildrenToShape, false);
        borderLayer->setVisible(false);
    }
    else
    {

        // shadow
        contentContainerLayer->anchors()->setMargins(frameWindow->shadowEdage());
        // rounded
        int eliptic = frameWindow->roundEliptic();
        if (eliptic)
        {
            contentContainerLayer->setFlag(Dw::ItemClipsChildrenToShape, true);
            contentContainerLayer->setClipRegion(&DwRegion(contentContainerLayer->boundingRect().toRect(), eliptic, eliptic));
            borderLayer->setVisible(true);
        }
        else
        {
            contentContainerLayer->setFlag(Dw::ItemClipsChildrenToShape, false);
            borderLayer->setVisible(true);
        }
    }

    // hijack to window
    resizeEvt->setInterestItem(frameWindow);
    // do not accept this event ,
    // as in view handler will set the viewport and sceneport
}

//------------------------------------------------------------------------------
/**
*/
static void onActiveEvt(DwGraphicsView *view, DwEvent *evt)
{
    DwFrameWindow* frameWindow = dwsafe_cast<DwFrameWindow>(view->indicator(Dw::WindowItem));
    if (frameWindow && frameWindow->winStyle() == Dw::PopupWindow 
        && evt->isA(DwWindowActiveEvent::EventId()))
    {
        DwWindowActiveEvent* activeEvt = dwsafe_cast<DwWindowActiveEvent>(evt);
        if (!activeEvt->m_actived && !(frameWindow->reserved()&0x06))
        {
            frameWindow->setReserved(frameWindow->reserved()|0x06); // used to keep re entry away from
            frameWindow->close();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
static void onMoveEvt(DwGraphicsView *view, DwEvent *evt)
{
    DW_ASSERT(evt->eventId() == DwMoveEvent::EventId());
    DwMoveEvent * moveEvt = (DwMoveEvent*)(evt);

    DwFrameWindow* frameWindow = (DwFrameWindow*)(view->parent());
    if (moveEvt->m_clientRect.y() == 0)
    {
        int i;
        i = 3;
    }
    frameWindow->checkRect(moveEvt->m_clientRect);
    // hijack to window
    moveEvt->setInterestItem(frameWindow);
}

//------------------------------------------------------------------------------
/**
*/
static LRESULT _dw_onNcHitTest( DwFrameWindow *window, DwNcHitTestEvent *hitTestEvt,
                                const DwMargins &borderMargins, int captionHeight)
{
    DW_UNUSED(window);
    if(::IsZoomed(HWND(hitTestEvt->m_winId)))
    {
        return HTCLIENT;
    }
    const DwPoint &pt = hitTestEvt->m_devicePoint;

    int cx = hitTestEvt->m_clientRect.width();
    int cy = hitTestEvt->m_clientRect.height();

    DwRect rectCaption(borderMargins.left(), borderMargins.top(), cx - borderMargins.left() - borderMargins.right(), captionHeight);
    if(rectCaption.contains(pt))
    {
        return HTCAPTION;
    }

    if(borderMargins.left() > 0)
    {
        DwRect rectTopLeft(0, 0, borderMargins.left(),  borderMargins.top());
        if(rectTopLeft.contains(pt))
        {
            return HTTOPLEFT;
        }
        DwRect rectLeft(0, borderMargins.top(), borderMargins.left(), cy - borderMargins.top() - borderMargins.bottom());
        if(rectLeft.contains(pt))
        {
            return HTLEFT;
        }
    }

    if(borderMargins.right() > 0)
    {
        DwRect rectTopRight(cx - borderMargins.right(), 0, borderMargins.right(), borderMargins.top());
        if(rectTopRight.contains(pt))
        {
            return HTTOPRIGHT;
        }
        DwRect rectRight(cx - borderMargins.right(), borderMargins.top(), borderMargins.right(), cy - borderMargins.top() - borderMargins.bottom());
        if(rectRight.contains(pt))
        {
            return HTRIGHT;
        }
    }

    if(borderMargins.top() > 0)
    {
        DwRect rectTop(borderMargins.left(), 0, cx - borderMargins.left() - borderMargins.right(), borderMargins.top());
        if(rectTop.contains(pt))
        {
            return HTTOP;
        }
    }

    if(borderMargins.bottom() > 0)
    {
        DwRect rectBottomLeft(0, cy - borderMargins.bottom(), borderMargins.left(), borderMargins.bottom());
        if(rectBottomLeft.contains(pt))
        {
            return HTBOTTOMLEFT;
        }

        DwRect rectBottomRight(cx - borderMargins.right(), cy - borderMargins.bottom(), borderMargins.right(), borderMargins.bottom());

        //increace the bottom right corner
        rectBottomRight.setLeft( rectBottomRight.left() - 10 );
        rectBottomRight.setTop( rectBottomRight.top() - 10 );
        if(rectBottomRight.contains(pt))
        {
            return HTBOTTOMRIGHT;
        }

        DwRect rectBottom(borderMargins.left(), cy - borderMargins.bottom(), cx - borderMargins.left() - borderMargins.right(), borderMargins.bottom());
        if(rectBottom.contains(pt))
        {
            return HTBOTTOM;
        }
    }

    return HTCLIENT;
}

//------------------------------------------------------------------------------
/**
*/
static DwWidget::DragPolicy dragPolicyOf(DwGraphicsItem *item)
{
    DwWidgetItem *widgetItem = dwsafe_cast<DwWidgetItem>(item);
    if (widgetItem)
    {
        DwWidget::DragPolicy policy = widgetItem->dragPolicy();
        if (policy == DwWidget::DragWindow || policy == DwWidget::NoDrag)
        {
            return policy;
        }
        else
        {
            return dragPolicyOf(widgetItem->parentItem());
        }
    }

    if (item && item->parentItem())
    {
        return dragPolicyOf(item->parentItem());
    }

    return DwWidget::NoDrag;
}

//------------------------------------------------------------------------------
/**
*/
static void onNcHitTest(DwGraphicsView *view, DwEvent *evt)
{
    DW_ASSERT(evt->eventId() == DwNcHitTestEvent::EventId());
    DwNcHitTestEvent *hitTestEvt = (DwNcHitTestEvent*)(evt);

    DwFrameWindow* frameWindow = (DwFrameWindow*)(view->parent());
    int border = 0;
    if (frameWindow->isAttribute(Dw::WA_ResizeAble))
    {
        border = 4;
    }
    hitTestEvt->m_result = _dw_onNcHitTest(frameWindow, hitTestEvt, DwMargins(border, border, border, border), 0);

    if (hitTestEvt->m_result == HTCLIENT && frameWindow->isMovable())
    {
        // at this, we can go on dispatch  evt to item. just do not accept
        DwGraphicsItem *item = hitTestEvt->indexInterestItemFromView(view);
        if (dragPolicyOf(item) == DwWidget::DragWindow)
        {
            hitTestEvt->m_result = HTCAPTION;
        }
    }
    // hijack to window
    hitTestEvt->setInterestItem(frameWindow);
}

//------------------------------------------------------------------------------
/**
*/
static void onNc2Client(DwGraphicsView *, DwEvent *evt)
{
    DW_UNUSED(evt);
    DW_ASSERT(evt->eventId() == DwMouseNcToClientEvent::EventId());
    DwCursor::setCursor(Dw::ArrowCursor);
}


//------------------------------------------------------------------------------
/**
*/
static void addFrameAnimation(DwGraphicsItem *item, DwReal start, DwReal end, int duration)
{
    DW_ASSERT(item);
    item->clearAnimations();

    DwPropertyAnimation *anims = new DwPropertyAnimation(NULL);
    anims->setTargetItem(item);
    anims->setProperty("opacity");
    anims->setStartValue(start);
    anims->setEndValue(end);
    anims->setEasingCurve(Dw::InOutQuad);
    anims->setDuration(duration);
    anims->start(Dw::DeleteWhenStopped);
}

#if DW_ENABLE_EVENT_TIPS
DwFrameWindow *_dw_toolTipWindow = NULL;
DwGraphicsSimpleTextItem *_dw_simpleText = NULL;
//------------------------------------------------------------------------------
/**
*/
static void onShowToolTip(DwGraphicsView* view, DwEvent* evt)
{
    DW_UNUSED(view);
    DwShowToolTipEvent* tipEvt = dwsafe_cast<DwShowToolTipEvent>(evt);
    DwFrameWindow* frameWindow = (DwFrameWindow*)view->parent();

    if (tipEvt->m_tips.isEmpty())
    {
        if (_dw_toolTipWindow)
        {
            _dw_toolTipWindow->close();
            _dw_toolTipWindow = NULL;
        }
        return;
    }

    if (frameWindow->isAttribute(Dw::WA_InnerToolTip))
    {
        if (!_dw_toolTipWindow)
        {
            _dw_toolTipWindow = new DwFrameWindow(NULL);
            _dw_toolTipWindow->setBackgroundImage(L"");
            _dw_toolTipWindow->setBackgroundColor(DwColor(249, 249, 251));
            _dw_toolTipWindow->setAttribute(Dw::WA_DeleteOnClose);
            _dw_toolTipWindow->setAttribute(Dw::WA_ResizeAble, false);

            //DwGraphicsItem * item = (DwGraphicsItem*)_dw_toolTipWindow->borderLayer();
            _dw_simpleText = new DwGraphicsSimpleTextItem(_dw_toolTipWindow);
            _dw_simpleText->anchors()->setCenterIn(_dw_toolTipWindow);
            _dw_simpleText->setFontSize(13);
        }

        //Example Code
        DwRectF bound = _dw_simpleText->font().MeasureString(tipEvt->m_tips);
        _dw_simpleText->setText(tipEvt->m_tips);
        _dw_simpleText->anchors()->setCenterIn(_dw_toolTipWindow);
        int shadowN = _dw_toolTipWindow->shadowEdage()*2;
        bound.adjust(0, 0, 4+shadowN, 2+shadowN);

        // content fill the window
        DwPointF curCursorPos = DwCursor::pos();
        curCursorPos.setY(curCursorPos.y() + 20);
        DwRectF tipRect = DwRectF(curCursorPos, bound.size());

        _dw_toolTipWindow->setCenterWindowOnShow(false);
        _dw_toolTipWindow->setRect(tipRect.toRect());
        _dw_toolTipWindow->setWinStyle(Dw::ToolWindow);
        addFrameAnimation((DwGraphicsItem*)(_dw_toolTipWindow->fullContentLayer()), 0.0, 1.0, 380);
        _dw_toolTipWindow->show(Dw::SW_ShowNoActivate);
        tipEvt->accept();
    }
    else
    {
        tipEvt->setInterestItem(frameWindow);
    }
}

//------------------------------------------------------------------------------
/**
*/
static void onHideToolTip(DwGraphicsView* view, DwEvent* evt)
{
    DW_UNUSED(view);
    DwHideToolTipEvent* tipEvt = dwsafe_cast<DwHideToolTipEvent>(evt);
    DwFrameWindow* frameWindow = (DwFrameWindow*)view->parent();

    if (_dw_toolTipWindow)
    {
        _dw_toolTipWindow->close();
        _dw_toolTipWindow = NULL;
    }

    if (frameWindow && frameWindow->isAttribute(Dw::WA_InnerToolTip))
    {
        tipEvt->accept();
    }
    else
    {
        tipEvt->setInterestItem(frameWindow);
    }
}

//------------------------------------------------------------------------------
/**
*/
static void onKeyUp(DwGraphicsView *view, DwEvent* evt)
{
    DwKeyUpEvent *keyEvt = dwsafe_cast<DwKeyUpEvent>(evt);
    if (keyEvt->keyCode() == 'B')
    {
        if (keyEvt->isAltDown() &&
            keyEvt->isShfitDown() &&
            keyEvt->isCtrlDown())
        {
            DwGraphicsScene* scene = view->scene();
            scene->setDebugDraw(!scene->debugDraw());
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
static void dwClearToolTips()
{
    if (_dw_toolTipWindow)
    {
        DwFrameWindow *win = _dw_toolTipWindow;
        _dw_toolTipWindow = NULL;
        win->close();
    }
}
#endif

//------------------------------------------------------------------------------
/**
    STOP DRAG WINDOW IN WIDGET LEVEL IF THE WINDOW IS THE REAL WINDOW
*/
static void onDragMoveEvt(DwGraphicsItem *item, DwEvent *evt)
{
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(item);
    if ( window->view()->window() )
    {
        evt->accept();
    }
}

//------------------------------------------------------------------------------
/**
*/
struct DwFrameWindowData
{
    bool isModal;
    DwFrameWindowData()
    {
        isModal = false;
    }
};

//------------------------------------------------------------------------------
/**
*/
struct DwFrameWindowLib : public sigslot::has_slots
{
    DwMap<DwFrameWindow*, DwFrameWindowData> m_frames;

    DwFrameWindowLib();
    virtual ~DwFrameWindowLib();

    void registerFrame(DwFrameWindow *win);
    void unRegisterFrame(DwFrameWindow *win);
};

//------------------------------------------------------------------------------
/**
*/
DwFrameWindowLib::DwFrameWindowLib()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwFrameWindowLib::~DwFrameWindowLib()
{
    DW_ASSERT(m_frames.count() == 0);
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindowLib::registerFrame(DwFrameWindow *win)
{
    m_frames[win] = DwFrameWindowData();
}

//------------------------------------------------------------------------------
/**
*/
void DwFrameWindowLib::unRegisterFrame(DwFrameWindow *win)
{
    m_frames.remove(win);
}

DW_GLOBAL_STATIC(DwFrameWindowLib, _dw_frameWindowLib);
