#pragma once
#include "dwgui/dwwidgetitem.h"

//------------------------------------------------------------------------------
class DwGraphicsMarginsPixmapItem;
class DwGraphicsNinePatchItem;
class DwPushButton;
class DwRowPositioner;
class DwGraphicsPixmapItem;
class DwGraphicsSimpleTextItem;
class DwRowPositioner;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwMessageBox : public DwWidgetItem
{
    DW_DECLARE_CLASS(DwMessageBox);
public:
    explicit DwMessageBox(DwGraphicsItem *parent = NULL);
    virtual ~DwMessageBox();

    void show();

    void setSize(const DwSizeF &s);
    void setCaption(const DwString &txt);
    void setHeadIcon(const DwUrl &url);
    void setIcon(const DwUrl &url);
    void setText(const DwString &txt);

    void setBackground(const DwUrl &url);
    DwGraphicsMarginsPixmapItem *background() const;

    void setButtonText(DwWidget::MessageBoxButton b, const DwString &txt);
    void setMode(DwWidget::MessageBoxMode m);
    DwPushButton *buttonOf(DwWidget::MessageBoxButton b) const;

    DwGraphicsSimpleTextItem *headText() const;
    DwGraphicsPixmapItem *headIcon() const;
    void setHeadIconTextSpacing(DwReal s);

    DwGraphicsSimpleTextItem *text() const;
    DwGraphicsPixmapItem *icon() const;
    DwRowPositioner *centerContent() const;
    void setIconTextSpacing(DwReal s);

protected:
    void setFlash(int i);
    int flash() const;

    DwGraphicsMarginsPixmapItem* m_background;
    DwWidgetItem *m_content;
    DwGraphicsNinePatchItem* m_border;

    DwGraphicsPixmapItem *m_headIcon;
    DwGraphicsSimpleTextItem *m_headText;

    DwRowPositioner *m_centerContent;
    DwGraphicsPixmapItem *m_icon;
    DwGraphicsSimpleTextItem *m_text;

    DwPushButton *m_closed;
    DwPushButton *m_ok;
    DwPushButton *m_cancle;

    int m_flash;
    DW_DECLARE_SUB_PROPERTY(DwMessageBox, DwWidgetItem);
    DW_WIDGET_PROPERTY_BEGIN(DwMessageBox, DwWidgetItem);
    DW_WIDGET_PROPERTY_NC(flash, int, setFlash, flash);
    DW_WIDGET_PROPERTY_END();
};// end of DwMessageBox
DW_REGISTER_CLASS(DwMessageBox);