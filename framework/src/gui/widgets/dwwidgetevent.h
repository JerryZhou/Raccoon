#pragma once
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwrichchar.h"
#include "dwgui/dwrttiobjectptr.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwWidgetEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwWidgetEvent);
public:
protected:
private:
};// end of DwWidgetEvent
DW_REGISTER_CLASS(DwWidgetEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwWidgetWindowEvent : public DwWidgetEvent
{
    DW_DECLARE_EVENTID(DwWidgetWindowEvent);
public:
    enum EKind { Nothing, Open, Close, UpdateTitle, ResizeMax, ResizeMin, ResizeRestore};
    DwWidgetWindowEvent( EKind k = Nothing)
        : m_kind(k)
    {
        ;
    }

    EKind m_kind;
protected:
private:
};// end of DwWidgetWindowEvent
DW_REGISTER_CLASS(DwWidgetWindowEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwCheckBoxEvent : public DwWidgetEvent
{
    DW_DECLARE_EVENTID(DwCheckBoxEvent);
public:
    DwCheckBoxEvent()
        : m_isChecked (false)
        , m_newState(Dw::UnChecked)
    {
        ;
    }
    bool m_isChecked;
    Dw::CheckedState m_newState;
protected:
private:
};// end of DwCheckBoxEvent
DW_REGISTER_CLASS(DwCheckBoxEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwTextEditEvent : public DwWidgetEvent
{
    DW_DECLARE_EVENTID(DwTextEditEvent);
public:
    DwTextEditEvent()
        : m_change(DwWidget::Ops)
    {
        ;
    }
    DwWidget::EditTextChange m_change;
    DwRichChar m_char;
};// end of DwTextEditEvent
DW_REGISTER_CLASS(DwTextEditEvent);

//------------------------------------------------------------------------------
class DwPushButton;
class DW_GUI_EXPORT DwMessageBoxEvent : public DwWidgetEvent
{
    DW_DECLARE_EVENTID(DwMessageBoxEvent);
public:
    DwMessageBoxEvent()
        : m_btn(0)
    {
        ;
    }

    DwPushButton *m_btn;
};// end of DwMessageBoxEvent
DW_REGISTER_CLASS(DwMessageBoxEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwScrollBarEvent : public DwWidgetEvent
{
    DW_DECLARE_EVENTID(DwScrollBarEvent);
public:
    DwScrollBarEvent()
        : m_cur(0.0)
        , m_delta(0.0)
        , m_flow(DwWidget::TopToBottom)
        , m_down(false)
        , m_up(false)
    {
        ;
    }

    DwWidget::Flow m_flow;
    DwReal m_cur;
    DwReal m_delta;
    bool m_down;
    bool m_up;
};// end of DwScrollBarEvent
DW_REGISTER_CLASS(DwScrollBarEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwScrollAreaEvent : public DwWidgetEvent
{
    DW_DECLARE_EVENTID(DwScrollAreaEvent);
public:
    DwScrollAreaEvent()
        : m_flow(DwWidget::TopToBottom)
        , m_percent(0.0)
    {
        ;
    }

    DwWidget::Flow m_flow;
    DwReal m_percent;
};// end of DwScrollAreaEvent
DW_REGISTER_CLASS(DwScrollAreaEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwMenuEvent : public DwWidgetEvent
{
    DW_DECLARE_EVENTID(DwMenuEvent);
public:
    DwMenuEvent()
    {
        ;
    }

    DwString m_actionName;
    DwRttiObjectPtr<DwGraphicsItem> m_action;
};// end of DwMenuEvent
DW_REGISTER_CLASS(DwMenuEvent);