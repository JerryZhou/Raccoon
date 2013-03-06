#pragma once
#include "dwcore/dwobject.h"
#include "dwcore/dwstring.h"
#include "dwgraphics/dwcolor.h"
#include "dwcore/sigslot.h"
#include "dwgui/dwwidgetitem.h"
#include "dwgui/dwurl.h"

//------------------------------------------------------------------------------
class DwGraphicsView;
class DwGraphicsScene;
class DwWidgetItem;
class DwTitleBar;
class DwImageWidget;
class DwGraphicsFrameItem;
class DwGraphicsMarginsPixmapItem;
class DwGraphicsNinePatchItem;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwFrameWindow : public DwWidgetItem
    , public DwObject, public sigslot::has_slots
{
    DW_DECLARE_CLASS(DwFrameWindow);
public:
    DwFrameWindow(DwObject *parent = NULL);
    virtual ~DwFrameWindow();

    void show(Dw::ShowCommand nCmd = Dw::SW_ShowNormal);
    void hide();
    void close();
    void update();

    inline DwGraphicsView* view() const;
    inline DwGraphicsScene* scene() const;
    inline DwGraphicsNinePatchItem *border() const;

    void setBackgroundColor(const DwColor &);
    DwColor backgroundColor() const;

    void setBackgroundImage(const DwUrl&);

    void setCaption(const DwString&);
    DwString caption()const;

    int icon() const;
    void setWinIcon(int);

    bool isEnabled() const ;
    void setEnabled(bool enabled) ;

    void setParentWindow(DwFrameWindow *win);
    void checkParentWindow(DwFrameWindow *win);
    DwFrameWindow *parentWindow() const;

    inline DwTitleBar* titleBar() const;

    void setAttribute(Dw::WidgetAttribute attribute, bool b = true);
    bool isAttribute(Dw::WidgetAttribute attribute);

    // the window pos
    DwPointF pos()const;
    void setPos(const DwPointF &p);
    void setPos(DwReal x, DwReal y);
    DwReal x() const;
    void setX(DwReal x);
    DwReal y() const;
    void setY(DwReal y);

    // the window size
    DwSizeF size() const;
    void setSize(const DwSizeF &s);
    void setSize(DwReal w, DwReal h);
    DwReal width() const;
    void setWidth(DwReal w);
    DwReal height() const;
    void setHeight(DwReal h);

    // the window rect (screen coordinate)
    DwRectF rect() const;
    void setRect(const DwRectF &r);
    void setRect(DwReal x, DwReal y, DwReal w, DwReal h);
    void checkRect(const DwRectF &r);

    // the window opacity
    DwReal opacity() const;
    void setOpacity(DwReal s);

    // show stuffs
    void setCenterWindowOnShow(bool center);
    bool isCenterWindowOnShow() const;
    void centerWindow() const;

    void setWinStyle(int style);
    int winStyle();

    // clip rgn, only worked before window show
    int roundEliptic() const;
    void setRoundEliptic(int n);

    int shadowEdage() const;
    void setShadowEdage(int n);

    // win id( Hwnd)
    WinId winId() const;

    virtual const DwString fullId() const;

    // debug stuffs
    virtual void debugString(DwDebug &stream)const ;

    // layerd the window
    DwGraphicsItem *backgrondLayer() const;
    DwGraphicsItem *contentLayer() const;
    DwGraphicsItem *popupLayer() const;

    // Used to layer shaow
    DwGraphicsItem *fullContentLayer() const;
    DwGraphicsItem *contentContainerLayer() const;
    DwGraphicsItem *borderLayer() const;

    // popup
    void showPopup();

    // Look up view
    static DwFrameWindow* windowFromPoint(const DwPoint &p);
    static DwFrameWindow* windowFromPoint(int x, int y);
    static DwFrameWindow* windowFromWinId(int winId);

protected:
    friend class DwTitleBar;

    void onWindowDestroyed();
    void setTitleBar(DwTitleBar *bar);

    DwGraphicsView* m_view;
    DwGraphicsScene* m_scene;
    DwTitleBar* m_titleBar;
    DwGraphicsNinePatchItem* m_border;
    DwGraphicsItem *m_popupLayer;

    DwGraphicsItem *m_fullContentLayer;
    DwGraphicsItem *m_contentContainerLayer;

    DwString m_caption;
    int m_icon;
    DwRectF m_rect;
    bool m_isCenter;
    int m_winStyle;
    int m_roundEliptic;
    int m_shadowEdage;
    DwRttiObjectPtr<DwFrameWindow> m_parentWindow;

    // a binary array
    uint32_t m_attrbutes[(Dw::WA_AttributeCount / 32 +  (Dw::WA_AttributeCount % 32 != 0 ? 1 : 0))];

    DW_DECLARE_SUB_PROPERTY(DwFrameWindow, DwWidgetItem);
    DW_WIDGET_PROPERTY_BEGIN(DwFrameWindow, DwWidgetItem);
    DW_WIDGET_PROPERTY(pos, DwPointF, setPos, pos);
    DW_WIDGET_PROPERTY_NC(x, DwReal, setX, x);
    DW_WIDGET_PROPERTY_NC(y, DwReal, setY, y);
    DW_WIDGET_PROPERTY(size, DwSizeF, setSize, size);
    DW_WIDGET_PROPERTY_NC(width, DwReal, setWidth, width);
    DW_WIDGET_PROPERTY_NC(height, DwReal, setHeight, height);
    DW_WIDGET_PROPERTY(rect, DwRectF, setRect, rect);
    DW_WIDGET_PROPERTY_NC(opacity, DwReal, setOpacity, opacity);
    DW_WIDGET_PROPERTY_END();
};// end of DwFrameWindow
DW_REGISTER_CLASS(DwFrameWindow);

//------------------------------------------------------------------------------
/**
*/
inline DwGraphicsView* DwFrameWindow::view() const
{
    return m_view;
}

//------------------------------------------------------------------------------
/**
*/
inline DwGraphicsScene* DwFrameWindow::scene() const
{
    return m_scene;
}

//------------------------------------------------------------------------------
/**
*/
inline DwGraphicsNinePatchItem *DwFrameWindow::border() const
{
    return m_border;
}

//------------------------------------------------------------------------------
/**
*/
inline DwTitleBar* DwFrameWindow::titleBar() const
{
    return m_titleBar;
}