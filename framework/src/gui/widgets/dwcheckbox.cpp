#include "stable.h"
#include "dwcheckbox.h"
#include "dwcheckbox_p.h"
#include "dwwidgetevent.h"
#include "dwgui/dwgraphicsscene.h"
#include "dwgui/dwgraphicsframeitem.h"
#include "dwgui/dwgraphicsbackgrounditem.h"

DW_IMPLEMENT_CLASS(DwCheckBox, 'CBOX', DwPushButton);

//------------------------------------------------------------------------------
/**
*/
DwCheckBox::DwCheckBox( DwGraphicsItem *parent /*= NULL*/ )
    : DwPushButton(parent)
    , m_state(Dw::UnChecked)
{
    setFocusable(false);
    background()->setVisible(false);

    setEventHandler(new DwCheckBoxEventHandler(this, m_background));
    setText(tr("checkbox"));
    updateIcon();

    // accessible
    setAccessibleName(tr("raccoon check box"));
    setAccessibleDescription(tr("raccoon margins check box"));
    setWhatsThis(tr("raccoon base margins check box"));
}

//------------------------------------------------------------------------------
/**
*/
DwCheckBox::~DwCheckBox()
{

}

//------------------------------------------------------------------------------
/**
*/
void DwCheckBox::setText(const DwString& txt)
{
    DwPushButton::setText(txt);
    adjustSize();
}

//------------------------------------------------------------------------------
/**
*/
bool DwCheckBox::checked() const
{
    return m_state == Dw::Checked;
}

//------------------------------------------------------------------------------
/**
*/
void DwCheckBox::setChecked( bool b )
{
    Dw::CheckedState newState = b ? Dw::Checked : Dw::UnChecked;
    if (newState == m_state)
    {
        return;
    }

    m_state = newState;
    updateIcon();

    DwCheckBoxEvent evt;
    evt.m_isChecked = checked();
    evt.m_newState = newState;
    sendEvent(&evt);
}

//------------------------------------------------------------------------------
/**
*/
DwWidget::FrameState DwCheckBox::curState() const
{
    DwGraphicsBackgroundItem *back = background();
    if ( back->isDisable() )
    {
        return DwWidget::SDisable;
    }
    else if( back->isPressedDown() )
    {
        return DwWidget::SPressDown;
    }
    else if ( back->isFocus() )
    {
        return DwWidget::SFocus;
    }
    else if ( back->isHover() )
    {
        return DwWidget::SHover;
    }
    else
    {
        return DwWidget::SNormal;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwCheckBox::updateIcon()
{
    DwWidget::FrameState s = curState();
    setIcon("dwgui:checkbox.png", m_state*DwWidget::SStateMax+s);
    adjustSize();
}