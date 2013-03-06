#pragma once
#include "dwgui/dwevent.h"
#include "dwgui/dweventid.h"
#include "dwgui/dweventdispatch.h"

//------------------------------------------------------------------------------
class DwAppEventDispatchData;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAppEventDispatch : public DwEventDispatch
{
public:
    DwAppEventDispatch();
    virtual ~DwAppEventDispatch();

    void dispatchEvent(DwEvent *evt);

    template<typename T>
    void dispatchEventAsync(T *evt, int sec = 0);

protected:
    void dispatchEventAsyncHelper(DwEvent *evt, int sec = 0);

    DwAppEventDispatchData *d;
};// end of DwAppEventDispatch

//------------------------------------------------------------------------------
DW_GUI_EXPORT DwAppEventDispatch *dwAppEventDispatch();

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline void DwAppEventDispatch::dispatchEventAsync(T *evt, int sec/* = 0*/)
{
    DW_ASSERT(evt);
    DW_ASSERT(evt->isA(DwEvent::RTTI()));
    T * asyncEvt = new T (*evt);
    dispatchEventAsyncHelper(asyncEvt, sec);
}