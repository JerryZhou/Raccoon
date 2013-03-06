#include "stable.h"
#include "dwprivateprofile.h"

//------------------------------------------------------------------------------
/**
*/
bool DwPrivateProfile::exists(const DwUrl& url, const DwString& section,
                              const DwString& key)
{
    int i = readInt(url, section, key);
    DwString str = readString(url, section, key);
    if (i == -1 && str == L"")
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwPrivateProfile::writeInt(const DwUrl& url, const DwString& section,
                                const DwString& key, int v)
{
    return writeString(url, section, key, DwString::number(v));
}

//------------------------------------------------------------------------------
/**
*/
bool DwPrivateProfile::writeString(const DwUrl& url, const DwString& section,
                                   const DwString& key, const DwString &v)
{
    DW_ASSERT(url.scheme() == L"file");
    DW_ASSERT(url.isValid());
    DW_ASSERT(!section.isEmpty());
    return ::WritePrivateProfileString(section.constString(),   // lpAppName
                                       key.isEmpty() ? NULL : key.constString() ,              // lpKeyName
                                       v.isEmpty() ? NULL : v.constString(),                   // lpString
                                       url.localPath().constString()) == True;
}

//------------------------------------------------------------------------------
/**
*/
bool DwPrivateProfile::writeStruct(const DwUrl& url, const DwString& section,
                                   const DwString& key, void* pStruct, int size)
{
    DW_ASSERT(url.scheme() == L"file");
    DW_ASSERT(url.isValid());
    DW_ASSERT(!section.isEmpty());
    DW_ASSERT(size > 0);
    return ::WritePrivateProfileStruct(section.constString(),   // lpszSection
                                       key.isEmpty() ? NULL : key.constString(),               // lpszKey
                                       pStruct,                                                // lpStruct
                                       (UINT)size,                                             // uSizeStruct
                                       url.localPath().constString()) == True;                 // szFile
}

//------------------------------------------------------------------------------
/**
*/
bool DwPrivateProfile::removeSection(const DwUrl& url, const DwString& section)
{
    DW_ASSERT(url.scheme() == L"file");
    DW_ASSERT(url.isValid());
    DW_ASSERT(!section.isEmpty());
    return ::WritePrivateProfileString(section.constString(),   // lpAppName
                                       NULL ,                                                  // lpKeyName
                                       NULL,                                                   // lpString
                                       url.localPath().constString()) == True;                 // szFile
}

//------------------------------------------------------------------------------
/**
*/
bool DwPrivateProfile::removeKey(const DwUrl&url, const DwString& section, const DwString& key)
{
    DW_ASSERT(url.scheme() == L"file");
    DW_ASSERT(url.isValid());
    DW_ASSERT(!section.isEmpty());
    DW_ASSERT(!key.isEmpty());
    return ::WritePrivateProfileString(section.constString(),   // lpAppName
                                       key.constString() ,                                     // lpKeyName
                                       NULL,                                                   // lpString
                                       url.localPath().constString()) == True;                 // szFile
}

//------------------------------------------------------------------------------
/**
*/
int DwPrivateProfile::readInt(const DwUrl& url, const DwString& section,
                              const DwString& key)
{
    DW_ASSERT(url.scheme() == L"file");
    DW_ASSERT(url.isValid());
    DW_ASSERT(!section.isEmpty());
    DW_ASSERT(!key.isEmpty());
    return (int)::GetPrivateProfileInt(section.constString(),   // LPCTSTR lpAppName
                                       key.constString(),                                      // LPCTSTR lpKeyName
                                       -1,                                                     // Default
                                       url.localPath().constString());                         // LPCTSTR lpFileName
}

//------------------------------------------------------------------------------
/**
*/
DwString DwPrivateProfile::readString(const DwUrl& url, const DwString& section,
                                      const DwString& key)
{
    DW_ASSERT(url.scheme() == L"file");
    DW_ASSERT(url.isValid());
    DW_ASSERT(!section.isEmpty());
    DW_ASSERT(!key.isEmpty());
    TCHAR str[256];
    int size =  ::GetPrivateProfileString(section.constString(),    // LPCTSTR lpAppName
                                          key.constString(),                                          // LPCTSTR lpKeyName
                                          L"",                                                        // LPCTSTR lpDefault
                                          str,                                                        // LPTSTR lpReturnedString
                                          255,                                                        // DWORD nSize, In Character
                                          url.localPath().constString());                             // LPCTSTR lpFileName
    if (size == 0)
    {
        return DwString(L"");
    }
    // truncated
    else if (size == 255 - 1 && str[254] == 0)
    {
        // If neither lpAppName nor lpKeyName is NULL
        // and the supplied destination buffer is too small to hold the requested string,
        // the string is truncated and followed by a null character,
        // and the return value is equal to nSize minus one.

        TCHAR *newstr = new TCHAR[1024];
        int size =  ::GetPrivateProfileString(section.constString(),    // LPCTSTR lpAppName
                                              key.constString(),                                          // LPCTSTR lpKeyName
                                              L"",                                                        // LPCTSTR lpDefault
                                              newstr,                                                     // LPTSTR lpReturnedString
                                              1023,                                                       // DWORD nSize, In Character
                                              url.localPath().constString());                             // LPCTSTR lpFileName
        DW_ASSERT(size <= 1023);
        newstr[size] = 0;
        DwString ret = DwString::fromWCharArray(str);
        delete [] newstr;
        return ret;
    }
    else
    {
        DW_ASSERT(size <= 255);
        str[size] = 0;
        return DwString::fromWCharArray(str);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwPrivateProfile::readStruct(const DwUrl& url, const DwString& section,
                                  const DwString& key, void* pStruct, int size)
{
    return ::GetPrivateProfileStruct(section.constString(),     //  lpAppName
                                     key.constString(),                                      //  lpKeyName
                                     pStruct,                                                // struct pointer
                                     size,                                                   // struct size
                                     url.localPath().constString()) != 0;                    //  lpFileName
}