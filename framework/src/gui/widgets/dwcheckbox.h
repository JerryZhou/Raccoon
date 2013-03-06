#pragma once
#include "dwgui/dwpushbutton.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwCheckBox : public DwPushButton
{
    DW_DECLARE_CLASS(DwCheckBox);
public:
    explicit DwCheckBox(DwGraphicsItem *parent = NULL);
    virtual ~DwCheckBox();

    bool checked() const;
    void setChecked(bool b);
    void setText(const DwString& );

protected:
    friend class DwCheckBoxEventHandler;

    DwWidget::FrameState curState() const;
    void updateIcon();

    Dw::CheckedState m_state;
};// end of DwCheckBox
DW_REGISTER_CLASS(DwCheckBox);