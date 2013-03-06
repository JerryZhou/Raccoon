#include "stable.h"
#include "dwmultiedit.h"
#include "dwmultitextlayoutengine.h"
#include "dwmultiediteventhandler.h"
#include "dwgraphicsedittext.h"
#include "dwcontentsizevalidater.h"
#include "dwlayoutenginedocvalidater.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwMultiEdit, 'MEIT', DwSimpleEdit);

//------------------------------------------------------------------------------
/**
*/
DwMultiEdit::DwMultiEdit( DwGraphicsItem *parent /*= NULL*/ )
    : DwSimpleEdit(parent)
{
    removeFromFocusLinkList();
    setFlag(Dw::ItemSelfKeyFocus);

    m_editText->setLayoutEngine(new DwMultiTextLayoutEngine());
    m_editText->layoutEngine().setOrgContentRect(m_editText->boundingRect().toRect());

    setEventHandler(new DwMultiEditEventHandler(this, m_background));

    // will check the size
    DwContentSizeValidater * v = new DwContentSizeValidater(&(m_editText->doc()), this);
    m_editText->doc().appendValidater(v);

    // will invalidate the layout engine 's cache
    // DwLayoutEngineDocValidater * ve = new DwLayoutEngineDocValidater(&(m_editText->doc()), this);
    // m_editText->doc().appendValidater(ve);

    // auto line break
    m_editText->doc().textFormatAt(0)->setAutoLineBreak(true);

    // accessible
    setAccessibleName(tr("raccoon multi edit"));
    setAccessibleDescription(tr("raccoon multi edit"));
    setWhatsThis(tr("raccoon base multi edit"));
}

//------------------------------------------------------------------------------
/**
*/
DwMultiEdit::~DwMultiEdit()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwMultiEdit::setOrgContentRect(const DwRect &r)
{
    setBoundingRect(r);
    updateOrgContent();
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwMultiEdit::orgContentRect() const
{
    return m_editText->layoutEngine().orgContentRect();
}

//------------------------------------------------------------------------------
/**
*/
void DwMultiEdit::updateOrgContent()
{
    m_editText->layoutEngine().setOrgContentRect(m_editText->boundingRect().toRect());
}

