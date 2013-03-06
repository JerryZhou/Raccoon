#include "stable.h"

#include <dwcore/dwglobalstatic.h>
#include "dwtimer.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwTimer, 'TIMR', DwRttiObject);

//------------------------------------------------------------------------------
class DwTimerService : public CWindowImpl<DwTimerService>
{
public:
    DECLARE_WND_CLASS(L"DwTimerServiceWindow")

    BEGIN_MSG_MAP(DwTimerService)
    MESSAGE_HANDLER(WM_TIMER, OnTimer)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_NCDESTROY, OnNcDestroy)
    END_MSG_MAP()

    DwTimerService()
        : m_nextTimerId(1)
    {
        ;
    }

    ~DwTimerService()
    {
        DW_ASSERT(m_timers.count() == 0 && "SHOULD STOP ALL THE TIMER BEFORE APP EXIT");
    }

    int startTimer(DwTimer *timer, int msec)
    {
        if(m_timers.count() == 0)
        {
            Create(NULL, CWindow::rcDefault, NULL, WS_POPUP);
        }
        int timerId = SetTimer(nextTimerId(), msec);
        m_timers.insert(timerId, timer);
        return timerId;
    }

    void stopTimer(int timerId)
    {
        m_timers.remove(timerId);
        KillTimer(timerId);

        if(m_timers.count() == 0)
        {
            DestroyWindow();
            m_hWnd = NULL;
        }
    }

    LRESULT OnTimer(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        DW_UNUSED(nMsg);
        DW_UNUSED(lParam);
        DW_ASSERT(nMsg == WM_TIMER);
        DW_ASSERT(m_timers.contains(wParam));

        int timerId = wParam;
        DwTimer *timer = m_timers.value(timerId);
        timer->sigTimeout.emit();

        bHandled = TRUE;

        return 0;
    }

    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        return 1;
    }

    LRESULT OnNcDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        bHandled = TRUE;

        return 0;
    }

private:
    int nextTimerId()
    {
        while(m_timers.contains(m_nextTimerId))
        {
            m_nextTimerId++;
        }

        return m_nextTimerId++;
    }

    DwMap<int, DwTimer*> m_timers;
    int m_nextTimerId;
};
DW_GLOBAL_STATIC(DwTimerService, __dwTimerService);

//------------------------------------------------------------------------------
/**
*/
DwTimer::DwTimer( DwObject *parent /*= NULL*/ )
    : DwObject(parent)
    , m_timerId(-1)
    , m_interval(-1)
{
}

//------------------------------------------------------------------------------
/**
*/
DwTimer::~DwTimer()
{
    stop();
}

//------------------------------------------------------------------------------
/**
*/
int DwTimer::interval() const
{
    return m_interval;
}

//------------------------------------------------------------------------------
/**
*/
void DwTimer::setInterval( int msec )
{
    if(isActive())
    {
        start(msec);
    }
    else
    {
        m_interval = msec;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwTimer::isActive() const
{
    return m_timerId != -1;
}

//------------------------------------------------------------------------------
/**
*/
void DwTimer::start()
{
    DW_ASSERT(m_interval != -1);
    start(m_interval);
}

//------------------------------------------------------------------------------
/**
*/
void DwTimer::start(int msec)
{
    if(m_timerId != -1)
    {
        if(m_interval == msec)
        {
            return;
        }

        stop();
    }
    m_interval = msec;
    m_timerId = __dwTimerService()->startTimer(this, m_interval);
}

//------------------------------------------------------------------------------
/**
*/
void DwTimer::stop()
{
    // called in deconstruct
    if(m_timerId == -1)
    {
        return;
    }

    __dwTimerService()->stopTimer(m_timerId);
    m_interval = -1;
    m_timerId = -1;
}

//------------------------------------------------------------------------------
/**
*/
void DwTimer::debugString(DwDebug &stream)const
{
    stream  << "DwTimer ["
            <<" Active :" << isActive()
            << " id :" << m_timerId
            << " interval: " << m_interval << "]";
}
