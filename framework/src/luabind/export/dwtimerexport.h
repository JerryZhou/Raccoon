#pragma once
#include "dwluabind/dwluaexport.h"
#include "dwgui/dwtimer.h"

//------------------------------------------------------------------------------
class DwSTimerDeleterLater;
typedef uint32_t STimerId;
typedef int SRefId;

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwTimerExport : public DwLuaExport, public sigslot::has_slots
{
    DW_DECLARE_CLASS(DwTimerExport)
public:
    DwTimerExport();
    virtual ~DwTimerExport();

    /// export
    virtual bool exportd(DwLuaEngine *engine);

    /// listenmer
    virtual bool onEngineDestroy(DwLuaEngine *e);

    /// new
    STimerId newTimer(uint32_t mesc);
    /// delete
    bool deleteTimer(STimerId id);
    bool deleteTimerLater(STimerId id);
    /// start timer
    bool startTimer(STimerId id);
    bool startTimer(STimerId id, uint32_t mesc);
    /// stop the timer, can be start again
    bool stopTimer(STimerId id);
    /// is timer active
    bool isActive(STimerId id) const;
    /// set interval
    bool setInterval(STimerId id, uint32_t mesc);
    /// connect the function to timer
    bool connect(STimerId id);
    /// disconnect the timer
    bool disconnect(STimerId id);

protected:
    void onTimerOut();

    struct STimer
    {
        STimerId id;

        bool active;
        uint32_t interval;
        uint32_t cur;

        DwVector<SRefId> connects;

        STimer()
        {
            id = 0;
            interval = 0;
            cur = 0;
            active = false;
        }
    };
    typedef DwMap<STimerId, STimer> STimerId2STimerMap;

    void _deleteTimer(STimer &timer);
    void _startTimer(STimer &timer);
    bool _setInterval(STimer &timer, uint32_t mesc);
    bool _disconnect(STimer &timer);
    bool _callTimeOut(STimer &timer);

private:
    friend class DwSTimerDeleterLater;

    STimerId2STimerMap m_sTimerMap;
    DwTimer m_realTimer;
    int m_activeCount;
};// end of DwTimerExport
DW_REGISTER_CLASS(DwTimerExport);