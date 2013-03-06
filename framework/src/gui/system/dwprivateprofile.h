#pragma once

#include "dwgui/dwurl.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwPrivateProfile
{
public:
    // return true if the file(url), [section] and key is exits
    static bool exists(const DwUrl& url, const DwString& section, const DwString& key);

    // write a int to file(url), [section] key = v
    static bool writeInt(const DwUrl& url, const DwString& section, const DwString& key, int v);
    // write a string to file(url), [section] key = v
    static bool writeString(const DwUrl& url, const DwString& section, const DwString& key, const DwString &v);
    // write a struct to file(url), [section] key = pStruct
    static bool writeStruct(const DwUrl& url, const DwString& section, const DwString& key, void* pStruct, int size);

    // remove the secion [section]
    static bool removeSection(const DwUrl& url, const DwString& section);
    // remove the key [section] key = *
    static bool removeKey(const DwUrl&url, const DwString& section, const DwString& key);

    // read a int at file(url), [section] key = v
    static int readInt(const DwUrl& url, const DwString& section, const DwString& key);
    // read a string at file(url), [section] key = v
    static DwString readString(const DwUrl& url, const DwString& section, const DwString& key);
    // read a binary struct to pStruct
    static bool readStruct(const DwUrl& url, const DwString& section, const DwString& key, void* pStruct, int size);
protected:
};// end of DwPrivateProfile