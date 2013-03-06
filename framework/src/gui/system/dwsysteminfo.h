#pragma once

#include "dwcore/dwstring.h"

class DW_GUI_EXPORT DwSystemInfo
{
public:
    // host platforms
    enum Platform
    {
        Win32,

        UnknownPlatform,
    };

    // CPU types
    enum CpuType
    {
        X86_32,             // any 32-bit x86
        X86_64,             // any 64-bit x86

        UnknownCpuType,
    };

    // constructor
    DwSystemInfo();

    // get host platform
    Platform platform() const;
    // get cpu type
    CpuType cpuType() const;
    // get number of processors
    int numCpuCores() const;
    // get page size
    int pageSize() const;

    // convert platform to string
    static DwString PlatformAsString(Platform p);
    // convert CpuType to string
    static DwString CpuTypeAsString(CpuType cpu);
    // get the system information
    static DwSystemInfo *systemInfo();

protected:
    Platform m_platform;
    CpuType m_cpuType;
    int m_numCpuCores;
    int m_pageSize;
};// end of DwSystemInfo