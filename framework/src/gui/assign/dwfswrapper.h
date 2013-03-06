#pragma once

#include "dwcore/dwstring.h"

class DW_GUI_EXPORT DwFsWrapper
{
public:
    typedef HANDLE Handle;

    static bool isDeviceName(const DwString& scheme);

    static DwString homeDirectory();

    static DwString binDirectory();

    static DwString tempDirectory();

    static DwString userDirectory();

    static DwString appDataDirectory();

    static DwString programsDirectory();

    static DwString rootDirectory();

    static DwString currentDirectory();

    static DwVector<DwString> listFiles(const DwString& dirPath, const DwString& pattern);

    static DwVector<DwString> listDirectories(const DwString& dirPath, const DwString& pattern);
};// end of DwFsWrapper