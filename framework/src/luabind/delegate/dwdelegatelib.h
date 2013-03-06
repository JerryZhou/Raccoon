#pragma once
#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class DwDelegate;

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwDelegateLib : public DwRttiObject
{
    DW_DECLARE_CLASS(DwDelegateLib);
public:
    DwDelegateLib();
    virtual ~DwDelegateLib();

    bool registerDelegate(DwRtti* obj, DwDelegate* gate);
    DwDelegate *delegateOf(DwRtti* obj) const;

protected:
    DwDelegate *parentOf(DwRtti* obj) const;

    typedef DwMap<DwRtti*, DwDelegate*> DelegatesMap;
    DelegatesMap m_delegates;
};// end of DwDelegateLib

//------------------------------------------------------------------------------
DW_LUABIND_EXPORT DwDelegateLib* dwDelegateLib();

//------------------------------------------------------------------------------
#define DW_REGISTER_ITEM_DELEGATE(type, dtype) \
    static const bool type##itemdelegate##_registered = dwDelegateLib()->registerDelegate(type::RTTI(), dtype::RTTI())