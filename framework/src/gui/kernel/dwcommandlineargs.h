#pragma once

#include "dwcore/dwvector.h"
#include "dwcore/dwstring.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwCommandLineArgs
{
public:
    DwCommandLineArgs();
    DwCommandLineArgs(const DwString& );
    DwCommandLineArgs(int argc, const char **);

    /// get the command name
    const DwString& cmdName() const;
    /// return true if arg exists
    bool hasArg(const DwString& arg) const;
    /// get DwString value
    const DwString& stringOf(const DwString& name, const DwString& defaultValue = "") const;
    /// get int value
    int intOf(const DwString& name, int defaultValue = 0) const;
    /// get float value
    DwReal realOf(const DwString& name, DwReal defaultValue = 0.0) const;
    /// get bool value (key=value)
    bool boolOf(const DwString& name, bool defaultValue=false) const;
    /// get bool flag (args only needs to exist to trigger as true)
    bool boolOfFlag(const DwString& name) const;

    /// get number of arguments (exluding command name)
    int numArgs() const;
    /// get DwString argument at index
    const DwString& stringAtIndex(int index) const;
    /// get int value at index
    int intAtIndex(int index) const;
    /// get float value at index
    DwReal realAtIndex(int index) const;
    /// get bool value at index
    bool boolAtIndex(int index) const;

    /// append other command DwString
    void appendCommandDwString(const DwCommandLineArgs& otherArgs);

    /// get all of the value
    const DwVector<DwString>& args() const;

protected:
    /// put out an error
    void error(const DwString& argName) const;
    /// find index of argument, fails hard with error msg if not found
    int findArgIndex(const DwString& argName) const;

    DwVector<DwString> m_args;
};// end of DwCommandLineArgs