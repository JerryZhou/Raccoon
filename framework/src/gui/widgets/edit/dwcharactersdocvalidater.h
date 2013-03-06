#pragma once
#include "dwgui/dwdocumentvalidater.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwCharactersDocValidater : public DwDocumentValidater
{
    DW_DECLARE_CLASS(DwCharactersDocValidater);
public:
    explicit DwCharactersDocValidater(DwTextDocument *doc = NULL , const DwString & s = DwString());

    bool setCharacters(const DwString &s);

    virtual bool onAdd(DwRichChar *c, int pos = -1);

protected:
    DwString m_characters;
};// end of DwCharactersDocValidater
DW_REGISTER_CLASS(DwCharactersDocValidater);