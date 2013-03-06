#pragma once

#include <dwcore/sigslot.h>

#include <dwcore/dwobject.h>
#include <dwcore/dwsize.h>
#include <dwcore/dwrect.h>
#include <dwgraphics/dwimage.h>
#include <dwgraphics/dwpainter.h>
#include <dwgui/dwrttiobject.h>

//------------------------------------------------------------------------------
class DwWindowImpl;
class DwGraphicsView;
class DwPainter;
class DwWindowAdapter;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwWindow : public DwRttiObject, public DwObject, public sigslot::has_slots
{
    DW_DECLARE_CLASS(DwWindow);
public:
    DwWindow();
    virtual ~DwWindow();

    void show(int nCmdShow = Dw::SW_ShowNormal, const DwRect& r = DwRect(0, 0, 640, 480));
    DwSize size() const;

    DwString winid() const; // ### deprecated

    DwGraphicsView* view() const;

    int width() const;
    int height() const;

    void update();

    void close();
    void move(int x, int y);

    void setCaption(const DwString&);
    DwString caption() const;
    void setIcon(int icon);
    void setSize(const DwSize &s);
    void setRect(const DwRect &r);
    WinId winId() const;
    void centerWindow(WinId windId = 0);
    bool isWindowEnabled() const;
    void enableWindow(bool b);
    void setParentWindow(DwWindow *win);
    void setActive();
    void setFocus();

    virtual void paintEvent(DwPainter &p, const DwRectF &rect);

    DwWindowAdapter* adapter()const;

    static DwWindow* windowFromPoint(const DwPoint &p);
    static DwWindow* windowFromPoint(int x, int y);
    static DwWindow* windowFromWinId(WinId id);

public:
    sigslot::signal0<> sigCreated;
    sigslot::signal0<> sigClosed;
    sigslot::signal0<> sigDestroyed;

protected:
    void setView(DwGraphicsView* view);

private:
    void on_window_created();
    void on_window_closed();
    void on_window_destroyed();

private:
    friend class DwObject;
    friend class DwWindowImpl;
    friend class DwGraphicsView;

    DwWindowImpl *m_window;
};// end of DwWindow