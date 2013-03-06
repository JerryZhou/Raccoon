#include "stable.h"
#include "dwwin32registry.h"

//------------------------------------------------------------------------------
/**
*/
bool DwWin32Registry::exists(RootKey rootKey, const DwString& key, const DwString& name)
{
    DW_ASSERT(!key.isEmpty());
    HKEY win32RootKey = RootKeyToWin32KeyHandle(rootKey);
    HKEY hKey = 0;
    LONG res = ::RegOpenKeyEx(win32RootKey,       // hKey
                              key.constString(),    // lpSubKey
                              0,                  // ulOptions (reserved)
                              KEY_READ,           // samDesired
                              &hKey);
    if (ERROR_SUCCESS != res)
    {
        // key does not exist
        return false;
    }
    if (!name.isEmpty())
    {
        res = ::RegQueryValueEx(hKey,             // hKey
                                name.constString(), // lpValueName
                                NULL,             // lpReserved
                                NULL,             // lpType
                                NULL,             // lpData
                                NULL);            // lpcbData
        ::RegCloseKey(hKey);
        return (ERROR_SUCCESS == res);
    }
    else
    {
        // key exists, value name was empty
        ::RegCloseKey(hKey);
        return true;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwWin32Registry::writeString(RootKey rootKey, const DwString& key,
                                  const DwString& name, const DwString& value)
{
    DW_ASSERT(!key.isEmpty());
    DW_ASSERT(!name.isEmpty());

    HKEY win32RootKey = RootKeyToWin32KeyHandle(rootKey);
    HKEY hKey = 0;
    LONG res = ::RegCreateKeyEx(win32RootKey,     // hKey
                                key.constString(),  // lpSubKey
                                0,                // Reserved
                                NULL,             // lpClass
                                REG_OPTION_NON_VOLATILE,  // dwOptions
                                KEY_ALL_ACCESS,   // samDesired
                                NULL,             // lpSecurityAttribute
                                &hKey,            // phkResult
                                NULL);            // lpdwDisposition
    if (ERROR_SUCCESS == res)
    {
        res = ::RegSetValueEx(hKey,               // hKey
                              name.constString(),   // lpValueName
                              0,                  // Reserved
                              REG_SZ,             // dwType (normal string)
                              (const BYTE*) value.constString(),    // lpData
                              (value.length() + 1)*sizeof(wchar_t));                // cbData
        ::RegCloseKey(hKey);
        return (ERROR_SUCCESS == res);
    }
    else
    {
        return false;
    }
}

bool DwWin32Registry::writeInt(RootKey rootKey, const DwString& key, const DwString& name, int32_t value)
{
    DW_ASSERT(!key.isEmpty());
    DW_ASSERT(!name.isEmpty());

    HKEY win32RootKey = RootKeyToWin32KeyHandle(rootKey);
    HKEY hKey = 0;
    LONG res = ::RegCreateKeyEx(win32RootKey,     // hKey
                                key.constString(),  // lpSubKey
                                0,                // Reserved
                                NULL,             // lpClass
                                REG_OPTION_NON_VOLATILE,  // dwOptions
                                KEY_ALL_ACCESS,   // samDesired
                                NULL,             // lpSecurityAttribute
                                &hKey,            // phkResult
                                NULL);            // lpdwDisposition
    if (ERROR_SUCCESS == res)
    {
        res = ::RegSetValueEx(hKey,               // hKey
                              name.constString(),   // lpValueName
                              0,                  // Reserved
                              REG_DWORD,             // dwType (32-bit number)
                              (const BYTE*) &value,    // lpData
                              sizeof(int32_t));                // cbData
        ::RegCloseKey(hKey);
        return (ERROR_SUCCESS == res);
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
DwString DwWin32Registry::readString(RootKey rootKey, const DwString& key, const DwString& name)
{
    DW_ASSERT(!key.isEmpty());
    HKEY win32RootKey = RootKeyToWin32KeyHandle(rootKey);
    HKEY hKey = 0;
    LONG res = ::RegOpenKeyEx(win32RootKey,       // hKey
                              key.constString(),    // lpSubKey
                              0,                  // ulOptions (reserved)
                              KEY_READ,           // samDesired
                              &hKey);
    DW_ASSERT(ERROR_SUCCESS == res);

    // need a buffer for the result
    const int MaxResultLen = 1024;
    TCHAR wideResult[MaxResultLen];

    // read registry key value as wide char string
    DWORD pcbData = sizeof(wideResult) - sizeof(TCHAR);
    res = ::RegQueryValueExW(hKey,                // hKey
                             (LPWSTR)name.constString(),    // lpValueName
                             NULL,                // lpReserved
                             NULL,                // lpType
                             (LPBYTE)wideResult,  // lpData
                             &pcbData);           // lpcbData
    DW_ASSERT(ERROR_SUCCESS == res);
    ::RegCloseKey(hKey);

    // need to 0-terminate queried value
    DW_ASSERT(((pcbData & 1) == 0) && (pcbData < sizeof(wideResult)));
    wideResult[pcbData / 2] = 0;

    return DwString::fromWCharArray(wideResult);
}

//------------------------------------------------------------------------------
/**
*/
int DwWin32Registry::readInt(RootKey rootKey, const DwString& key, const DwString& name)
{
    DW_ASSERT(!key.isEmpty());
    HKEY win32RootKey = RootKeyToWin32KeyHandle(rootKey);
    HKEY hKey = 0;
    LONG res = RegOpenKeyEx(win32RootKey,       // hKey
                            key.constString(),    // lpSubKey
                            0,                  // ulOptions (reserved)
                            KEY_READ,           // samDesired
                            &hKey);
    DW_ASSERT(ERROR_SUCCESS == res);

    DWORD dwType = REG_DWORD;
    DWORD dwSize = sizeof(DWORD);
    DWORD result;

    res = ::RegQueryValueEx(hKey,
                            name.constString(),
                            NULL,
                            &dwType,
                            (BYTE*)&result,
                            &dwSize);
    DW_ASSERT(ERROR_SUCCESS == res);
    ::RegCloseKey(hKey);
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool DwWin32Registry::deleteKey(RootKey rootKey, const DwString& key)
{
    DW_ASSERT(!key.isEmpty());
    HKEY win32RootKey = RootKeyToWin32KeyHandle(rootKey);
    LONG res = ::RegDeleteKey(win32RootKey,       // hKey
                              key.constString());   // lpSubKey
    return (ERROR_SUCCESS == res);
}

//------------------------------------------------------------------------------
/**
*/
DwWin32Registry::RootKey DwWin32Registry::asRootKey(const DwString& str)
{
    if (str == L"HKEY_CLASSES_ROOT") return ClassesRoot;
    else if (str == L"HKEY_CURRENT_USER") return CurrentUser;
    else if (str == L"HKEY_LOCAL_MACHINE") return LocalMachine;
    else if (str == L"HKEY_USERS") return Users;
    else
    {
        DW_ASSERT("DwWin32Register::AsRootKey(): Invalid string !");
        return ClassesRoot;
    }
}

//------------------------------------------------------------------------------
/**
*/
HKEY DwWin32Registry::RootKeyToWin32KeyHandle( RootKey rootKey )
{
    switch (rootKey)
    {
    case ClassesRoot:
        return HKEY_CLASSES_ROOT;
    case CurrentUser:
        return HKEY_CURRENT_USER;
    case LocalMachine:
        return HKEY_LOCAL_MACHINE;
    case Users:
        return HKEY_USERS;
    }
    // can't happen
    DW_ASSERT("Can't happen!");
    return 0;
}
