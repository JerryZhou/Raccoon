#pragma once

#include "dwapp/dwstreamwriter.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwTextWriter : public DwStreamWriter
{
    DW_DECLARE_CLASS(DwTextWriter);
public:
    /// constructor
    DwTextWriter();
    /// destructor
    virtual ~DwTextWriter();
    /// write a single character
    void writeChar(unsigned char c);
    /// write a string
    void writeString(const DwString& str);
    /// write some formatted text
    void writeFormatted(const char* fmtString, ...);
    /// write a line of text and append a new-line
    void writeLine(const DwString& line);
    /// write a number of lines, separated by new-lines
    void writeLines(const DwVector<DwString>& lines);
    /// generic writer method
    template<typename T> void write(const T& t);
};// end of DwTextWriter