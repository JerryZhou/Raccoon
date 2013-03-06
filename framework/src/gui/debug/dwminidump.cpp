#include "stable.h"
#include <DbgHelp.h>
#include "dwminidump.h"
#include "dwguiconfig.h"
#include "graphicsview/dwdummys.h"
#include "debug/dwringlogcase.h"
#include "dwgui/dwurl.h"
#include "dwgui/dwapplication.h"

#if DW_ENABLE_MINIDUMPS
#pragma comment(lib, "dbghelp.lib")
#endif

struct DwDumphelper
{
    DwString appLocalPath;
    DwString dumpPath;
    DwDumphelper()
    {
        DwUrl appData("appdata:");
        appLocalPath = appData.localPath();
    }

    void initDumpDirectory()
    {
        if (dumpPath.isEmpty() || dumpPath.isNull())
        { 
            dumpPath = initDirectory(appLocalPath.constString(), dwApp()->companyName());
            dumpPath = initDirectory(dumpPath, dwApp()->appName());
            dumpPath = initDirectory(dumpPath, "CrashReports");
        }
    }

    DwString initDirectory(DwString p, DwString sub)
    {
        DwString strAppPath;
        dwStringFormat(strAppPath, L"%s/%s", p.constString(), sub.constString());
        ::CreateDirectory(strAppPath.constString(), NULL);
        return strAppPath;
    }
};
DW_GLOBAL_STATIC(DwDumphelper, _dw_dumpHelper);
//------------------------------------------------------------------------------
/**
*/
void DwMiniDump::setup()
{
#if DW_ENABLE_MINIDUMPS
    ::SetUnhandledExceptionFilter(DwMiniDump::ExceptionCallback);
    _dw_dumpHelper();
#endif
}

//------------------------------------------------------------------------------
/**
*/
bool DwMiniDump::writeMiniDump()
{
    return DwMiniDump::writeMiniDumpInternal(0);
}

//------------------------------------------------------------------------------
/**
*/
bool DwMiniDump::writeMiniDumpInternal( EXCEPTION_POINTERS* exceptionInfo )
{
    DW_UNUSED(exceptionInfo);
#if (DW_ENABLE_MINIDUMPS)
    DwString dumpFilename = buildMiniDumpFilename();
    if (!dumpFilename.isEmpty())
    {
        DwString logFileName = dumpFilename + L".lg.txt";
        DW_UI_LOG_FLUSH(logFileName);

        HANDLE hFile = ::CreateFileW(dumpFilename.constString(), // lpFileName
                                     GENERIC_WRITE,            // dwDesiredAccess
                                     FILE_SHARE_READ,          // dwShareMode
                                     0,                        // lpSecurityAttributes
                                     CREATE_ALWAYS,            // dwCreationDisposition,
                                     FILE_ATTRIBUTE_NORMAL,    // dwFlagsAndAttributes
                                     NULL);                    // hTemplateFile
        if (NULL != hFile)
        {
            HANDLE hProc = ::GetCurrentProcess();
            DWORD procId = ::GetCurrentProcessId();
            BOOL res = FALSE;
            if (NULL != exceptionInfo)
            {
                // extended exception info is available
                MINIDUMP_EXCEPTION_INFORMATION extInfo = { 0 };
                extInfo.ThreadId = GetCurrentThreadId();
                extInfo.ExceptionPointers = exceptionInfo;
                extInfo.ClientPointers = TRUE;
                res = ::MiniDumpWriteDump(hProc, procId, hFile, MiniDumpNormal, &extInfo, NULL, NULL);
            }
            else
            {
                // extended exception info is not available
                res = ::MiniDumpWriteDump(hProc, procId, hFile, MiniDumpNormal, NULL, NULL, NULL);
            }
            ::CloseHandle(hFile);
            return true;
        }
    }
#endif
    return false;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwMiniDump::buildMiniDumpFilename()
{
    DwString dumpFilename;

    // get our module filename directly from windows
    TCHAR buf[_MAX_PATH] = { 0 };
    DWORD numBytes = ::GetModuleFileName(NULL, buf, sizeof(buf) - 1);
    if (numBytes > 0)
    {
        TCHAR dirver[_MAX_DRIVE] = { 0 };
        TCHAR dir[_MAX_DIR] = { 0 };
        TCHAR fileName[_MAX_FNAME] = { 0 };

        errno_t error = _wsplitpath_s(buf, dirver, _MAX_DRIVE, dir, _MAX_DIR, fileName, _MAX_FNAME, NULL, 0);
        DW_ASSERT( 0 == error);
        DW_UNUSED(error);

        DwString modulePath(dirver);
        modulePath.append(dir);
        DwString moduleName(fileName);

        // get the current calender time
        SYSTEMTIME t;
        ::GetLocalTime(&t);
        DwString timeStr;
        dwStringFormat(timeStr, L"%04d-%02d-%02d_%02d-%02d-%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);

        // build the dump filename
        // appdata/appname/modulename_timestr.dump
        _dw_dumpHelper()->initDumpDirectory();
        dwStringFormat(dumpFilename, L"%s/%s_%s.dmp", _dw_dumpHelper()->dumpPath.constString(), moduleName.constString(), timeStr.constString());
//#ifdef _DEBUG
//        // modulepath/modulename_timestr.dump
          //dwStringFormat(dumpFilename, L"%s%s_%s.dmp", modulePath.constString(), moduleName.constString(), timeStr.constString());
//#endif
    }
    return dumpFilename;
}

//------------------------------------------------------------------------------
/**
*/
LONG WINAPI DwMiniDump::ExceptionCallback( EXCEPTION_POINTERS* exceptionInfo )
{
    DwMiniDump::writeMiniDumpInternal(exceptionInfo);
    return EXCEPTION_CONTINUE_SEARCH;
}
