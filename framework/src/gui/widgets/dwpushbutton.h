#pragma once
#include "dwgui/dwwidgetitem.h"

//------------------------------------------------------------------------------
class DwGraphicsSimpleTextItem;
class DwGraphicsNinePatchItem;
class DwFont;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwPushButton : public DwWidgetItem
{
    DW_DECLARE_CLASS(DwPushButton);
public:
    explicit DwPushButton(DwGraphicsItem *parent = NULL);
    virtual ~DwPushButton();

    void setBackground(const DwUrl& );
    void setIcon(const DwUrl&, int idx=0);
    void setText(const DwString& );
    void setFont(const DwFont &);

    inline DwGraphicsNinePatchItem* icon() const;
    inline DwGraphicsSimpleTextItem* text() const;

    void setHorizable(bool h);
    void setButtonMargins(DwReal iconLeft, DwReal textLeft, DwReal textRight);

    void setTextOnly();
    void setIconOnly();

    void adjustSize();

    DwPointF offset() const;
    void setOffset(const DwPointF &o);
    void setContentOffset(const DwPointF &o);

protected:
    DwGraphicsItem *m_content;
    DwGraphicsNinePatchItem *m_icon;
    DwGraphicsSimpleTextItem *m_text;
    bool m_horizable;
    DwPointF m_offset;
private:
    DW_WIDGET_PROPERTY_BEGIN(DwPushButton, DwWidgetItem);
    DW_WIDGET_PROPERTY_END();
};// end of DwPushButton
DW_REGISTER_CLASS(DwPushButton);

//------------------------------------------------------------------------------
/**
*/
inline DwGraphicsNinePatchItem* DwPushButton::icon() const
{
    return m_icon;
}

//------------------------------------------------------------------------------
/**
*/
inline DwGraphicsSimpleTextItem* DwPushButton::text() const
{
    return m_text;
}
