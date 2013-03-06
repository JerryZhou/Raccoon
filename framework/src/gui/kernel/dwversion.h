#pragma once

#include "dwgui/dwringlogcase.h"

struct DW_GUI_EXPORT DwVersion
{
    DwVersion();
    DwVersion(uint32_t v);
    DwVersion(uint8_t ma, uint8_t mi, uint8_t bu, uint8_t re);
    DwVersion(const DwString &v);

    bool operator != (const DwVersion &rfs) const;
    bool operator == (const DwVersion &rfs) const;
    bool operator <= (const DwVersion &rfs) const;
    bool operator < (const DwVersion &rfs) const;
    bool operator >(const DwVersion &rfs) const;
    bool operator >=(const DwVersion &rfs) const;

    union
    {
        struct
        {
            uint8_t revision;
            uint8_t build;
            uint8_t minor;
            uint8_t major;
        };
        uint32_t version;
    };
};

//------------------------------------------------------------------------------
/**
*/
inline DwVersion::DwVersion()
    : version(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
inline DwVersion::DwVersion(uint32_t v)
{
    version = v;
}

//------------------------------------------------------------------------------
/**
*/
inline DwVersion::DwVersion(uint8_t ma, uint8_t mi, uint8_t bu, uint8_t re)
{
    major = ma;
    minor = mi;
    build = bu;
    revision = re;
}

//------------------------------------------------------------------------------
/**
*/
inline DwVersion::DwVersion(const DwString &v)
    : version(0)
{
    if (v.length() > 0)
    {
        int nMajor, nMinor, nBuild, nRevision;
        int num = ::swscanf_s(v.constString(), L"%u.%u.%u.%u", &nMajor, &nMinor, &nBuild, &nRevision);
        switch (num)
        {
        case 0:
            major = 0;
        case 1:
            minor = 0;
        case 2:
            build = 0;
        case 3:
            revision = 0;
        case 4:
            break;
        }
        switch (num)
        {
        case 4:
            revision = nRevision;
        case 3:
            build = nBuild;
        case 2:
            minor = nMinor;
        case 1:
            major = nMajor;
        case 0:
            break;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwVersion>(const DwVersion& v)
{
    format("%u.%u.%u.%u", v.major, v.minor, v.build, v.revision);
    return *this;
}