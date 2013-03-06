#include "stable.h"
#include "dwitemdelegatelineedit.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwlineedit.h"
#include "dwgui/dwgraphicsedittext.h"
#include "dwgui/dwdocumentvalidater.h"
#include "dwgui/dwtextdocument.h"

DW_IMPLEMENT_CLASS(DwItemDelegateLineEdit, 'IDLE', DwItemDelegate);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(isReadOnly)
{
    DwLineEdit *edit = dwsafe_cast<DwLineEdit>(obj);
    DW_CHECK_ASSERT_RET(edit, 0);

    edit->setReadOnly(_lua_pop_value_of<bool>(L, 3));
    return 0;
}

DECLARE_DELEGATE_SET(text)
{
    DwLineEdit *edit = dwsafe_cast<DwLineEdit>(obj);
    DW_CHECK_ASSERT_RET(edit, 0);

    DwString s = edit->editText()->doc().plainText();
    DwString sTo = _lua_pop_value_of<DwString>(L, 3);
    if (s != sTo)
    {
        edit->editText()->doc().clear();
        edit->editText()->doc().append(sTo);
    }
    return 0;
}

DECLARE_DELEGATE_SET(acceptCharacterSet)
{
    DwLineEdit *edit = dwsafe_cast<DwLineEdit>(obj);
    DW_CHECK_ASSERT_RET(edit, 0);

    DwString sTo = _lua_pop_value_of<DwString>(L, 3);
    edit->setAcceptCharacterSet(sTo);
    return 0;
}

DECLARE_DELEGATE_SET(placeHolderText)
{
    DwLineEdit *edit = dwsafe_cast<DwLineEdit>(obj);
    DW_CHECK_ASSERT_RET(edit, 0);

    DwString sTo = _lua_pop_value_of<DwString>(L, 3);
    edit->setPlaceHolderText(sTo);
    return 0;
}

DECLARE_DELEGATE_SET(placeHolderTextFont)
{
    DwLineEdit *edit = dwsafe_cast<DwLineEdit>(obj);
    DW_CHECK_ASSERT_RET(edit, 0);

    DwFont sTo = _lua_pop_value_of<DwFont>(L, 3);
    edit->setPlaceHolderTextFont(sTo);
    return 0;
}

DECLARE_DELEGATE_SET(placeHolderVisible)
{
    DwLineEdit *edit = dwsafe_cast<DwLineEdit>(obj);
    DW_CHECK_ASSERT_RET(edit, 0);

    bool sTo = _lua_pop_value_of<bool>(L, 3);
    edit->setPlaceHolderVisible(sTo);
    return 0;
}

DECLARE_DELEGATE_SET(maxLength)
{
    DwLineEdit *edit = dwsafe_cast<DwLineEdit>(obj);
    DW_CHECK_ASSERT_RET(edit, 0);

    int sTo = _lua_pop_value_of<int>(L, 3);
    edit->setMaxLength(sTo);
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(isReadOnly)
{
    DwLineEdit *edit = dwsafe_cast<DwLineEdit>(obj);
    DW_CHECK_ASSERT_RET(edit, 0);
    return _lua_push_value_of<bool>(L, edit->isReadOnly());
}

DECLARE_DELEGATE_GET(text)
{
    DwLineEdit *edit = dwsafe_cast<DwLineEdit>(obj);
    DW_CHECK_ASSERT_RET(edit, 0);

    DwString s = edit->editText()->doc().plainText();
    return _lua_push_value_of<DwString>(L, s);
}

DECLARE_DELEGATE_GET(length)
{
    DwLineEdit *edit = dwsafe_cast<DwLineEdit>(obj);
    DW_CHECK_ASSERT_RET(edit, 0);
    return _lua_push_value_of<int>(L, edit->editText()->doc().length());
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(setText)
{
    return delegate_set_text(obj, L);
}

//------------------------------------------------------------------------------
/**
*/
DwItemDelegateLineEdit::DwItemDelegateLineEdit()
{
    DECLARE_ADD_SET(isReadOnly);
    DECLARE_ADD_SET(text);
    DECLARE_ADD_SET(acceptCharacterSet);
    DECLARE_ADD_SET(placeHolderText);
    DECLARE_ADD_SET(placeHolderTextFont);
    DECLARE_ADD_SET(placeHolderVisible);
    DECLARE_ADD_SET(maxLength);

    DECLARE_ADD_GET(isReadOnly);
    DECLARE_ADD_GET(text);
    DECLARE_ADD_GET(length);

    DECLARE_ADD_CALL(setText);
}