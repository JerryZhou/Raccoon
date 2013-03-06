#include "stable.h"
#include "dwitemdelegatesimpletext.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwgraphicssimpletextitem.h"

DW_IMPLEMENT_CLASS(DwItemDelegateSimpleText, 'IDST', DwItemDelegate);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(text)
{
    DwGraphicsSimpleTextItem *simpleText = dwsafe_cast<DwGraphicsSimpleTextItem>(obj);
    DW_CHECK_ASSERT_RET(simpleText, 0);

    simpleText->setText(_lua_pop_value_of<DwString>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(fontSize)
{
    DwGraphicsSimpleTextItem *simpleText = dwsafe_cast<DwGraphicsSimpleTextItem>(obj);
    DW_CHECK_ASSERT_RET(simpleText, 0);

    simpleText->setFontSize(_lua_pop_value_of<int>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(fontColor)
{
    DwGraphicsSimpleTextItem *simpleText = dwsafe_cast<DwGraphicsSimpleTextItem>(obj);
    DW_CHECK_ASSERT_RET(simpleText, 0);

    simpleText->setFontColor(_lua_pop_value_of<DwColor>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(font)
{
    DwGraphicsSimpleTextItem *simpleText = dwsafe_cast<DwGraphicsSimpleTextItem>(obj);
    DW_CHECK_ASSERT_RET(simpleText, 0);

    simpleText->setFont(_lua_pop_value_of<DwFont>(L, -1));
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(text)
{
    DwGraphicsSimpleTextItem *simpleText = dwsafe_cast<DwGraphicsSimpleTextItem>(obj);
    DW_CHECK_ASSERT_RET(simpleText, 0);
    return _lua_push_value_of<DwString>(L, simpleText->text());
}

DECLARE_DELEGATE_GET(fontSize)
{
    DwGraphicsSimpleTextItem *simpleText = dwsafe_cast<DwGraphicsSimpleTextItem>(obj);
    DW_CHECK_ASSERT_RET(simpleText, 0);
    return _lua_push_value_of<int>(L, simpleText->fontSize());
}

DECLARE_DELEGATE_GET(fontColor)
{
    DwGraphicsSimpleTextItem *simpleText = dwsafe_cast<DwGraphicsSimpleTextItem>(obj);
    DW_CHECK_ASSERT_RET(simpleText, 0);
    return _lua_push_value_of<DwColor>(L, simpleText->fontColor());
}

DECLARE_DELEGATE_GET(font)
{
    DwGraphicsSimpleTextItem *simpleText = dwsafe_cast<DwGraphicsSimpleTextItem>(obj);
    DW_CHECK_ASSERT_RET(simpleText, 0);
    return _lua_push_value_of<DwFont>(L, simpleText->font());
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(setText)
{
    DwGraphicsSimpleTextItem *simpleText = dwsafe_cast<DwGraphicsSimpleTextItem>(obj);
    DW_CHECK_ASSERT_RET(simpleText, 0);
    simpleText->setText(_lua_pop_value_of<DwString>(L, 3));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DwItemDelegateSimpleText::DwItemDelegateSimpleText()
{
    DECLARE_ADD_SET(text);
    DECLARE_ADD_SET(fontSize);
    DECLARE_ADD_SET(fontColor);
    DECLARE_ADD_SET(font);

    DECLARE_ADD_GET(text);
    DECLARE_ADD_GET(fontSize);
    DECLARE_ADD_GET(fontColor);
    DECLARE_ADD_GET(font);

    DECLARE_ADD_CALL(setText);
}