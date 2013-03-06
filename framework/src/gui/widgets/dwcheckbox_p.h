#pragma once
#include "dwgui/dwwidgeteventhandler.h"

//------------------------------------------------------------------------------
class DwCheckBoxEventHandler : public DwWidgetEventHandler
{
    DW_DECLARE_CLASS(DwCheckBoxEventHandler);
public:
    explicit DwCheckBoxEventHandler(DwGraphicsItem* item = NULL, DwGraphicsBackgroundItem* back = NULL);

    virtual ~DwCheckBoxEventHandler();

    virtual void onMouseDown(DwMouseDownEvent* evt);

    virtual void onMouseUp(DwMouseUpEvent* evt);

#if DW_ENABLE_EVENT_HOVER
    virtual void onHoverEnter(DwHoverEnterEvent* evt);

    virtual void onHoverLeave(DwHoverLeaveEvent* evt);
#endif

#if DW_ENABLE_EVENT_FOCUS
    virtual void onFocusIn(DwFocusInEvent* evt);

    virtual void onFocusOut(DwFocusOutEvent* evt);
#endif

    virtual void onMouseClicked(DwMouseClickedEvent* evt);

protected:
};
DW_IMPLEMENT_CLASS(DwCheckBoxEventHandler, 'CBER', DwWidgetEventHandler);

//------------------------------------------------------------------------------
/**
*/
DwCheckBoxEventHandler::DwCheckBoxEventHandler(DwGraphicsItem* item/* = NULL*/, DwGraphicsBackgroundItem* back/* = NULL*/)
    : DwWidgetEventHandler(item, back)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwCheckBoxEventHandler::~DwCheckBoxEventHandler()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwCheckBoxEventHandler::onMouseDown(DwMouseDownEvent* evt)
{
    __super::onMouseDown(evt);
    DwCheckBox *check = dwsafe_cast<DwCheckBox>(m_item);
    check->updateIcon();
}

//------------------------------------------------------------------------------
/**
*/
void DwCheckBoxEventHandler::onMouseUp(DwMouseUpEvent* evt)
{
    __super::onMouseUp(evt);
    DwCheckBox *check = dwsafe_cast<DwCheckBox>(m_item);
    check->updateIcon();
}

#if DW_ENABLE_EVENT_HOVER
//------------------------------------------------------------------------------
/**
*/
void DwCheckBoxEventHandler::onHoverEnter(DwHoverEnterEvent* evt)
{
    __super::onHoverEnter(evt);
    DwCheckBox *check = dwsafe_cast<DwCheckBox>(m_item);
    check->updateIcon();
}

//------------------------------------------------------------------------------
/**
*/
void DwCheckBoxEventHandler::onHoverLeave(DwHoverLeaveEvent* evt)
{
    __super::onHoverLeave(evt);
    DwCheckBox *check = dwsafe_cast<DwCheckBox>(m_item);
    check->updateIcon();
}
#endif

#if DW_ENABLE_EVENT_FOCUS
//------------------------------------------------------------------------------
/**
*/
void DwCheckBoxEventHandler::onFocusIn(DwFocusInEvent* evt)
{
    __super::onFocusIn(evt);
    DwCheckBox *check = dwsafe_cast<DwCheckBox>(m_item);
    check->updateIcon();
}

//------------------------------------------------------------------------------
/**
*/
void DwCheckBoxEventHandler::onFocusOut(DwFocusOutEvent* evt)
{
    __super::onFocusOut(evt);
    DwCheckBox *check = dwsafe_cast<DwCheckBox>(m_item);
    check->updateIcon();
}
#endif

//------------------------------------------------------------------------------
/**
*/
void DwCheckBoxEventHandler::onMouseClicked(DwMouseClickedEvent* evt)
{
    __super::onMouseClicked(evt);
    DwCheckBox *check = dwsafe_cast<DwCheckBox>(m_item);
    check->setChecked(!check->checked());
}