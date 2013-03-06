#include "stable.h"
#include "dwstringutil.h"

//------------------------------------------------------------------------------
const int  InvalidIndex = -1;

//------------------------------------------------------------------------------
/**
*/
DwString DwStringUtil::extractToEnd( const DwString &str, int pos )
{
    return str.right(str.size() - pos);
}

//------------------------------------------------------------------------------
/**
*/
void DwStringUtil::terminateAtIndex( DwString &str, int pos )
{
    str.resize(pos);
}

//------------------------------------------------------------------------------
/**
*/
void DwStringUtil::trimRight( DwString &str, wchar_t c )
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
void DwStringUtil::convertBackslashes( DwString &str )
{
    str.replace(L"\\", L"/");
}

//------------------------------------------------------------------------------
/**
*/
int DwStringUtil::lastSlashIndex( DwString &str, int from /*= -1*/ )
{
    int index = str.lastIndexOf(L'/', from);
    if(index == InvalidIndex) index = str.lastIndexOf(L'\\', from);
    if(index == InvalidIndex) index = str.lastIndexOf(L':', from);
    return index;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwStringUtil::extractDirName( const DwString &str )
{
    DwString pathString(str);

    int index = lastSlashIndex(pathString);
    if (index != InvalidIndex)
    {
        if (pathString.at(index+1) == 0)
        {
            terminateAtIndex(pathString, index);
            index = lastSlashIndex(pathString, index);
        }
    }

    if (index != InvalidIndex)
    {
        terminateAtIndex(pathString, index);
    }

    return pathString;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwStringUtil::extractLastDirName( const DwString &str )
{
    DwString pathString(str);
    int index = lastSlashIndex(pathString);

    if (index != InvalidIndex)
    {
        if (pathString.at(index + 1) == 0)
        {
            terminateAtIndex(pathString, index);
            index = lastSlashIndex(pathString);
        }

        if (index != InvalidIndex)
        {
            terminateAtIndex(pathString, index); // cut filename

            index = lastSlashIndex(pathString);

            if (index != InvalidIndex)
            {
                return extractToEnd(pathString, index + 1);
            }
        }
    }
    return L"";
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwString> DwStringUtil::tokenize( const DwString& inStr, const DwString& whiteSpace, wchar_t fence )
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

//------------------------------------------------------------------------------
/**
*/
DwVector<DwString> DwStringUtil::tokenize( const DwString& inStr, const DwString& whiteSpace )
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

//------------------------------------------------------------------------------
/**
*/
bool DwStringUtil::checkValidCharSet( const DwString &str, const DwString &charSet )
{
    int i;
    for (i = 0; i < str.length(); i++)
    {
        if (InvalidIndex == charSet.indexOf(str.at(i), 0))
        {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwStringUtil::matchPattern( const DwString &strr, const DwString &pattern )
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
                if (matchPattern(str, pat)) return true;
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

//------------------------------------------------------------------------------
/**
*/
void __cdecl DwStringUtil::stringFormat( DwString &str, const char* fmt, ... )
{
    va_list argList;
    va_start(argList, fmt);
    stringFormatFromArgList(str, fmt, argList);
    va_end(argList);
}

//------------------------------------------------------------------------------
/**
*/
void __cdecl DwStringUtil::stringFormat( DwString &str, const wchar_t* fmt, ... )
{
    va_list argList;
    va_start(argList, fmt);
    stringFormatFromArgList(str, fmt, argList);
    va_end(argList);
}

//------------------------------------------------------------------------------
/**
*/
void __cdecl DwStringUtil::stringFormatFromArgList( DwString &str, const char* fmt, va_list argList )
{
    char buf[4096]; // an 4 kByte buffer
#if WIN32
    // need to use non-CRT thread safe function under Win32
    StringCchVPrintfA(buf, sizeof(buf), fmt, argList);
#else
    _vsnprintf(buf, sizeof(buf), fmt, argList);
#endif
    str = DwString::fromUtf8(buf);
}

//------------------------------------------------------------------------------
/**
*/
void __cdecl DwStringUtil::stringFormatFromArgList( DwString &str, const wchar_t* fmt, va_list argList )
{
    wchar_t buf[4096]; // an 4 kByte buffer
#if WIN32
    // need to use non-CRT thread safe function under Win32
    StringCchVPrintfW(buf, sizeof(buf), fmt, argList);
#else
    _vsnwprintf(buf, sizeof(buf), fmt, argList);
#endif
    str = DwString::fromWCharArray(buf);
}