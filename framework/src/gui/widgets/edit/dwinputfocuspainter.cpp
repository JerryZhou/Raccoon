#include "stable.h"
#include "dwinputfocuspainter.h"
#include "dwgui/dwgraphicsitem.h"
#include "dwgraphics/dwpainter.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwInputFocusPainter, 'IFPR', DwRttiObject);

const int KDeltaTick = 600;
//------------------------------------------------------------------------------
/**
*/
DwInputFocusPainter::DwInputFocusPainter()
{
    m_show = false;
    m_showTick = KDeltaTick;
    m_showTimer.sigTimeout.connect(this, &DwInputFocusPainter::onShowTime);
}

//------------------------------------------------------------------------------
/**
*/
DwInputFocusPainter::~DwInputFocusPainter()
{
    m_showTimer.stop();
}

//------------------------------------------------------------------------------
/**
*/
bool DwInputFocusPainter::onFocusIn( DwGraphicsItem *item )
{
    m_show = false;
    m_focusItem = item;
    if (!m_showTimer.isActive())
    {
        m_showTick = KDeltaTick;
        m_showTimer.start(KDeltaTick);
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwInputFocusPainter::onFocusOut( DwGraphicsItem * )
{
    if (m_showTimer.isActive())
    {
        m_showTimer.stop();

    }
    if (m_focusItem)
    {
        m_focusItem->update();
    }
    m_focusItem = NULL;
    return true;
}

void DwInputFocusPainter::onShowTime()
{
    if (m_focusItem && m_focusItem->hasFocus())
    {
        m_focusItem->update();
        m_showTick = KDeltaTick;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwInputFocusPainter::paint(DwPainter *painter, DwGraphicsItem * item, const DwPointF &from, const DwPointF &to, const DwColor &c)
{
    if (item && (m_focusItem == item || m_focusItem == item->parentItem() || !item->parentItem())
            && m_focusItem
            && m_showTick == KDeltaTick)
    {
        if (!m_show && m_focusItem && m_focusItem->hasFocus())
        {
            painter->drawLine(from.toPoint(), to.toPoint(), c);
        }
        m_showTick = 0;
        m_show = !m_show;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void DwInputFocusPainter::setNeedShowIt(bool b/* = true*/)
{
    if (b)
    {
        if (m_showTick != KDeltaTick)
        {
            m_showTick = KDeltaTick;
            m_show = false;
            m_showTimer.stop();
            m_showTimer.start(KDeltaTick);
        }
    }
    else
    {
        m_show = true;
        m_showTick = 0;
        m_showTimer.stop();
    }
}

//------------------------------------------------------------------------------
/**
*/
DW_GLOBAL_STATIC(DwInputFocusPainter, _dw_inputFocusPainter);
DW_GUI_EXPORT DwInputFocusPainter * dwInputFocusPainter()
{
    return _dw_inputFocusPainter();
}