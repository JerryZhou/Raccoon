#pragma once
#include "dwgui/dwwidgetitem.h"

//------------------------------------------------------------------------------
class DwGraphicsEditText;
class DwGraphicsBackgroundItem;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwSimpleEdit : public DwWidgetItem
{
    DW_DECLARE_CLASS(DwSimpleEdit);
public:
    explicit DwSimpleEdit(DwGraphicsItem *parent = NULL);
    virtual ~DwSimpleEdit();

    DwGraphicsBackgroundItem *background() const;
    DwGraphicsEditText *editText() const;

    void setReadOnly(bool b);
    bool isReadOnly() const;

protected:
    DwGraphicsBackgroundItem * m_background;
    DwGraphicsEditText *m_editText;
    bool m_readOnly;
};// end of DwSimpleEdit
DW_REGISTER_CLASS(DwSimpleEdit);