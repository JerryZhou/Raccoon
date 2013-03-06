#include "stable.h"
#include "dwediteventlistener.h"
#include "dwtextdocument.h"
#include "dwrichchar.h"
#include "dwsimpleedit.h"
#include "dwgraphicsedittext.h"
#include "dwgui/dwwidgetevent.h"
#include "dwgui/dwanchors.h"
#include "dwgui/dwgraphicsscene.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwEditEventListener, 'EELR', DwDocumentValidater);

//------------------------------------------------------------------------------
/**
*/
DwEditEventListener::DwEditEventListener(DwTextDocument *doc/* = NULL*/, DwSimpleEdit *edit/* = NULL*/)
    : DwDocumentValidater(doc)
    , m_edit(NULL)
{
    m_edit = edit;

    open();
    setValidater(DwDocumentValidater::ValidaterAdd);
    setValidater(DwDocumentValidater::ValidaterDel);
    setValidaterWhen(DwDocumentValidater::ValidaterAfter, true);
    setValidaterWhen(DwDocumentValidater::ValidaterBefore, false);
}

//------------------------------------------------------------------------------
/**
*/
bool DwEditEventListener::validateDoc()
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwEditEventListener::onAdd(DwRichChar *c, int pos/* = -1*/)
{
    DW_UNUSED(c);
    DW_UNUSED(pos);

    DwTextEditEvent evt;
    evt.m_change = DwWidget::AddChar;
    evt.m_char = *c;

    m_edit->sendEvent(&evt);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwEditEventListener::onDel(DwRichChar *c, int pos/* = -1*/)
{
    DW_UNUSED(c);
    DW_UNUSED(pos);

    DwTextEditEvent evt;
    evt.m_change = DwWidget::DelChar;
    evt.m_char = *c;

    m_edit->sendEvent(&evt);
    return true;
}
