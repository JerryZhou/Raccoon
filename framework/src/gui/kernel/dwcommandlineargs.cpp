#include "stable.h"
#include "dwcommandlineargs.h"
#include "graphicsview/dwdummys.h"


//------------------------------------------------------------------------------
/**
*/
DwCommandLineArgs::DwCommandLineArgs()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwCommandLineArgs::DwCommandLineArgs(const DwString& l)
{
    m_args = dwTokenize(l, L" \t\n=", '"');
}

//------------------------------------------------------------------------------
/**
*/
DwCommandLineArgs::DwCommandLineArgs(int argc, const char **argv)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        m_args.append(argv[i]);
    }
}

//------------------------------------------------------------------------------
/**
    Put out a missing argument error.
*/
void DwCommandLineArgs::error(const DwString& ) const
{
    DW_WARNING("CmdLine: arg '%s' not found!");
}

//------------------------------------------------------------------------------
/**
    Returns the command name.
*/
const DwString& DwCommandLineArgs::cmdName() const
{
    DW_ASSERT(m_args.size() > 0);
    return m_args[0];
}

//------------------------------------------------------------------------------
/**
    Returns true if argument exists.
*/
bool DwCommandLineArgs::hasArg(const DwString& name) const
{
    int index = m_args.indexOf(name);
    return (index != InvalidIndex);
}

//------------------------------------------------------------------------------
/**
    Returns index of argument. Throws an error if argument not found.
*/
int DwCommandLineArgs::findArgIndex(const DwString& name) const
{
    DW_ASSERT(!name.isEmpty());
    int i = m_args.indexOf(name);
    if (InvalidIndex == i)
    {
        error(name);
    }
    return i;
}

//------------------------------------------------------------------------------
/**
*/
const DwString& DwCommandLineArgs::stringOf(const DwString& name, const DwString& defaultValue) const
{
    if (hasArg(name))
    {
        int index = findArgIndex(name);
        DW_ASSERT(index < m_args.size()-1);
        if (index < m_args.size()-1)
        {
            return m_args[index + 1];
        }
    }

    return defaultValue;
}

//------------------------------------------------------------------------------
/**
*/
int DwCommandLineArgs::intOf(const DwString& name, int defaultValue) const
{
    if (hasArg(name))
    {
        int index = findArgIndex(name);
        DW_ASSERT(index < m_args.size()-1);
        if (index < m_args.size()-1)
        {
            return m_args[index + 1].toInt32();
        }
    }

    return defaultValue;
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwCommandLineArgs::realOf(const DwString& name, DwReal defaultValue) const
{
    if (hasArg(name))
    {
        int index = findArgIndex(name);
        DW_ASSERT(index < m_args.size()-1);
        if (index < m_args.size()-1)
        {
            return _wtof(m_args[index + 1].constString());
        }
    }

    return defaultValue;
}

//------------------------------------------------------------------------------
/**
*/
bool DwCommandLineArgs::boolOf(const DwString& name, bool defaultValue) const
{
    if (hasArg(name))
    {
        int index = findArgIndex(name);
        DW_ASSERT(index < m_args.size()-1);
        if (index < m_args.size()-1)
        {
            const DwString &str = m_args[index + 1];
            if (str.compareNoCase(L"true") == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    return defaultValue;
}

//------------------------------------------------------------------------------
/**
*/
bool DwCommandLineArgs::boolOfFlag(const DwString& name) const
{
    return hasArg(name);
}

//------------------------------------------------------------------------------
/**
*/
int DwCommandLineArgs::numArgs() const
{
    DW_ASSERT(m_args.size() > 0);
    return m_args.size() - 1;
}

//------------------------------------------------------------------------------
/**
*/
const DwString& DwCommandLineArgs::stringAtIndex(int index) const
{
    return m_args[index + 1];
}

//------------------------------------------------------------------------------
/**
*/
int DwCommandLineArgs::intAtIndex(int index) const
{
    return m_args[index + 1].toInt32();
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwCommandLineArgs::realAtIndex(int index) const
{
    return _wtof(m_args[index + 1].constString());
}

//------------------------------------------------------------------------------
/**
*/
bool DwCommandLineArgs::boolAtIndex(int index) const
{
    const DwString &str = m_args[index + 1];
    if (str.compareNoCase(L"True"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwCommandLineArgs::appendCommandDwString(const DwCommandLineArgs& otherArgs)
{
    for (int i = 0; i < otherArgs.m_args.size(); ++i)
    {
        m_args.append(otherArgs.m_args[i]);
    }
}

//------------------------------------------------------------------------------
/**
*/
const DwVector<DwString>& DwCommandLineArgs::args() const
{
    return m_args;
}