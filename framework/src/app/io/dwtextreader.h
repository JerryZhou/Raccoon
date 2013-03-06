#pragma once

#include "dwapp/dwstreamreader.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwTextReader : public DwStreamReader
{
    DW_DECLARE_CLASS(DwTextReader);
public:
    /// constructor
    DwTextReader();
    /// destructor
    virtual ~DwTextReader();
    /// read a single character from the stream
    unsigned char readChar();
    /// read until next newline
    DwString readLine();
    /// read entire stream into a string object
    DwString readAll();
    /// read entire stream as lines into string array
    DwVector<DwString> readAllLines();
};// end of DwTextReader