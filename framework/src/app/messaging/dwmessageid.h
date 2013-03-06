#pragma once

class DW_APP_EXPORT DwMessageId
{
public:
    /// constructor
    DwMessageId();
    /// equality operator
    bool operator==(const DwMessageId& rhs) const;
};// end of DwMessageId

//------------------------------------------------------------------------------
/**
*/
inline
DwMessageId::DwMessageId()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
DwMessageId::operator==(const DwMessageId& rhs) const
{
    return (this == &rhs);
}

//------------------------------------------------------------------------------
/**
    Message Id macros.
*/
#define DW_DECLARE_MESSAGEID \
public:\
    static DwMessageId Id; \
    virtual const DwMessageId& GetId() const;\
private:

#define DW_IMPLEMENT_MESSAGEID(type) \
    DwMessageId type::Id; \
    const DwMessageId& type::GetId() const { return type::Id; }