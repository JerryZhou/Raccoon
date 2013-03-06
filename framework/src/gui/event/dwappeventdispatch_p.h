#pragma once
#include "dwgui/dwtimer.h"

class DwAppEventDispatchData : public sigslot::has_slots
{
public:
    DwAppEventDispatchData()
        : m_inStop(false)
        , m_minInterval(INT_MAX)
    {
        m_timer.sigTimeout.connect(this, &DwAppEventDispatchData::onAsyncPoint);
        // solve global dependence
        m_timer.start(0);
        m_timer.stop();
    }

    virtual ~DwAppEventDispatchData()
    {
        m_timer.sigTimeout.disconnect(this);
        m_timer.stop();
    }

    void dispatch(DwEvent *evt, int sec)
    {
        if (m_inStop)
        {
            dwAppEventDispatch()->dispatchEvent(evt);
            delete evt;
            return;
        }

        m_events[evt] = sec;

        if ((sec && sec < m_minInterval) || m_minInterval == 0)
        {
            updateInterval();
        }
    }

    void updateInterval()
    {
        if (m_events.count() == 0)
        {
            m_timer.stop();
        }
        else
        {
            int newInterval = INT_MAX;
            DwMap<DwEvent *, int>::iterator ite = m_events.begin();
            while(ite != m_events.end())
            {
                if (ite->second > 0 && newInterval > ite->second)
                {
                    newInterval = ite->second;
                }
                ++ite;
            }
            if (newInterval != m_timer.interval() && newInterval != INT_MAX)
            {
                m_minInterval = newInterval;
                m_timer.start(newInterval);
            }
        }
    }

    void onAsyncPoint()
    {
        int elaspes = m_timer.interval();

        DwMap<DwEvent *, int> copyItems = m_events;
        m_events.clear();

        DwMap<DwEvent *, int>::iterator ite = copyItems.begin();
        while(ite != copyItems.end())
        {
            ite->second -= elaspes;
            if (ite->second <= 0)
            {
                dwAppEventDispatch()->dispatchEvent(ite->first);
                delete ite->first;
            }
            else
            {
                m_events[ite->first] = ite->second;
            }
            ++ite;
        }
        copyItems.clear();

        m_minInterval = 0;

        updateInterval();
    }

    void stop()
    {
        m_inStop = true;
        m_timer.stop();

        DwMap<DwEvent *, int> copyItems = m_events;
        m_events.clear();

        DwMap<DwEvent *, int>::iterator ite = copyItems.begin();
        while(ite != copyItems.end())
        {
            dwAppEventDispatch()->dispatchEvent(ite->first);
            delete ite->first;
            ++ite;
        }
        copyItems.clear();
        DW_ASSERT(m_events.count() == 0 && "SHOULD NOT DISPATCH ASYNC EVENT WHEN STOP");
    }

    void restart()
    {
        m_inStop = false;
        updateInterval();
    }

    int m_minInterval;
    DwTimer m_timer;
    DwMap<DwEvent* , int> m_events;
    bool m_inStop;
};// end of DwAppEventDispatchData