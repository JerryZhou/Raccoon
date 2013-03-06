
#include "stable.h"
#include "dwfiletime.h"
#include "dwapp/dwutil.h"


//------------------------------------------------------------------------------
/**
*/
DwFileTime::DwFileTime(const DwString& str)
{
    DwVector<DwString> tokens = DwStringUtil::tokenize(str, "#");
    DW_ASSERT(tokens.size() == 2);
    this->time.dwHighDateTime = tokens[0].toInt32();
    this->time.dwLowDateTime = tokens[1].toInt32();
}

//------------------------------------------------------------------------------
/**
*/
DwString
DwFileTime::AsString() const
{
    DwString str;
    DwStringUtil::stringFormat(str, "%d#%d", this->time.dwHighDateTime, this->time.dwLowDateTime);
    return str;
}