#pragma once
#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwTextFormat : public DwRttiObject
{
    DW_DECLARE_CLASS(DwTextFormat);
public:
    DwTextFormat();

    void setAutoLineBreak(bool b);
    bool isAutoLineBreak() const;

    int lineSpacing() const;
    void setLineSpacing(int spacing) ;

protected:
    bool m_autoLineBreak;
    int m_lineSpacing;
};// end of DwTextFormat
DW_REGISTER_CLASS(DwTextFormat);