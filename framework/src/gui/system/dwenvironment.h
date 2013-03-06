#pragma once

#include "dwcore/dwstring.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwEnviroment
{
public:
    // return true if env - variable is exist
    static bool exists(const DwString &enviroment);
    // return the env-variable string
    static DwString read(const DwString &enviroment);
};// end of DwEnviroment