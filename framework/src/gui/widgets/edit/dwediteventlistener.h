#pragma once
#include "dwgui/dwdocumentvalidater.h"

//------------------------------------------------------------------------------
class DwSimpleEdit;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwEditEventListener : public DwDocumentValidater
{
    DW_DECLARE_CLASS(DwEditEventListener);
public:
    explicit DwEditEventListener(DwTextDocument *doc = NULL, DwSimpleEdit *edit = NULL);

    virtual bool validateDoc();

    virtual bool onAdd(DwRichChar *c, int pos = -1);
    virtual bool onDel(DwRichChar *c, int pos = -1);

protected:
    DwSimpleEdit *m_edit;
};// end of DwEditEventListener
DW_REGISTER_CLASS(DwEditEventListener);