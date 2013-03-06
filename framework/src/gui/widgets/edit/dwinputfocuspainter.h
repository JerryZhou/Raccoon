#pragma once
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwrttiobjectptr.h"
#include "dwcore/dwpoint.h"
#include "dwgraphics/dwcolor.h"
#include "dwgui/dwtimer.h"
#include "dwcore/sigslot.h"

//------------------------------------------------------------------------------
class DwGraphicsItem;
class DwPainter;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwInputFocusPainter : DwRttiObject, public sigslot::has_slots
{
    DW_DECLARE_CLASS(DwInputFocusPainter);
public:
    DwInputFocusPainter();
    virtual ~DwInputFocusPainter();

    bool onFocusIn(DwGraphicsItem *item);
    bool onFocusOut(DwGraphicsItem *item);
    void onShowTime();

    bool paint(DwPainter *painter, DwGraphicsItem *item, const DwPointF &from, const DwPointF &to, const DwColor &c);

    void setNeedShowIt(bool b = true);

protected:
    DwRttiObjectPtr<DwGraphicsItem> m_focusItem;
    DwTimer m_showTimer;
    bool m_show;
    int m_showTick;
};// end of DwInputFocusPainter
DW_REGISTER_CLASS(DwInputFocusPainter);

//------------------------------------------------------------------------------
DW_GUI_EXPORT DwInputFocusPainter *dwInputFocusPainter();