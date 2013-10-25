#include "stable.h"
#include "dwgui/dwgraphicsframeitem.h"
#include "dwgui/dweventsignaldispatch.h"
#include "dwgui/dwclipboard.h"
#include "dwsimpleediteventhandler.h"
#include "dwsimpleedit.h"
#include "dwinputfocuspainter.h"
#include "dwtextdocument.h"
#include "dwrichchar.h"
#include "dwgraphicsedittext.h"
#include "dwinputfocuspainter.h"
#include "dwtextlayoutengine.h"

#define _dw_update_cursor() dwInputFocusPainter()->setNeedShowIt(true);\
    updateGraphicsEditToLookCursor()

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwSimpleEditEventHandler, 'SEER', DwWidgetEventHandler);

//------------------------------------------------------------------------------
/**
*/
DwSimpleEditEventHandler::DwSimpleEditEventHandler(DwSimpleEdit* edit/* = NULL*/, DwGraphicsBackgroundItem* back/* = NULL*/)
    : DwWidgetEventHandler(edit, back)
    , m_edit(edit)
    , m_mouseDown(false)
    , m_dbClick(false)
    , m_enableCopy(true)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwSimpleEditEventHandler::clearSelect()
{
    if (m_edit->isReadOnly())
    {
        return;
    }

    DwTextDocument &doc = m_edit->editText()->doc();
    DwTextCursor &cursor = doc.cursor();
    DwTextCursor &selectCursor = doc.selectCursor();

    if (selectCursor.cur() != cursor.cur())
    {
        int start = dwMin<int>(selectCursor.cur(), cursor.cur());
        int end = dwMax<int>(selectCursor.cur(), cursor.cur());
        for (int i=end-1; i>=start && i>=0; --i)
        {
            doc.remove(i);
        }
        cursor.moveTo(start);
        selectCursor.moveTo(cursor.cur());
    }
}

//------------------------------------------------------------------------------
/**
*/
DwString DwSimpleEditEventHandler::selectText()
{
    DwTextDocument &doc = m_edit->editText()->doc();
    DwTextCursor &cursor = doc.cursor();
    DwTextCursor &selectCursor = doc.selectCursor();
    DwString text;
    if (selectCursor.cur() != cursor.cur())
    {
        int start = dwMin<int>(selectCursor.cur(), cursor.cur());
        int end = dwMax<int>(selectCursor.cur(), cursor.cur());
        for (int i=start; i<end && i<doc.length(); ++i)
        {
            text.append(wchar_t(doc.at(i).character()));
        }
    }
    return text;
}

//------------------------------------------------------------------------------
/**
*/
void DwSimpleEditEventHandler::updateGraphicsEditToLookCursor()
{
    DwTextDocument &doc = m_edit->editText()->doc();
    DwTextLayoutEngine & textLayoutEngine = m_edit->editText()->layoutEngine();
    DwPointF from;
    DwPointF to;
    bool isOk = textLayoutEngine.layout(&doc, m_edit->editText()->boundingRect(), from, to);
    if (isOk)
    {
        DwPoint offset = m_edit->editText()->offset();

        from = m_edit->editText()->mapToParent(DwPointF(from)).toPoint();
        to = m_edit->editText()->mapToParent(DwPointF(to)).toPoint();

        if (from.x() + offset.x() <= m_edit->editText()->x())
        {
            m_edit->editText()->setDisplayOffsetX( m_edit->editText()->x() - from.x() + 2 );
        }
        else if (from.x() + offset.x() >= m_edit->editText()->x() + m_edit->editText()->width() )
        {
            m_edit->editText()->setDisplayOffsetX( m_edit->editText()->x() + m_edit->editText()->width() - from.x() - 2 );
        }

        if (doc.cursor().cur() == 0)
        {
            m_edit->editText()->setDisplayOffsetX( 0 );
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwSimpleEditEventHandler::onKeyDown(DwKeyDownEvent* evt)
{
    DW_UNUSED(evt);
    supreClass::onKeyDown(evt);
    DwTextDocument &doc = m_edit->editText()->doc();
    DwTextCursor &cursor = doc.cursor();
    DwTextCursor &selectCursor = doc.selectCursor();
    int cur = cursor.cur();
    int selectCur = selectCursor.cur();
    int layoutCacheInvalidate = 0;

    if (evt->isCtrlDown())
    {
        bool deal = false;
        switch(evt->keyCode())
        {
        case 'c':
        case 'C':
	    if(m_enableCopy)
	    {
		DwString text;
		if (selectCursor.cur() != cursor.cur()) {
		    text = selectText();
		} else {
		    text = doc.plainText();
		}

		if (!text.isEmpty()) {
		    DwClipBoard::setText(text);
		}
	    }
	    break;
        case 'v':
        case 'V':
            deal = true;
            if (!m_edit->isReadOnly())
            {
                {
                    DwString text = DwClipBoard::text();
                    if (!text.isEmpty())
                    {
                        clearSelect();
                        if (!m_edit->isReadOnly())
                        {
                            int cur = cursor.cur();
                            cur += doc.insert(text, cur);
                            cursor.moveTo(cur);
                        }
                    }
                }
                layoutCacheInvalidate |= DwTextLayoutEngine::ALL;
            }
            break;
        case 'x':
        case 'X':
            deal = true;
            if (!m_edit->isReadOnly())
            {
                {
                    if (selectCursor.cur() != cursor.cur())
                    {
                        DwString text = selectText();
                        if (!text.isEmpty())
                        {
                            DwClipBoard::setText(text);
                        }
                        int cur = dwMin<int>(selectCursor.cur(), cursor.cur());
                        clearSelect();
                        cursor.moveTo(cur);
                    }
                    else
                    {
                        DwString text = doc.plainText();
                        if (!text.isEmpty())
                        {
                            DwClipBoard::setText(text);
                        }
                        doc.clear();
                        cursor.moveBegin();
                    }
                }
                layoutCacheInvalidate |= DwTextLayoutEngine::ALL;
            }
            break;
        case 'a':
        case 'A':
	    {
		cursor.moveEnd();
		selectCursor.moveBegin();
	    }
	    layoutCacheInvalidate |= DwTextLayoutEngine::Cursor;
	    layoutCacheInvalidate |= DwTextLayoutEngine::Select;
	    break;
        case VK_LEFT:
            deal = true;
            cursor.moveWorldLeft();
            layoutCacheInvalidate |= DwTextLayoutEngine::Cursor;
            layoutCacheInvalidate |= DwTextLayoutEngine::Select;
            break;
        case VK_RIGHT:
            deal = true;
            cursor.moveWorldRight();
            layoutCacheInvalidate |= DwTextLayoutEngine::Cursor;
            layoutCacheInvalidate |= DwTextLayoutEngine::Select;
            break;
        default:
            break;
        }

        if(!evt->isShfitDown() && deal)
        {
            selectCursor.moveTo(cursor.cur());
            layoutCacheInvalidate |= DwTextLayoutEngine::Select;
        }
    }
    else
    {
        bool deal = false;
        switch(evt->keyCode())
        {
        case VK_LEFT:
            deal = true;
            if (selectCursor.cur() != cursor.cur() && !evt->isShfitDown())
            {
                cursor.moveTo(dwMin<int>(selectCursor.cur(), cursor.cur()));
            }
            else
            {
                cursor.moveLeft();
            }
            layoutCacheInvalidate |= DwTextLayoutEngine::Cursor;
            layoutCacheInvalidate |= DwTextLayoutEngine::Select;
            break;
        case VK_RIGHT:
            deal = true;
            if (selectCursor.cur() != cursor.cur() && !evt->isShfitDown())
            {
                cursor.moveTo(dwMax<int>(selectCursor.cur(), cursor.cur()));
            }
            else
            {
                cursor.moveRight();
            }
            layoutCacheInvalidate |= DwTextLayoutEngine::Cursor;
            layoutCacheInvalidate |= DwTextLayoutEngine::Select;
            break;
        case VK_UP:
            deal = true;
            cursor.moveUp();
            layoutCacheInvalidate |= DwTextLayoutEngine::Cursor;
            layoutCacheInvalidate |= DwTextLayoutEngine::Select;
            break;
        case VK_DOWN:
            deal = true;
            cursor.moveDown();
            layoutCacheInvalidate |= DwTextLayoutEngine::Cursor;
            layoutCacheInvalidate |= DwTextLayoutEngine::Select;
            break;
        case VK_HOME:
            deal = true;
            cursor.moveBegin();
            layoutCacheInvalidate |= DwTextLayoutEngine::Cursor;
            layoutCacheInvalidate |= DwTextLayoutEngine::Select;
            break;
        case VK_END:
            deal = true;
            cursor.moveEnd();
            layoutCacheInvalidate |= DwTextLayoutEngine::Cursor;
            layoutCacheInvalidate |= DwTextLayoutEngine::Select;
            break;
        case VK_DELETE:
            if (!m_edit->isReadOnly())
            {
                deal = true;
                {
                    if (selectCursor.cur() != cursor.cur())
                    {
                        clearSelect();
                    }
                    else if (cursor.cur() < doc.length())
                    {
                        doc.remove(cursor.cur());
                    }
                }
                layoutCacheInvalidate |= DwTextLayoutEngine::ALL;
            }
            break;
        default:
            break;
        }

        if(!evt->isShfitDown() && deal)
        {
            selectCursor.moveTo(cursor.cur());
            layoutCacheInvalidate |= DwTextLayoutEngine::Select;
        }
    }

    if (cur != cursor.cur() || selectCur != selectCursor.cur())
    {
        m_edit->editText()->layoutEngine().invalidateLayoutCache(layoutCacheInvalidate);
        m_edit->update();
        _dw_update_cursor();
    }
    else
    {
        if (layoutCacheInvalidate & DwTextLayoutEngine::Select)
        {
            m_edit->editText()->layoutEngine().invalidateLayoutCache(layoutCacheInvalidate);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwSimpleEditEventHandler::onCharEvent(DwCharEvent* evt)
{
    DW_UNUSED(evt);
    supreClass::onCharEvent(evt);
    // Simplely Append Char
    DwTextDocument &doc = m_edit->editText()->doc();
    DwTextCursor &cursor = doc.cursor();
    DwTextCursor &selectCursor = doc.selectCursor();
    int layoutCacheInvalidate = 0;

    if (evt->m_characterCode
            && !DwKeyEvent::isKeyModifierDown(Dw::CTRL)
            && !DwKeyEvent::isKeyModifierDown(Dw::ALT))// utf16
    {
        if (evt->m_virtualKey == VK_TAB ||
                evt->m_virtualKey == VK_RETURN
                || m_edit->isReadOnly())
        {
            return;
        }
        int cur = cursor.cur();
        int selectCur = selectCursor.cur();

        if (evt->m_virtualKey == VK_BACK)
        {
            if (selectCursor.cur() != cursor.cur())
            {
                clearSelect();
            }
            else if (cursor.cur() > 0)
            {
                doc.remove(cursor.cur()-1);
                cursor.moveLeft();
                selectCursor.moveTo(cursor.cur());
            }
            layoutCacheInvalidate |= DwTextLayoutEngine::ALL;
        }
        else
        {

            bool isOK = doc.insert(DwRichChar(wchar_t(evt->m_characterCode)), cursor.cur());
            if (isOK)
            {
                if (selectCursor.cur() > cursor.cur())
                {
                    selectCursor.moveRight();
                    cursor.moveRight();
                    clearSelect();
                }
                else
                {
                    clearSelect();
                    cursor.moveRight();
                }
                selectCursor.moveTo(cursor.cur());
                layoutCacheInvalidate |= DwTextLayoutEngine::ALL;
            }
        }
        if (cursor.cur() != cur || selectCursor.cur() != selectCur)
        {
            m_edit->editText()->layoutEngine().invalidateLayoutCache(layoutCacheInvalidate);
            m_edit->update();
            _dw_update_cursor();
        }
    }
}

#if DW_ENABLE_EVENT_FOCUS
//------------------------------------------------------------------------------
/**
*/
void DwSimpleEditEventHandler::onFocusIn(DwFocusInEvent* evt)
{
    DW_UNUSED(evt);
    supreClass::onFocusIn(evt);
    dwInputFocusPainter()->onFocusIn(m_edit);
}

//------------------------------------------------------------------------------
/**
*/
void DwSimpleEditEventHandler::onFocusOut(DwFocusOutEvent *evt)
{
    DW_UNUSED(evt);
    supreClass::onFocusOut(evt);
    dwInputFocusPainter()->onFocusOut(m_edit);
}
#endif

//------------------------------------------------------------------------------
/**
*/
void DwSimpleEditEventHandler::onMouseClicked( DwMouseClickedEvent* evt )
{
    DW_UNUSED(evt);
    supreClass::onMouseClicked(evt);
    _dw_update_cursor();
}

//------------------------------------------------------------------------------
/**
*/
void DwSimpleEditEventHandler::onMouseDown(DwMouseDownEvent* evt)
{
    DW_UNUSED(evt);
    supreClass::onMouseDown(evt);
    if ( m_dbClick ) return;
    m_mouseDown = true;

    // Simplely Append Char
    evt->translatedToItem(m_edit->editText());
    DwPoint itemPoint = evt->m_itemPoint - m_edit->editText()->offset();
    DwTextDocument &doc = m_edit->editText()->doc();
    DwTextCursor &cursor = doc.cursor();
    DwTextCursor &selectCursor = doc.selectCursor();
    int layoutCacheInvalidate = 0;

    int num = m_edit->editText()->layoutEngine().numOfPrintChar(&doc, m_edit->editText()->boundingRect(), itemPoint);

    if (num != cursor.cur())
    {
        cursor.moveTo(num);
        if (!DwKeyEvent::isKeyModifierDown(Dw::SHIFT))
        {
            selectCursor.moveTo(cursor.cur());
        }
        layoutCacheInvalidate |= DwTextLayoutEngine::Cursor;
        layoutCacheInvalidate |= DwTextLayoutEngine::Select;

        m_edit->editText()->layoutEngine().invalidateLayoutCache(layoutCacheInvalidate);
        m_edit->update();
        _dw_update_cursor();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwSimpleEditEventHandler::onMouseUp(DwMouseUpEvent* evt)
{
    DW_UNUSED(evt);
    supreClass::onMouseUp(evt);
    m_mouseDown = false;
    m_dbClick = false;
}

//------------------------------------------------------------------------------
/**
*/
void DwSimpleEditEventHandler::onMouseMoved(DwMouseMoveEvent* evt)
{
    supreClass::onMouseMoved(evt);
    if (m_mouseDown)
    {
        evt->translatedToItem(m_edit->editText());
        DwPoint itemPoint = evt->m_itemPoint - m_edit->editText()->offset();
        DwTextDocument &doc = m_edit->editText()->doc();
        DwTextCursor &cursor = doc.cursor();
        int layoutCacheInvalidate = 0;

        int num = m_edit->editText()->layoutEngine().numOfPrintChar(&doc, m_edit->editText()->boundingRect(), itemPoint);

        if (num != cursor.cur())
        {
            cursor.moveTo(num);
            layoutCacheInvalidate |= DwTextLayoutEngine::Cursor;
            layoutCacheInvalidate |= DwTextLayoutEngine::Select;

            m_edit->editText()->layoutEngine().invalidateLayoutCache(layoutCacheInvalidate);
            m_edit->update();
            _dw_update_cursor();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwSimpleEditEventHandler::onMouseDbClicked(DwMouseDbClickEvent* evt)
{
    DW_UNUSED(evt);
    supreClass::onMouseDbClicked(evt);
    m_dbClick = true;

    DwTextDocument &doc = m_edit->editText()->doc();
    DwTextCursor &cursor = doc.cursor();
    DwTextCursor &selectCursor = doc.selectCursor();
    cursor.moveEnd();
    selectCursor.moveBegin();
    int layoutCacheInvalidate = 0;
    layoutCacheInvalidate |= DwTextLayoutEngine::Cursor;
    layoutCacheInvalidate |= DwTextLayoutEngine::Select;

    m_edit->editText()->layoutEngine().invalidateLayoutCache(layoutCacheInvalidate);
    m_edit->update();
    _dw_update_cursor();
}
