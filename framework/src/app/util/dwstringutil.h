#pragma once
#include "dwcore/dwstring.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwStringUtil
{
public:
    static DwString extractToEnd(const DwString &str, int pos);

    static void terminateAtIndex(DwString &str, int pos);

    static void trimRight(DwString &str, wchar_t c);

    static void convertBackslashes(DwString &str);

    static int lastSlashIndex(DwString &str, int from = -1);

    static DwString extractDirName(const DwString &str);

    static DwString extractLastDirName(const DwString &str);

    static DwVector<DwString> tokenize(const DwString& inStr, const DwString& whiteSpace, wchar_t fence);

    static DwVector<DwString> tokenize(const DwString& inStr, const DwString& whiteSpace);

    static bool checkValidCharSet(const DwString &str, const DwString &charSet);

    static bool matchPattern(const DwString &str, const DwString &pattern);

    static void __cdecl stringFormat(DwString &str, const char* fmt, ...);
    
    static void __cdecl stringFormatFromArgList(DwString &str, const char* fmt, va_list argList);
    
    static void __cdecl stringFormat(DwString &str, const wchar_t* fmt, ...);
    
    static void __cdecl stringFormatFromArgList(DwString &str, const wchar_t* fmt, va_list argList);
};// end of DwStringUtil