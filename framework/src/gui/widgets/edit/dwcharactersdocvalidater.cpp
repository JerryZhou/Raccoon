#include "stable.h"
#include "dwcharactersdocvalidater.h"
#include "dwtextdocument.h"
#include "dwrichchar.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwCharactersDocValidater, 'DCDR', DwDocumentValidater);

//------------------------------------------------------------------------------
/**
*/
DwCharactersDocValidater::DwCharactersDocValidater(DwTextDocument *doc/* = NULL */,
        const DwString & s/* = DwString()*/)
    : DwDocumentValidater(doc)
    , m_characters(s)
{
    open();
    setValidater(DwDocumentValidater::ValidaterAdd);
}

//------------------------------------------------------------------------------
/**
*/
bool DwCharactersDocValidater::setCharacters(const DwString &s)
{
    m_characters = s;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwCharactersDocValidater::onAdd(DwRichChar *c, int pos/* = -1*/)
{
    DW_UNUSED(pos);
    return m_characters.indexOf(c->character()) != -1;
}