#include "stable.h"
#include "dwcontentsizevalidater.h"
#include "dwtextdocument.h"
#include "dwrichchar.h"
#include "dwsimpleedit.h"
#include "dwgraphicsedittext.h"
#include "dwgui/dwanchors.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwContentSizeValidater, 'NDVR', DwDocumentValidater);

//------------------------------------------------------------------------------
/**
*/
DwContentSizeValidater::DwContentSizeValidater(DwTextDocument *doc/* = NULL*/, DwSimpleEdit *edit/* = NULL*/)
    : DwDocumentValidater(doc)
    , m_edit(NULL)
{
    m_edit = edit;

    open();
    setValidater(DwDocumentValidater::ValidaterAdd);
    setValidater(DwDocumentValidater::ValidaterDel);
    setValidaterWhen(DwDocumentValidater::ValidaterAfter);
    setValidaterWhen(DwDocumentValidater::ValidaterBefore, false);
}

//------------------------------------------------------------------------------
/**
*/
bool DwContentSizeValidater::validateDoc()
{
    DwTextDocument &doc = m_edit->editText()->doc();
    DwTextLayoutEngine & textLayoutEngine = m_edit->editText()->layoutEngine();

    DwRectF rect = m_edit->editText()->boundingRect();
    DwRectF textRect;
    bool isOk = textLayoutEngine.contentRect(&doc, rect, textRect);

    if (isOk && textRect.toRect().size() != rect.toRect().size())
    {
        DwReal left = m_edit->editText()->anchors()->leftMargin();
        DwReal right = m_edit->editText()->anchors()->rightMargin();
        DwReal top = m_edit->editText()->anchors()->topMargin();
        DwReal bottom = m_edit->editText()->anchors()->bottomMargin();

        DwRectF editRect(textRect);
        editRect.adjust(0, 0, left + right , top + bottom );

        if (!doc.textFormatAt(0)->isAutoLineBreak())
        {
            m_edit->setWidth(editRect.width());
        }

        m_edit->setHeight(editRect.height());
    }

    return isOk;
}

//------------------------------------------------------------------------------
/**
*/
bool DwContentSizeValidater::onAdd(DwRichChar *c, int pos/* = -1*/)
{
    DW_UNUSED(c);
    DW_UNUSED(pos);
    DwTextDocument &doc = m_edit->editText()->doc();
    DwTextLayoutEngine & textLayoutEngine = m_edit->editText()->layoutEngine();

    textLayoutEngine.onAdd(&doc, m_edit->editText()->boundingRect(), c, pos);

    return validateDoc();
}

//------------------------------------------------------------------------------
/**
*/
bool DwContentSizeValidater::onDel(DwRichChar *c, int pos/* = -1*/)
{
    DW_UNUSED(c);
    DW_UNUSED(pos);
    DwTextDocument &doc = m_edit->editText()->doc();
    DwTextLayoutEngine & textLayoutEngine = m_edit->editText()->layoutEngine();

    textLayoutEngine.onDel(&doc, m_edit->editText()->boundingRect(), c, pos);

    return validateDoc();
}
