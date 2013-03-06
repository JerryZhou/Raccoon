#pragma once
#include "dwcore/dwstring.h"

class DW_GUI_EXPORT DwMiniDump
{
public:
    // setup the the Win32 exception callback hook
    static void setup();
    // write a mini dump
    static bool writeMiniDump();

private:
    // internal mini-dump-writer method with extra exception info
    static bool writeMiniDumpInternal(EXCEPTION_POINTERS* exceptionInfo);
    // build a filename for the dump file
    static DwString buildMiniDumpFilename();
    // the actual exception handler function called back by Windows
    static LONG WINAPI ExceptionCallback(EXCEPTION_POINTERS* exceptionInfo);
};