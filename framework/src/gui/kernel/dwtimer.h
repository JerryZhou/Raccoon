#pragma once
#include "dwcore/sigslot.h"
#include "dwcore/dwobject.h"
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwrtti.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwTimer : public DwObject , public DwRttiObject
{
    DW_DISABLE_COPY(DwTimer);
    DW_DECLARE_CLASS(DwTimer);

public:
    explicit DwTimer(DwObject *parent = NULL);
    virtual ~DwTimer();

    int interval() const;
    void setInterval(int msec);

    bool isActive() const;

    void start();
    void start(int msec);
    void stop();

    inline int timerId() const;

    virtual void debugString(DwDebug &stream)const;

public:
    sigslot::signal0<> sigTimeout;

private:
    int m_timerId;
    int m_interval;
};// end of DwTimer

//------------------------------------------------------------------------------
/**
*/
inline int DwTimer::timerId() const
{
    return m_timerId;
}