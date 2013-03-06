#pragma once
#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwDelegate : public DwRttiObject
{
    DW_DECLARE_CLASS(DwDelegate);
public:
    typedef int (*fn_delegate_set)(DwRttiObject *item, lua_State *L);
    typedef int (*fn_delegate_get)(DwRttiObject *item, lua_State *L);
    typedef int (*fn_delegate_call)(DwRttiObject *item, lua_State *L);

    DwDelegate();

    bool addSet(const DwString &s, fn_delegate_set t);
    bool addGet(const DwString &s, fn_delegate_get t);
    bool addCall(const DwString &s, fn_delegate_get t);

    fn_delegate_set entrySet(const DwString & s);
    fn_delegate_get entryGet(const DwString & s);
    fn_delegate_get entryCall(const DwString & s);

    DwRtti *key() const;
    DwDelegate *parent() const;

protected:
    void setKey(DwRtti *k) ;
    void setParent(DwDelegate *p) ;

    friend class DwDelegateLib;

    DwHash<DwString, fn_delegate_set> m_sets;
    DwHash<DwString, fn_delegate_get> m_gets;
    DwHash<DwString, fn_delegate_call> m_calls;

    DwRtti *m_key;
    DwDelegate *m_parent;
};// end of DwDelegate
//------------------------------------------------------------------------------
DW_REGISTER_CLASS(DwDelegate);

//------------------------------------------------------------------------------
/**
*/
#define DECLARE_DELEGATE_SET(s) \
    static int delegate_set_##s##(DwRttiObject *obj, lua_State *L)

#define DECLARE_DELEGATE_GET(s) \
    static int delegate_get_##s##(DwRttiObject *obj, lua_State *L)

#define DECLARE_DELEGATE_CALL(s) \
    static int delegate_call_##s##(DwRttiObject *obj, lua_State *L)

#define DECLARE_ADD_SET(s) \
    addSet(#s, delegate_set_##s##)

#define DECLARE_ADD_GET(s) \
    addGet(#s, delegate_get_##s##)

#define DECLARE_ADD_CALL(s) \
    addCall(#s, delegate_call_##s##)