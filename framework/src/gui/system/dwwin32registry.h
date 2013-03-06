#pragma once

#include "dwcore/dwstring.h"

// ONLY WORK IN WIN32
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwWin32Registry
{
public:
    // key enumeration
    enum RootKey
    {
        ClassesRoot,
        CurrentUser,
        LocalMachine,
        Users,
    };

    // return true if a registry entry exists
    static bool exists(RootKey rootKey, const DwString& key, const DwString& name);
    // write a registry entry
    static bool writeString(RootKey rootKey, const DwString& key, const DwString& name, const DwString& value);
    // write a registry entry
    static bool writeInt(RootKey rootKey, const DwString& key, const DwString& name, int32_t value);
    // read a string registry entry, the string will be UTF-8 encoded!
    static DwString readString(RootKey rootKey, const DwString& key, const DwString& name);
    // read an int registry entry
    static int readInt(RootKey rootKey, const DwString& key, const DwString& name);
    // delete a registry key (and all its contained values)
    static bool deleteKey(RootKey rootKey, const DwString& key);
    // convert rootkey from string
    static RootKey asRootKey(const DwString& str);

private:
    // convert RootKey enum into Win32 key handle
    static HKEY RootKeyToWin32KeyHandle(RootKey rootKey);
};// end of DwRegister