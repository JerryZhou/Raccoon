#pragma once
#include "dwgui/dwsimpleedit.h"

//------------------------------------------------------------------------------
class DwCharactersDocValidater;
class DwNumWidthDocValidater;
class DwGraphicsSimpleTextItem;
class DwFont;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwLineEdit : public DwSimpleEdit
{
    DW_DECLARE_CLASS(DwLineEdit);
public:
    explicit DwLineEdit(DwGraphicsItem *parent = NULL);
    virtual ~DwLineEdit();

    void setAcceptCharacterSet(const DwString& s);
    void setPlaceHolderText(const DwString& s);
    void setPlaceHolderTextFont(const DwFont& f);
    void setPlaceHolderVisible(bool b);
    void setMaxLength(int len);

protected:
    DwCharactersDocValidater* m_charsetValidater;
    DwNumWidthDocValidater* m_numWidthValidater;
    DwGraphicsSimpleTextItem* m_placeHolder;
};// end of DwLineEdit
DW_REGISTER_CLASS(DwLineEdit);