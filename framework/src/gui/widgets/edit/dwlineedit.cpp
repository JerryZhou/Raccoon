#include "stable.h"
#include "dwlineedit.h"
#include "dwgui/dwgraphicssimpletextitem.h"
#include "dwgui/dwdocumentvalidater.h"
#include "dwgui/dwcharactersdocvalidater.h"
#include "dwgui/dwnumwidthvalidater.h"
#include "dwgui/dwtextdocument.h"
#include "dwgui/dwgraphicsedittext.h"
#include "dwgui/dwdefaultitemeventhandler.h"
#include "dwgui/dwanchors.h"
#include "dwgraphics/dwfont.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwLineEdit, 'LEIT', DwSimpleEdit);

//------------------------------------------------------------------------------
/**
*/
DwLineEdit::DwLineEdit( DwGraphicsItem *parent /*= NULL*/ )
    : DwSimpleEdit(parent)
    , m_charsetValidater(NULL)
    , m_numWidthValidater(NULL)
    , m_placeHolder(NULL)
{
    // accessible
    setAccessibleName(tr("raccoon line edit"));
    setAccessibleDescription(tr("raccoon line edit"));
    setWhatsThis(tr("raccoon base line edit"));
}

//------------------------------------------------------------------------------
/**
*/
DwLineEdit::~DwLineEdit()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwLineEdit::setAcceptCharacterSet(const DwString& s)
{
    if (!m_charsetValidater)
    {
        m_charsetValidater = new DwCharactersDocValidater(&m_editText->doc(), s);
        m_editText->doc().appendValidater(m_charsetValidater);
    }
    else
    {
        m_charsetValidater->setCharacters(s);
    }
}

//------------------------------------------------------------------------------
/**
*/
static void onFocusIn(DwGraphicsItem* obj, DwEvent* )
{
    DwLineEdit* edit = dwsafe_cast<DwLineEdit>(obj);
    if (edit)
    {
        edit->setPlaceHolderVisible(false);
    }
}

//------------------------------------------------------------------------------
/**
*/
static void onFocusOut(DwGraphicsItem* obj, DwEvent* )
{
    DwLineEdit* edit = dwsafe_cast<DwLineEdit>(obj);
    if (edit && edit->editText()->doc().length() == 0)
    {
        edit->setPlaceHolderVisible(true);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwLineEdit::setPlaceHolderText(const DwString& s)
{
    if (!m_placeHolder)
    {
        m_placeHolder = new DwGraphicsSimpleTextItem(s, this);
        m_placeHolder->setName("placeHolder");
        m_placeHolder->setFontColor(DwColor(121, 80, 51, 198));
        m_placeHolder->anchors()->setCenterIn(this);
        
        eventHandler()->addEventCallBack(DwFocusInEvent::EventId(), onFocusIn);
        eventHandler()->addEventCallBack(DwFocusOutEvent::EventId(), onFocusOut);

        if (!hasFocus() && editText()->doc().length() == 0)
        {
            m_placeHolder->setVisible(true);
        }
    }
    else
    {
        m_placeHolder->setText(s);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwLineEdit::setPlaceHolderTextFont(const DwFont& f)
{
    if (m_placeHolder)
    {
        m_placeHolder->setFont(f);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwLineEdit::setPlaceHolderVisible( bool b )
{
    if (m_placeHolder)
    {
        m_placeHolder->setVisible(b);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwLineEdit::setMaxLength(int len)
{
    if (!m_numWidthValidater)
    {
        m_numWidthValidater = new DwNumWidthDocValidater(&editText()->doc(), len);
        m_editText->doc().appendValidater(m_numWidthValidater);
    }
    else
    {
        m_numWidthValidater->setNumWidth(len);
    }
}