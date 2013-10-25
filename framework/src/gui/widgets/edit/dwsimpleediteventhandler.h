#pragma once
#include "dwgui/dwwidgeteventhandler.h"

//------------------------------------------------------------------------------
class DwSimpleEdit;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwSimpleEditEventHandler : public DwWidgetEventHandler
{
    DW_DECLARE_CLASS(DwSimpleEditEventHandler);
    typedef DwWidgetEventHandler supreClass;

public:
    DwSimpleEditEventHandler(DwSimpleEdit* edit = NULL, DwGraphicsBackgroundItem* back = NULL);

    virtual void onKeyDown(DwKeyDownEvent* evt);
    virtual void onCharEvent(DwCharEvent* evt);
#if DW_ENABLE_EVENT_FOCUS
    virtual void onFocusIn(DwFocusInEvent* evt);
    virtual void onFocusOut(DwFocusOutEvent *evt);
#endif

    virtual void onMouseClicked(DwMouseClickedEvent* evt);
    virtual void onMouseDown(DwMouseDownEvent* evt);
    virtual void onMouseUp(DwMouseUpEvent* evt);
    virtual void onMouseMoved(DwMouseMoveEvent* evt);
    virtual void onMouseDbClicked(DwMouseDbClickEvent* evt);

    void enableCopy(bool b);
    bool isEnableCopy() const;

protected:
    void clearSelect();
    DwString selectText();
    virtual void updateGraphicsEditToLookCursor();

    DwSimpleEdit *m_edit;
    bool m_mouseDown;
    bool m_dbClick;
    bool m_enableCopy;
};// end of DwSimpleEditEventHandler
DW_REGISTER_CLASS(DwSimpleEditEventHandler);

//------------------------------------------------------------------------------
/**
*/
inline void DwSimpleEditEventHandler::enableCopy(bool b)
{
    m_enableCopy = b;
}


//------------------------------------------------------------------------------
/**
*/
inline bool DwSimpleEditEventHandler::isEnableCopy() const
{
    return m_enableCopy;
}
