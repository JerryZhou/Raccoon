#pragma once

#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class DwRingLog;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwRingLogLib : public DwRttiObject
{
    DW_DECLARE_CLASS(DwRingLogLib);
public:
    /// constructor
    DwRingLogLib();
    /// deconstructor
    virtual ~DwRingLogLib();
    /// return all the logs
    DwVector<DwRingLog*> logs()const;
    /// return all the log content
    DwMap<DWORD, DwString> logContents()const;
    /// return the content of id
    DwString logContent(DWORD id)const;
    /// return current thread log
    DwRingLog *currentLog()const;
    /// return current thread log, if do not exits will create it
    DwRingLog *log();

    /// return the instance of lib
    DW_GLOBAL_STATIC(DwRingLogLib, Instance);

protected:
    /// append log to current thread
    void appendLog(DwRingLog* l, bool owner);
    /// append log to thread id
    void appendLog(DWORD id, DwRingLog* l, bool owner);
    /// remove corresponding log 
    void removeLog(DwRingLog* l);
    /// alloc log for current thread
    DwRingLog* allocLog(int size = 4096*4);
    /// alloc log for corresponding thread
    DwRingLog* allocLog(DWORD id, int size = 4096*4);

    struct LogContext 
    {
        DwRingLog* log;
        bool owner;
        LogContext(): log(NULL), owner(false){}
        LogContext(DwRingLog *l, bool o): log(l), owner(o){}
    };

private:
    DwMap<DWORD, LogContext>  m_logs;
    DwMap<DwRingLog*, DWORD> m_logsId;
    DwCriticalSection m_section;
};
