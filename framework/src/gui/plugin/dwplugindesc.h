#pragma once
#include "dwgui/dwurl.h"
#include "dwgui/dwcommandlineargs.h"
#include "dwgui/dwrtti.h"
#include "dwgui/dwringlogcase.h"
#include "dwgui/dwversion.h"

struct DW_GUI_EXPORT DwPluginDesc
{
    DwUrl url;
    DwString desc;
    DwString dependence;
    DwString permissions;
    DwCommandLineArgs args;
    DwVersion version;
    DwRtti * rtti;
    DwRtti * mappingRtti;
    DwRtti * contextRtti;

    DwPluginDesc()
        : rtti(NULL)
        , mappingRtti(NULL)
        , contextRtti(NULL)
        , version(0)
    {
        ;
    }

    static bool isValid(const DwPluginDesc& idesc);
};

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwPluginDesc>(const DwPluginDesc& v)
{
    (*this) << "url: "<< v.url.asString();
    (*this) << " desc: " << v.desc;
    (*this) << " dependence: " << v.dependence;
    (*this) << " permissions: " << v.permissions;
    (*this) << " args: " << v.args.args();
    (*this) << " version: "<< v.version;
    if (v.rtti)
    {
        (*this) << " rtti: " << v.rtti->name();
    }
    else
    {
        (*this) << " rtti: " << "NULL";
    }
    if (v.mappingRtti)
    {
        (*this) << " mappingRtti: " << v.mappingRtti->name();
    }
    else
    {
        (*this) << " mappingRtti: " << "NULL";
    }
    if (v.contextRtti)
    {
        (*this) << " contextRtti: " << v.contextRtti->name();
    }
    else
    {
        (*this) << " contextRtti: " << "NULL";
    }

    return *this;
}