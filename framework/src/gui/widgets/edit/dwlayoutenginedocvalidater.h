#pragma once
#include "dwgui/dwdocumentvalidater.h"

//------------------------------------------------------------------------------
class DwSimpleEdit;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwLayoutEngineDocValidater : public DwDocumentValidater
{
    DW_DECLARE_CLASS(DwLayoutEngineDocValidater);
public:
    explicit DwLayoutEngineDocValidater(DwTextDocument *doc = NULL, DwSimpleEdit *edit = NULL);

    virtual bool validateDoc();
    virtual bool onAdd(DwRichChar *c, int pos = -1);
    virtual bool onDel(DwRichChar *c, int pos = -1);

protected:
    DwSimpleEdit *m_edit;
};// end of DwContentSizeValidater
DW_REGISTER_CLASS(DwLayoutEngineDocValidater);