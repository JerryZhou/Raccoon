#pragma once
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwgraphicsitem.h"
#include "dwgui/dwurl.h"

//------------------------------------------------------------------------------
class DwPropertyAnimation;

//Disable Copy And Assign
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwTrayIcon : DwGraphicsItem
{
    DW_DECLARE_CLASS(DwTrayIcon);
public:
    enum TrayIconContent{ ENull, ETips = 0x01, EIcon = 0x02, EALL = 0xff};
    explicit DwTrayIcon(DwGraphicsItem *parent = NULL);
    virtual ~DwTrayIcon();

    // tips
    void setTips(const DwString& s);
    DwString tips() const;

    // icon
    void setIcon(const DwUrl& s, int idx = 0);
    void setIconIdx(int idx);
    const DwUrl& Icon() const;
    int IconIdx() const;

    WinId winId() const;

    // control the trayicon
    void show();
    void hide();
    int modify(TrayIconContent content);

    // anis
    void startFlash(int duration, bool loop);
    void stopFlash(int idx = 0);
    void startFlashAnis(int duration, bool loop, int start, int end);

    // rect
    DwRect trayIconRect() const;

    // event
    DwItemEventHandler* eventHandler() const;

    virtual bool onWinEvent(UINT uMsg, LPARAM lParam, WPARAM wParam, HRESULT *hr);

protected:
    DW_DECLARE_SUB_PROPERTY(DwTrayIcon, DwGraphicsItem);
    DW_WIDGET_PROPERTY_BEGIN(DwTrayIcon, DwGraphicsItem);
    DW_WIDGET_PROPERTY_NC(IconIdx, int, setIconIdx, IconIdx);
    DW_WIDGET_PROPERTY_END();

    void update();

    WinId m_winId;
    DwString m_tips;
    DwUrl m_icon;
    int m_iconIdx;
    bool m_visible;
    int m_contents;
    DwPropertyAnimation* m_anis;
};// end of DwTrayIcon