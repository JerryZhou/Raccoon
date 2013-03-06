#include "stable.h"
#include "dwmultiediteventhandler.h"
#include "dwgui/dwgraphicsframeitem.h"
#include "dwgui/dweventsignaldispatch.h"
#include "dwgui/dwclipboard.h"
#include "dwgui/dwanchors.h"
#include "dwsimpleedit.h"
#include "dwinputfocuspainter.h"
#include "dwtextdocument.h"
#include "dwrichchar.h"
#include "dwgraphicsedittext.h"
#include "dwinputfocuspainter.h"

#define _dw_update_cursor() dwInputFocusPainter()->setNeedShowIt(true);\
    updateGraphicsEditToLookCursor()

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwMultiEditEventHandler, 'MEER', DwSimpleEditEventHandler);

//------------------------------------------------------------------------------
/**
*/
DwMultiEditEventHandler::DwMultiEditEventHandler( DwSimpleEdit* edit /*= NULL*/, DwGraphicsBackgroundItem* back /*= NULL*/ )
    : DwSimpleEditEventHandler(edit, back)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwMultiEditEventHandler::onKeyDown(DwKeyDownEvent* evt)
{
    __super::onKeyDown(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwMultiEditEventHandler::onCharEvent(DwCharEvent* evt)
{
    // Simplely Append Char
    DwTextDocument &doc = m_edit->editText()->doc();
    DwTextCursor &cursor = doc.cursor();
    DwTextCursor &selectCursor = doc.selectCursor();
    int layoutCacheInvalidate = 0;
    bool deal = false;

    if (evt->m_characterCode
            && !DwKeyEvent::isKeyModifierDown(Dw::CTRL)
            && !DwKeyEvent::isKeyModifierDown(Dw::ALT)
            && !m_edit->isReadOnly())// utf16
    {
        if (evt->m_virtualKey == VK_TAB)
        {
            deal = true;
            for (int i=0; i<4; ++i)
            {
                doc.insert(DwRichChar(wchar_t(' ')), cursor.cur());
                cursor.moveRight();
                selectCursor.moveTo(cursor.cur());
            }
            layoutCacheInvalidate |= DwTextLayoutEngine::Cursor;
            layoutCacheInvalidate |= DwTextLayoutEngine::Select;
        }
        else if( evt->m_virtualKey == VK_RETURN )
        {
            deal = true;
            doc.insert(DwRichChar(wchar_t('\n')), cursor.cur());
            cursor.moveRight();
            selectCursor.moveTo(cursor.cur());
            layoutCacheInvalidate |= DwTextLayoutEngine::Cursor;
            layoutCacheInvalidate |= DwTextLayoutEngine::Select;
        }
    }

    if (deal)
    {
        m_edit->editText()->layoutEngine().invalidateLayoutCache(layoutCacheInvalidate);
        m_edit->update();
        _dw_update_cursor();
    }

    __super::onCharEvent(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwMultiEditEventHandler::updateGraphicsEditToLookCursor()
{
    ;
}