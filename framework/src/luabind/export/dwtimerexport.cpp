#include "stable.h"
#include "dwtimerexport.h"
#include "dwtimerexport_p.h"
#include "dwluabind/dwluaengine.h"

DW_IMPLEMENT_CLASS(DwTimerExport, 'TERT', DwLuaExport);

//------------------------------------------------------------------------------
static uint32_t _unique_id()
{
    static uint32_t id = 0;
    return ++id;
}

// can be 24 frames in a second
const uint32_t KLuaMinInterval = 40;

//------------------------------------------------------------------------------
class DwSTimerDeleterLater : public DwRttiObject
{
    DW_DECLARE_CLASS(DwSTimerDeleterLater);
public:
    DwSTimerDeleterLater(STimerId id = 0, lua_State * L= NULL);
    virtual ~DwSTimerDeleterLater();

    STimerId m_id;
    lua_State *m_L;
};
DW_IMPLEMENT_CLASS(DwSTimerDeleterLater, 'SDLR', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwTimerExport::DwTimerExport()
    : m_activeCount(0)
{
    m_realTimer.sigTimeout.connect(this, &DwTimerExport::onTimerOut);
    setCallExportWhen(Dw::CallWhenEngineDestroy, true);
}

//------------------------------------------------------------------------------
/**
*/
DwTimerExport::~DwTimerExport()
{
    if (m_realTimer.isActive())
    {
        m_realTimer.stop();
    }
    DW_ASSERT(m_sTimerMap.count() == 0);
}

//------------------------------------------------------------------------------
/**
*/
bool DwTimerExport::exportd( DwLuaEngine *engine )
{
    luaL_register(engine->luaState(), "Timer", timerLib);
    bool isOk = engine->load("script:ui/timer.lua");
    isOk = isOk && engine->load("script:ui/timercom.lua");
    return isOk;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTimerExport::onEngineDestroy(DwLuaEngine *e)
{
    if (m_engine = e)
    {
        STimerId2STimerMap ::iterator ite = m_sTimerMap.begin();
        while (ite != m_sTimerMap.end())
        {
            STimer &stimer = ite->second;
            _deleteTimer(stimer);
            ++ite;
        }
        m_sTimerMap.clear();
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
STimerId DwTimerExport::newTimer(uint32_t mesc)
{
    STimer stimer;
    stimer.id = _unique_id();
    stimer.interval = mesc;

    m_sTimerMap[stimer.id] = stimer;
    return stimer.id;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTimerExport::deleteTimer(STimerId id)
{
    STimerId2STimerMap ::iterator ite = m_sTimerMap.find(id);
    if (ite != m_sTimerMap.end())
    {
        STimer &stimer = ite->second;
        _deleteTimer(stimer);
        m_sTimerMap.remove(ite);

        // there is no script anymore, so we stoped polling
        if ( m_sTimerMap.count() == 0 && m_realTimer.isActive())
        {
            m_realTimer.stop();
        }

        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTimerExport::deleteTimerLater(STimerId id)
{
    STimerId2STimerMap ::iterator ite = m_sTimerMap.find(id);
    if (ite != m_sTimerMap.end())
    {
        STimer &stimer = ite->second;
        stimer.active = false;

        DwSTimerDeleterLater *stimerDeleterLater = new DwSTimerDeleterLater(stimer.id, m_engine->luaState());
        stimerDeleterLater->deleteLater();
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTimerExport::startTimer(STimerId id)
{
    STimerId2STimerMap ::iterator ite = m_sTimerMap.find(id);
    if (ite != m_sTimerMap.end())
    {
        STimer &stimer = ite->second;
        _startTimer(stimer);

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTimerExport::startTimer(STimerId id, uint32_t mesc)
{
    STimerId2STimerMap ::iterator ite = m_sTimerMap.find(id);
    if (ite != m_sTimerMap.end())
    {
        STimer &stimer = ite->second;
        _setInterval(stimer, mesc);
        _startTimer(stimer);

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTimerExport::stopTimer(STimerId id)
{
    STimerId2STimerMap ::iterator ite = m_sTimerMap.find(id);
    if (ite != m_sTimerMap.end())
    {
        STimer &stimer = ite->second;

        if (stimer.active)
        {
            stimer.active = false;
            stimer.cur = 0;

            --m_activeCount;
            if (m_activeCount <= 0 && m_realTimer.isActive())
            {
                m_realTimer.stop();
            }
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTimerExport::isActive(STimerId id) const
{
    STimerId2STimerMap ::const_iterator ite = m_sTimerMap.find(id);
    if (ite != m_sTimerMap.end())
    {
        const STimer &stimer = ite->second;

        return stimer.active;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTimerExport::setInterval(STimerId id, uint32_t mesc)
{
    STimerId2STimerMap ::iterator ite = m_sTimerMap.find(id);
    if (ite != m_sTimerMap.end())
    {
        STimer &stimer = ite->second;

        return _setInterval(stimer, mesc);
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTimerExport::connect(STimerId id)
{
    STimerId2STimerMap ::iterator ite = m_sTimerMap.find(id);
    if (ite != m_sTimerMap.end())
    {
        lua_State *L = m_engine->luaState();

        STimer &stimer = ite->second;
        SRefId idRef = luaL_ref(L, LUA_REGISTRYINDEX);
        if (idRef != LUA_REFNIL)
        {
            stimer.connects.append(idRef);
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTimerExport::disconnect(STimerId id)
{
    STimerId2STimerMap ::iterator ite = m_sTimerMap.find(id);
    if (ite != m_sTimerMap.end())
    {
        STimer &stimer = ite->second;
        return _disconnect(stimer);
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwTimerExport::onTimerOut()
{
    STimerId2STimerMap ::iterator ite = m_sTimerMap.begin();
    while (ite != m_sTimerMap.end())
    {
        STimer &stimer = ite->second;

        if (stimer.active)
        {
            stimer.cur += KLuaMinInterval;
            while(stimer.cur > stimer.interval)
            {
                stimer.cur -= stimer.interval;
                if ( ! _callTimeOut(stimer) )
                {
                    break;
                }
            }
        }
        ++ite;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwTimerExport::_deleteTimer(STimer &timer)
{
    _disconnect(timer);
}

//------------------------------------------------------------------------------
/**
*/
void DwTimerExport::_startTimer(STimer &timer)
{
    if (!timer.active)
    {
        timer.active = true;
        DW_ASSERT(timer.interval > 0);

        ++m_activeCount;
        if (!m_realTimer.isActive())
        {
            m_realTimer.start(KLuaMinInterval);
        }
    }

}

//------------------------------------------------------------------------------
/**
*/
bool DwTimerExport::_setInterval(STimer &timer, uint32_t mesc)
{
    if (timer.id)
    {
        if( timer.interval != mesc)
        {
            timer.interval = mesc;
            timer.cur = 0;

            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTimerExport::_disconnect(STimer &timer)
{
    if (timer.id > 0)
    {
        lua_State * L = m_engine->luaState();
        for (int i=0; i<timer.connects.size(); ++i)
        {
            lua_unref(L, timer.connects.at(i));
        }
        timer.connects.clear();
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTimerExport::_callTimeOut(STimer &timer)
{
    lua_State *L = m_engine->luaState();
    for (int i=0; i<timer.connects.size(); ++i)
    {
        if (timer.active && timer.id)
        {
            int n = timer.connects.at(i);
            if (n != LUA_REFNIL)
            {
                lua_rawgeti(L, LUA_REGISTRYINDEX, n);
                DW_ASSERT(lua_isfunction(L, -1));
                lua_pushnumber(L, timer.id);
                lua_pushnumber(L, timer.interval);
                int res = lua_pcall(L, 2, 0, 0);
                if (0 != res)
                {
                    DwString s = lua_tostring(L, -1);
                    DW_UI_LOGER() << "script error when script timer out: "
                                  << timer.id << " : "
                                  << timer.interval
                                  << "\n";
                    lua_pop(L, 1); // pop the error
                    DW_ASSERT(false);
                    return false;
                }
            }
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
DwSTimerDeleterLater::DwSTimerDeleterLater(STimerId id, lua_State *L)
{
    m_id = id;
    m_L = L;
}

//------------------------------------------------------------------------------
/**
*/
DwSTimerDeleterLater::~DwSTimerDeleterLater()
{
    dwLuaTimerLib()->deleteTimer(m_id, m_L);
}