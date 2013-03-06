#include "stable.h"

#include "dwenvironment.h"

//------------------------------------------------------------------------------
/**
*/
bool DwEnviroment::exists(const DwString &enviroment)
{
    TCHAR buf[256];
    DWORD result = ::GetEnvironmentVariable(enviroment.constString(), buf, sizeof(buf) / sizeof(TCHAR));
    if (0 == result)
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    An environment variable has a maximum size limit of 32,767 characters,
    including the null-terminating character.
    see : ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.chs/dllproc/base/getenvironmentvariable.htm
*/
DwString DwEnviroment::read(const DwString &enviroment)
{
    DwString retval;
    TCHAR buf[256] = { 0 };
    DWORD result = ::GetEnvironmentVariable(enviroment.constString(), buf, sizeof(buf) / sizeof(TCHAR));

    // error?
    if (0 == result)
    {
        DW_WARNING("DwEnviroment::read: failed to read env variable !");
    }
    else if (result >= sizeof(buf))
    {
        // buffer overflow?
        TCHAR *dynBuf = new TCHAR[result];
        if(NULL == dynBuf)
        {
            DW_WARNING("Out of memory\n");
            return retval;
        }
        DWORD ret = ::GetEnvironmentVariable(enviroment.constString(), dynBuf, result);
        if(ret == 0)
        {
            DW_ASSERT(false);
            DW_SAFE_DELETE_ARRAY(dynBuf);
            return DwString();
        }

        retval =  DwString::fromWCharArray(dynBuf);
        DW_SAFE_DELETE_ARRAY(dynBuf);
    }
    else
    {
        // all ok
        retval = DwString::fromWCharArray(buf);
    }
    return retval;
}