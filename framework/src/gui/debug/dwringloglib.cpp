#include "stable.h"
#include "dwringloglib.h"
#include "dwringlog.h"

DW_IMPLEMENT_CLASS(DwRingLogLib, 'RLLB', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwRingLogLib::DwRingLogLib()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwRingLogLib::~DwRingLogLib()
{
    DwCSLocker locks(&m_section);

    DwMap<DWORD, LogContext>::iterator ite = m_logs.begin();
    while(ite != m_logs.end())
    {
        LogContext& context = ite->second;
        if (context.owner && context.log)
        {
            DW_SAFE_DELETE(context.log);
            context.owner = false;
        }
        ++ite;
    }
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwRingLog*> DwRingLogLib::logs()const
{
    DwCSLocker locks(&m_section);
    DwVector<DwRingLog*> ls;
    DwMap<DwRingLog*, DWORD>::const_iterator ite = m_logsId.begin();
    while(ite != m_logsId.end());
    {
        ls.append((DwRingLog*)ite->first);
        ++ite;
    }
    return ls;
}

//------------------------------------------------------------------------------
/**
*/
DwMap<DWORD, DwString> DwRingLogLib::logContents()const
{
    DwCSLocker locks(&m_section);
    DwMap<DWORD, DwString> contents;

    DwMap<DwRingLog*, DWORD>::const_iterator ite = m_logsId.begin();
    while(ite != m_logsId.end())
    {
        DwString str;
        ite->first->dumpToString(str);
        contents[ite->second] = str;
        ++ite;
    }
    return contents;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwRingLogLib::logContent(DWORD id)const
{
    DwCSLocker locks(&m_section);

    DwString str;
    DwRingLog *l = m_logs.value(id, LogContext()).log;
    if (l)
    {
        l->dumpToString(str);
    }
    return str;
}

//------------------------------------------------------------------------------
/**
*/
DwRingLog *DwRingLogLib::currentLog()const
{
    DWORD id = ::GetCurrentThreadId();

    DwCSLocker locks(&m_section);
    return m_logs.value(id, LogContext()).log;
}

//------------------------------------------------------------------------------
/**
*/
void DwRingLogLib::appendLog(DwRingLog* l, bool owner)
{
    DWORD id = ::GetCurrentThreadId();
    appendLog(id, l, owner);
}

//------------------------------------------------------------------------------
/**
*/
void DwRingLogLib::appendLog(DWORD id, DwRingLog* l, bool owner)
{
    LogContext context(l, owner);

    DwCSLocker locks(&m_section);
    DW_ASSERT(!m_logs.contains(id));
    m_logs[id] = context;
    m_logsId[l] = id;
}

//------------------------------------------------------------------------------
/**
*/
void DwRingLogLib::removeLog(DwRingLog* l)
{
    DW_ASSERT(l);
    DwCSLocker locks(&m_section);

    DWORD InvaidId = (DWORD)(-1);
    DWORD id = m_logsId.value(l, InvaidId);
    DW_ASSERT(id != InvaidId);
    m_logsId.remove(l);
    m_logs.remove(id);
}

//------------------------------------------------------------------------------
/**
*/
DwRingLog* DwRingLogLib::allocLog(int size/* = 4096*/)
{
    DWORD id = ::GetCurrentThreadId();
    return allocLog(id, size);
}

//------------------------------------------------------------------------------
/**
*/
DwRingLog* DwRingLogLib::allocLog(DWORD id, int size/* = 4096*/)
{
    DwRingLog *l = new DwRingLog(size);

    appendLog(id, l, true);
    return l;
}

//------------------------------------------------------------------------------
/**
*/
DwRingLog *DwRingLogLib::log()
{
    DwRingLog * l = currentLog();
    if (!l)
    {
        return allocLog();
    }
    return l;
}