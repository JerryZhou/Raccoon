#pragma once
#include "dwgui/dwwidgetitem.h"

//------------------------------------------------------------------------------
class DwLabel;
class DwRowPositioner; // TODO: to use list control
class DwFrameWindow;
class DwImageWidget;
class DwPushButton;
class DwGraphicsFrameItem;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwTitleBar : public DwWidgetItem
{
    DW_DECLARE_CLASS(DwTitleBar);
public:
    DwTitleBar(DwFrameWindow *win = NULL);
    virtual ~DwTitleBar();

    // Stand Button
    inline DwRowPositioner *buttons() const;
    inline DwPushButton *closedButton() const;
    inline DwPushButton *maximumButton() const;
    inline DwPushButton *minimumButton() const;
    inline DwPushButton *restoreButton() const;

    // caption
    inline DwLabel *caption() const;
    void setCaption(const DwString&);

    // icon
    inline DwImageWidget *icon() const;
    void setIcon(const DwUrl&);

    // window
    inline DwFrameWindow *window() const;

    // according to pattern
    void checkResizePattern(DwResizeEvent::ResizePattern pattern);

    virtual void notifyItemChange(Dw::GraphicsItemChange change, const DwVariant& newOne, const DwVariant& oldOne);

protected:
    void setWindow(DwFrameWindow * win);

    DwLabel *m_caption;
    DwImageWidget *m_icon;

    DwRowPositioner *m_buttons;
    DwPushButton *m_closed;
    DwPushButton *m_maximum;
    DwPushButton *m_minimum;
    DwPushButton *m_restore;

    DwFrameWindow *m_window;
};// end of DwTitleBar
DW_REGISTER_CLASS(DwTitleBar);

//------------------------------------------------------------------------------
/**
*/
inline DwRowPositioner *DwTitleBar::buttons() const
{
    return m_buttons;
}

//------------------------------------------------------------------------------
/**
*/
inline DwPushButton *DwTitleBar::closedButton() const
{
    return m_closed;
}

//------------------------------------------------------------------------------
/**
*/
inline DwPushButton *DwTitleBar::maximumButton() const
{
    return m_maximum;
}

//------------------------------------------------------------------------------
/**
*/
inline DwPushButton *DwTitleBar::minimumButton() const
{
    return m_minimum;
}

//------------------------------------------------------------------------------
/**
*/
inline DwPushButton *DwTitleBar::restoreButton() const
{
    return m_restore;
}

//------------------------------------------------------------------------------
/**
*/
inline DwLabel *DwTitleBar::caption() const
{
    return m_caption;
}

//------------------------------------------------------------------------------
/**
*/
inline DwImageWidget *DwTitleBar::icon() const
{
    return m_icon;
}

//------------------------------------------------------------------------------
/**
*/
inline DwFrameWindow *DwTitleBar::window() const
{
    return m_window;
}