#pragma once
#include "dwcore/dwstring.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwMediaType
{
public:
    /// constructor
    DwMediaType();
    DwMediaType(const DwString& str);
    DwMediaType(const DwString& _type, const DwString& _subType);
    DwMediaType(const DwMediaType& rhs);

    void operator=(const DwMediaType& rhs);
    bool operator==(const DwMediaType& rhs);
    bool operator!=(const DwMediaType& rhs);

    bool isValid() const;
    void clear();
    void set(const DwString& str);
    void set(const DwString& _type, const DwString& _subType);
    DwString asString() const;
    const DwString& type() const;
    const DwString& subType() const;

private:
    DwString m_type;
    DwString m_subType;
};// end of DwMediaType