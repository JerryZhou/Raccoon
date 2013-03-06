#include "stable.h"
#include "dwdummys.h"

// Used to expand the dwstring and the path
//////////////////////////////////////////////////////////////////////////
DwString dwExtractToEnd(const DwString &str, IndexT pos)
{
    return str.right(str.size() - pos);
}

//------------------------------------------------------------------------------
/**
*/
void dwTerminateAtIndex(DwString &str, IndexT pos)
{
    str.resize(pos);
}

//------------------------------------------------------------------------------
/**
*/
void dwTrimRight(DwString &str, wchar_t c)
{
    int index = str.size() - 1;
    while(index >= 0 && str.at(index) ==  c)
    {
        --index;
    }

    str.resize(index + 1);
}

//------------------------------------------------------------------------------
/**
*/
void dwConvertBackslashes(DwString &str)
{
    str.replace(L"\\", L"/");
}

//------------------------------------------------------------------------------
/**
*/
int dwLastSlashIndex(DwString &str, int from/* = -1*/)
{
    IndexT index = str.lastIndexOf(L'/', from);
    if(index == InvalidIndex) index = str.lastIndexOf(L'\\', from);
    if(index == InvalidIndex) index = str.lastIndexOf(L':', from);
    return index;
}

//------------------------------------------------------------------------------
/**
*/
DwString dwExtractDirName(const DwString &str)
{
    DwString pathString(str);

    IndexT index = dwLastSlashIndex(pathString);
    if (index != InvalidIndex)
    {
        if (pathString.at(index+1) == 0)
        {
            dwTerminateAtIndex(pathString, index);
            index = dwLastSlashIndex(pathString, index);
        }
    }

    if (index != InvalidIndex)
    {
        dwTerminateAtIndex(pathString, index);
    }

    return pathString;
}

//------------------------------------------------------------------------------
/**
*/
DwString dwExtractLastDirName(const DwString &str)
{
    DwString pathString(str);
    IndexT index = dwLastSlashIndex(pathString);

    if (index != InvalidIndex)
    {
        if (pathString.at(index + 1) == 0)
        {
            dwTerminateAtIndex(pathString, index);
            index = dwLastSlashIndex(pathString);
        }

        if (index != InvalidIndex)
        {
            dwTerminateAtIndex(pathString, index); // cut filename

            index = dwLastSlashIndex(pathString);

            if (index != InvalidIndex)
            {
                return dwExtractToEnd(pathString, index + 1);
            }
        }
    }
    return L"";
}

DwVector<DwString> dwTokenize(const DwString& inStr, const DwString& whiteSpace, wchar_t fence)
{
    DwVector<DwString> outTokens;
    DwString str(inStr);
    str.append(' ');
    str.remove(str.length()-1, 1);

    wchar_t* ptr = const_cast<wchar_t*>(str.constString());
    wchar_t* end = ptr + str.length();
    while (ptr < end)
    {
        wchar_t* c;

        // skip white space
        while (*ptr && wcschr(whiteSpace.constString(), *ptr))
        {
            ptr++;
        }
        if (*ptr)
        {
            // check for fenced area
            if ((fence == *ptr) && (0 != (c = wcschr(++ptr, fence))))
            {
                *c++ = 0;
                outTokens.append(ptr);
                ptr = c;
            }
            else if (0 != (c = wcspbrk(ptr, whiteSpace.constString())))
            {
                *c++ = 0;
                outTokens.append(ptr);
                ptr = c;
            }
            else
            {
                outTokens.append(ptr);
                break;
            }
        }
    }
    return outTokens;
}

DwVector<DwString> dwTokenize(const DwString& inStr, const DwString& whiteSpace)
{
    DwVector<DwString> outTokens;
    DwString str(inStr);
    str.append(' ');
    str.remove(str.length()-1, 1);

    wchar_t* ptr = const_cast<wchar_t*>(str.constString());
    const wchar_t* token;
    while (0 != (token = wcstok(ptr, whiteSpace.constString())))
    {
        outTokens.append(token);
        ptr = 0;
    }
    return outTokens;
}

bool dwCheckValidCharSet(const DwString &str, const DwString &charSet)
{
    IndexT i;
    for (i = 0; i < str.length(); i++)
    {
        if (InvalidIndex == charSet.indexOf(str.at(i), 0))
        {
            return false;
        }
    }
    return true;
}

bool dwMatchPattern(const DwString &strr, const DwString &pattern)
{
    const wchar_t* str = strr.constString();
    const wchar_t* pat = pattern.constString();

    wchar_t c2;
    bool always = true;
    while (always != false)
    {
        if (*pat == 0) 
        {
            if (*str == 0) return true;
            else           return false;
        }
        if ((*str == 0) && (*pat != '*')) return false;
        if (*pat=='*') 
        {
            pat++;
            if (*pat==0) return true;
            while (always)
            {
                if (dwMatchPattern(str, pat)) return true;
                if (*str==0) return false;
                str++;
            }
        }
        if (*pat=='?') goto match;
        if (*pat=='[') 
        {
            pat++;
            while (always) 
            {
                if ((*pat==']') || (*pat==0)) return false;
                if (*pat==*str) break;
                if (pat[1] == '-') 
                {
                    c2 = pat[2];
                    if (c2==0) return false;
                    if ((*pat<=*str) && (c2>=*str)) break;
                    if ((*pat>=*str) && (c2<=*str)) break;
                    pat+=2;
                }
                pat++;
            }
            while (*pat!=']') 
            {
                if (*pat==0) 
                {
                    pat--;
                    break;
                }
                pat++;
            }
            goto match;
        }

        if (*pat=='\\') 
        {
            pat++;
            if (*pat==0) return false;
        }
        if (*pat!=*str) return false;

match:
        pat++;
        str++;
    }
    // can't happen
    return false;
}

//////////////////////////////////////////////////////////////////////////

// The Max Path Length
//////////////////////////////////////////////////////////////////////////
#define DW_GUI_MAX_PATH 512
//////////////////////////////////////////////////////////////////////////

// Max Event Dispatch Deep
//////////////////////////////////////////////////////////////////////////
#define DW_GUI_MAX_EVENT_LEVEL 32
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void __cdecl dwStringFormat(DwString &str, const char* fmtString, ...)
{
    va_list argList;
    va_start(argList, fmtString);
    dwStringFormatFromArgList(str, fmtString, argList);
    va_end(argList);
}

void __cdecl dwStringFormatFromArgList(DwString &str, const char* fmtString, va_list argList)
{
    char buf[4096]; // an 4 kByte buffer
#if WIN32
    // need to use non-CRT thread safe function under Win32
    StringCchVPrintfA(buf, sizeof(buf), fmtString, argList);
#else
    _vsnprintf(buf, sizeof(buf), fmtString, argList);
#endif
    str = DwString::fromUtf8(buf);
}

void __cdecl dwStringFormat(DwString &str, const wchar_t* fmtString, ...)
{
    va_list argList;
    va_start(argList, fmtString);
    dwStringFormatFromArgList(str, fmtString, argList);
    va_end(argList);
}

void __cdecl dwStringFormatFromArgList(DwString &str, const wchar_t* fmtString, va_list argList)
{
    wchar_t buf[4096]; // an 4 kByte buffer
#if WIN32
    // need to use non-CRT thread safe function under Win32
    StringCchVPrintfW(buf, sizeof(buf), fmtString, argList);
#else
    _vsnwprintf(buf, sizeof(buf), fmtString, argList);
#endif
    str = DwString::fromWCharArray(buf);
}
//////////////////////////////////////////////////////////////////////////

int dwRandom()
{
    return ::rand();
}

int dwRandom(int from, int to)
{
    if (from != to)
    {
        return dwRandom();
    }

    return from + rand()%(abs(to-from));
}

float dwRandomF(float from, float to)
{
    return from + dwRandom()*(to-from)/RAND_MAX;
}