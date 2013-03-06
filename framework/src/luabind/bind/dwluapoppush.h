#pragma once
#include "dwcore/dwstring.h"
#include "dwcore/dwvariant.h"
#include "dwgraphics/dwcolor.h"
#include "dwgraphics/dwfont.h"
#include "dwgraphics/dwimage.h"
#include "dwgui/dwgraphicsitem.h"
#include "dwgui/dwpropertyanimation.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwanchors.h"

//////////////////////////////////////////////////////////////////////////
#ifndef DW_CHECK
#define DW_CHECK(con) if (!(con)) {return;}
#define DW_CHECK_RET(con, ret) if(!(con)) {return (ret);}
#define DW_CHECK_ASSERT(con) if(!(con)) {assert(false); return;}
#define DW_CHECK_ASSERT_RET(con, ret) if(!(con)) {assert(false); return (ret);}
#endif
//////////////////////////////////////////////////////////////////////////

// Helper Struct
//////////////////////////////////////////////////////////////////////////
struct LUA_3Int 
{
    int x , y, z;
};
struct LUA_3Real
{
    DwReal x, y, z;
};
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/**
*/
template<typename T>
static T _lua_pop_value_of(lua_State *L, int index)
{
    DW_UNUSED(L);
    DW_UNUSED(index);
    DW_ASSERT(false);
    return T();
}

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT char _lua_pop_value_of<char>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT unsigned char _lua_pop_value_of<unsigned char>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_pop_value_of<int>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT uint32_t _lua_pop_value_of<uint32_t>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT double _lua_pop_value_of<double>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT float _lua_pop_value_of<float>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int64_t _lua_pop_value_of<int64_t>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT uint64_t _lua_pop_value_of<uint64_t>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT void* _lua_pop_value_of<void*>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT DwRectF _lua_pop_value_of<DwRectF>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT DwRect _lua_pop_value_of<DwRect>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT DwPointF _lua_pop_value_of<DwPointF>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT DwPoint _lua_pop_value_of<DwPoint>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT DwSizeF _lua_pop_value_of<DwSizeF>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT DwSize _lua_pop_value_of<DwSize>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT DwString _lua_pop_value_of<DwString>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT bool _lua_pop_value_of<bool>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT DwColor _lua_pop_value_of<DwColor>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT DwFont _lua_pop_value_of<DwFont>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT DwMargins _lua_pop_value_of<DwMargins>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT DwVariant _lua_pop_value_of<DwVariant>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
typedef DwGraphicsItem * LUA_GRAPHICSITEM_PTR;
template<>
DW_LUABIND_EXPORT LUA_GRAPHICSITEM_PTR _lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
typedef DwAbstractAnimation * LUA_ANIMATION_PTR;
template<>
DW_LUABIND_EXPORT LUA_ANIMATION_PTR _lua_pop_value_of<LUA_ANIMATION_PTR>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT DwImage _lua_pop_value_of<DwImage>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
typedef DwRtti * LUA_RTTI_PTR;
template<>
DW_LUABIND_EXPORT LUA_RTTI_PTR _lua_pop_value_of<LUA_RTTI_PTR>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT LUA_3Int _lua_pop_value_of<LUA_3Int>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT LUA_3Real _lua_pop_value_of<LUA_3Real>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT DwUrl _lua_pop_value_of<DwUrl>(lua_State *L, int index);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT DwByteArray _lua_pop_value_of<DwByteArray>(lua_State *L, int index);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
template<typename T>
int _lua_push_value_of(lua_State *L, const T &d)
{
    return 0
}

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<double>(lua_State *L, const double &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<float>(lua_State *L, const float &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<char>(lua_State *L, const char &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<unsigned char>(lua_State *L, const unsigned char &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<int>(lua_State *L, const int &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<uint32_t>(lua_State *L, const uint32_t &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<int64_t>(lua_State *L, const int64_t &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<uint64_t>(lua_State *L, const uint64_t &d);

//------------------------------------------------------------------------------
/**
*/
typedef void* LUA_VOID_PTR;
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<LUA_VOID_PTR>(lua_State *L, const LUA_VOID_PTR &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<DwRectF>(lua_State *L, const DwRectF &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<DwRect>(lua_State *L, const DwRect &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<DwPointF>(lua_State *L, const DwPointF &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<DwPoint>(lua_State *L, const DwPoint &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<DwSizeF>(lua_State *L, const DwSizeF &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<DwSize>(lua_State *L, const DwSize &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<DwString>(lua_State *L, const DwString &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<bool>(lua_State *L, const bool &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<DwColor>(lua_State *L, const DwColor &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<DwFont>(lua_State *L, const DwFont &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<DwMargins>(lua_State *L, const DwMargins &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<DwVariant>(lua_State *L, const DwVariant &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(lua_State *L, const LUA_GRAPHICSITEM_PTR &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<LUA_ANIMATION_PTR>(lua_State *L, const LUA_ANIMATION_PTR &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<DwImage>(lua_State *L, const DwImage &d);

//------------------------------------------------------------------------------
/**
*/
typedef DwAnchors* LUA_ANCHOR_PTR;
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<LUA_ANCHOR_PTR>(lua_State *L, const LUA_ANCHOR_PTR &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<LUA_RTTI_PTR>(lua_State *L, const LUA_RTTI_PTR &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<LUA_3Int>(lua_State *L, const LUA_3Int &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<LUA_3Real>(lua_State *L, const LUA_3Real &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<DwUrl>(lua_State *L, const DwUrl &d);

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<DwByteArray>(lua_State *L, const DwByteArray &d);