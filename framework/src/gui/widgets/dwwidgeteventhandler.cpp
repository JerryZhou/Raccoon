#include "stable.h"
#include "dwwidgeteventhandler.h"
#include "dwgraphicsbackgrounditem.h"
#include "kernel/dwcursor.h"
#include "dwgui/dweventsignaldispatch.h"

DW_IMPLEMENT_CLASS(DwWidgetEventHandler, 'DWEH', DwDefaultItemEventHandler);

//------------------------------------------------------------------------------
/**
*/
DwWidgetEventHandler::DwWidgetEventHandler(DwGraphicsItem* item, DwGraphicsBackgroundItem* back/* = NULL*/)
    : DwDefaultItemEventHandler(item)
    , m_background(back)
{
    ;
};

//------------------------------------------------------------------------------
/**
*/
DwWidgetEventHandler::~DwWidgetEventHandler()
{
    m_background = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void DwWidgetEventHandler::onMouseDown(DwMouseDownEvent* evt)
{
    SuperClass::onMouseDown(evt);
    if(m_background) m_background->setPressedDown(true);

    evt->accept();
}

//------------------------------------------------------------------------------
/**
*/
void DwWidgetEventHandler::onMouseUp(DwMouseUpEvent* evt)
{
    SuperClass::onMouseUp(evt);
    if(m_background) m_background->setPressedDown(false);

    evt->accept();
}

#if DW_ENABLE_EVENT_HOVER
//------------------------------------------------------------------------------
/**
*/
void DwWidgetEventHandler::onHoverEnter(DwHoverEnterEvent* evt)
{
    SuperClass::onHoverEnter(evt);
    if(m_background) m_background->setHover(true);

    if (m_item->isMovable() && !m_item->isA(L"DwFrameWindow"))
    {
        DwCursor::save();
        DwCursor::setCursor(Dw::PointingHandCursor);
    }
    evt->accept();
}

//------------------------------------------------------------------------------
/**
*/
void DwWidgetEventHandler::onHoverLeave(DwHoverLeaveEvent* evt)
{
    SuperClass::onHoverLeave(evt);
    if(m_background) m_background->setHover(false);

    if (m_item->isMovable() && !m_item->isA(L"DwFrameWindow"))
    {
        DwCursor::restore();
    }
    evt->accept();
}
#endif

#if DW_ENABLE_EVENT_FOCUS
//------------------------------------------------------------------------------
/**
*/
void DwWidgetEventHandler::onFocusIn(DwFocusInEvent* evt)
{
    SuperClass::onFocusIn(evt);
    if (m_background) m_background->setFocus(true);
}

//------------------------------------------------------------------------------
/**
*/
void DwWidgetEventHandler::onFocusOut(DwFocusOutEvent* evt)
{
    SuperClass::onFocusOut(evt);
    if (m_background) m_background->setFocus(false);
}
#endif

#if DW_ENABLE_EVENT_DRAGDROP
//------------------------------------------------------------------------------
/**
*/
void DwWidgetEventHandler::onDragStartEvent(DwDragStartEvent* evt)
{
    evt->accept();
}

//------------------------------------------------------------------------------
/**
*/
void DwWidgetEventHandler::onDragMoveEvent(DwDragMoveEvent* evt)
{
    DW_ASSERT(evt->eventId() == DwDragMoveEvent::EventId());
    DW_ASSERT(m_item->isMovable());

    DwDragMoveEvent* drag = static_cast<DwDragMoveEvent*>(evt);
    m_item->setPos(m_item->pos() + drag->m_delta);
    evt->accept();
}

//------------------------------------------------------------------------------
/**
*/
void DwWidgetEventHandler::onDragEndEvent(DwDragEndEvent* evt)
{
    evt->accept();
}
#endif

#if DW_ENABLE_EVENT_TIPS
//------------------------------------------------------------------------------
/**
*/
void DwWidgetEventHandler::onToolTipEvent(DwToolTipEvent *evt)
{
    DwToolTipEvent* tipEvt = static_cast<DwToolTipEvent*>(evt);
    tipEvt->m_tips = m_item->toolTips();
    evt->accept();
}
#endif