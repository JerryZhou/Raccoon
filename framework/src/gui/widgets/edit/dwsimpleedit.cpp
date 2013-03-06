#include "stable.h"
#include "dwsimpleedit.h"
#include "dwgraphicsedittext.h"
#include "dwediteventlistener.h"
#include "dwgui/dwsimpleediteventhandler.h"
#include "dwgui/dwgraphicsframeitem.h"
#include "dwgui/dwanchors.h"
#include "dwgui/dwgraphicsbackgrounditem.h"

//------------------------------------------------------------------------------
const char KNormalImage[] = "dwgui:edit.png";

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwSimpleEdit, 'SEIT', DwWidgetItem);

DwSimpleEdit::DwSimpleEdit(DwGraphicsItem *parent /*= NULL*/)
    : DwWidgetItem(parent)
    , m_background(NULL)
    , m_editText(NULL)
    , m_readOnly(false)
{
    setName(L"SimpleEdit");
    setBoundingRect(DwRectF(0., 0., 102., 23.));
    setFocusPolicy(Dw::StrongFocus);

    m_background = new DwGraphicsBackgroundItem(this);
    m_background->setName(L"background");
    m_background->setURL(KNormalImage);
    m_background->anchors()->setFill(this);

    m_editText = new DwGraphicsEditText(this);
    m_editText->setName(L"text");
    m_editText->anchors()->setMargins(2);
    m_editText->anchors()->setFill(this);
    m_editText->layoutEngine().setOrgContentRect(m_editText->boundingRect().toRect());

    //will send the event about the document
    DwEditEventListener * ve = new DwEditEventListener(&(m_editText->doc()), this);
    m_editText->doc().appendValidater(ve);

    setEventHandler(new DwSimpleEditEventHandler(this, m_background));

    // accessible
    setAccessibleName(tr("raccoon simple edit"));
    setAccessibleDescription(tr("raccoon simple edit"));
    setWhatsThis(tr("raccoon base simple edit"));
}

//------------------------------------------------------------------------------
/**
*/
DwSimpleEdit::~DwSimpleEdit()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsBackgroundItem *DwSimpleEdit::background() const
{
    return m_background;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsEditText *DwSimpleEdit::editText() const
{
    return m_editText;
}

//------------------------------------------------------------------------------
/**
*/
void DwSimpleEdit::setReadOnly(bool b)
{
    m_readOnly = b;
}

//------------------------------------------------------------------------------
/**
*/
bool DwSimpleEdit::isReadOnly() const
{
    return m_readOnly;
}