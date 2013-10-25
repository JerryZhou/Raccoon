#pragma once
#include "dwgui/dwdocumentvalidater.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwNumWidthDocValidater : public DwDocumentValidater
{
    DW_DECLARE_CLASS(DwNumWidthDocValidater);
public:
    explicit DwNumWidthDocValidater(DwTextDocument *doc = NULL, int num = 0);

    int deltaOf(DwRichChar *c);

    bool setNumWidth(int num);

    virtual bool validateDoc();
    virtual bool onAdd(DwRichChar *c, int pos = -1);
    virtual bool onDel(DwRichChar *c, int pos = -1);
    virtual void refresh();

protected:
    int m_curWidth;
    int m_maxNumWidth;
};// end of DwNumWidthDocValidater
DW_REGISTER_CLASS(DwNumWidthDocValidater);
