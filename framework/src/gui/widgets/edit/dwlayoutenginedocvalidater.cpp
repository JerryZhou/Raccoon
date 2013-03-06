#include "stable.h"
#include "dwlayoutenginedocvalidater.h"
#include "dwtextdocument.h"
#include "dwrichchar.h"
#include "dwsimpleedit.h"
#include "dwgraphicsedittext.h"
#include "dwgui/dwanchors.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwLayoutEngineDocValidater, 'LEDR', DwDocumentValidater);

//------------------------------------------------------------------------------
/**
*/
DwLayoutEngineDocValidater::DwLayoutEngineDocValidater(DwTextDocument *doc/* = NULL*/, DwSimpleEdit *edit/* = NULL*/)
    : DwDocumentValidater(doc)
    , m_edit(NULL)
{
    m_edit = edit;

    open();
    setValidater(DwDocumentValidater::ValidaterAdd);
    setValidater(DwDocumentValidater::ValidaterDel);
    setValidaterWhen(DwDocumentValidater::ValidaterAfter);
}

//------------------------------------------------------------------------------
/**
*/
bool DwLayoutEngineDocValidater::validateDoc()
{
    return m_edit->editText()->layoutEngine().invalidateLayoutCache(DwTextLayoutEngine::Content);
}

//------------------------------------------------------------------------------
/**
*/
bool DwLayoutEngineDocValidater::onAdd(DwRichChar *c, int pos/* = -1*/)
{
    DW_UNUSED(c);
    DW_UNUSED(pos);
    //validateDoc();
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLayoutEngineDocValidater::onDel(DwRichChar *c, int pos/* = -1*/)
{
    DW_UNUSED(c);
    DW_UNUSED(pos);
    //validateDoc();
    return true;
}
