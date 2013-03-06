#include "stable.h"
#include "dwsysteminfo.h"
#include "dwcore/dwglobalstatic.h"

//------------------------------------------------------------------------------
/**
*/
DwSystemInfo::DwSystemInfo()
{
    // get runtime-info from Windows
    SYSTEM_INFO sysInfo;
    ::GetSystemInfo(&sysInfo);
    switch (sysInfo.wProcessorArchitecture)
    {
    case PROCESSOR_ARCHITECTURE_AMD64:
        m_cpuType = DwSystemInfo::X86_64;
        break;
    case PROCESSOR_ARCHITECTURE_INTEL:
        m_cpuType = DwSystemInfo::X86_32;
        break;
    default:
        m_cpuType = DwSystemInfo::UnknownCpuType;
        break;
    }
    m_platform = DwSystemInfo::Win32;
    m_numCpuCores = sysInfo.dwNumberOfProcessors;
    m_pageSize = sysInfo.dwPageSize;
}

//------------------------------------------------------------------------------
/**
*/
DwSystemInfo::Platform DwSystemInfo::platform() const
{
    return m_platform;
}

//------------------------------------------------------------------------------
/**
*/
DwSystemInfo::CpuType DwSystemInfo::cpuType() const
{
    return m_cpuType;
}

//------------------------------------------------------------------------------
/**
*/
int DwSystemInfo::numCpuCores() const
{
    return m_numCpuCores;
}

//------------------------------------------------------------------------------
/**
*/
int DwSystemInfo::pageSize() const
{
    return m_pageSize;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwSystemInfo::PlatformAsString(Platform p)
{
    switch (p)
    {
    case DwSystemInfo::Win32:
        return L"Win32";
        break;
    }

    return L"UnknownPlatform";
}

//------------------------------------------------------------------------------
/**
*/
DwString DwSystemInfo::CpuTypeAsString(CpuType cpu)
{
    switch(cpu)
    {
    case DwSystemInfo::X86_32:
        return L"X86_32";
        break;
    case DwSystemInfo::X86_64:
        return L"X86_64";
        break;
    }
    return L"UnknownCpuType";
}


DW_GLOBAL_STATIC(DwSystemInfo, _dw_systemInfo);
//------------------------------------------------------------------------------
/**
*/
DwSystemInfo *DwSystemInfo::systemInfo()
{
    return _dw_systemInfo();
}