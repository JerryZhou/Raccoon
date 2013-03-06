#pragma once
#include "stable.h"
#include "dwluapoppush.h"
#include "dwcore/dwstring.h"
#include "dwcore/dwvariant.h"
#include "dwgraphics/dwcolor.h"
#include "dwgraphics/dwfont.h"
#include "dwgraphics/dwimage.h"
#include "dwgui/dwgraphicsitem.h"
#include "dwgui/dwpropertyanimation.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwanchors.h"

//------------------------------------------------------------------------------
/**
*/
static DwString _p_key_of(lua_State *L, int index)
{
    DW_ASSERT(lua_isstring(L, index));
    DwString s = DwString::fromUtf8(lua_tostring(L, index));
    return s;
}

//------------------------------------------------------------------------------
/**
*/
template<>
char _lua_pop_value_of<char>(lua_State *L, int index)
{
    DW_ASSERT(lua_isnumber(L, index));
    return (char)lua_tointeger(L, index);
}

//------------------------------------------------------------------------------
/**
*/
template<>
unsigned char _lua_pop_value_of<unsigned char>(lua_State *L, int index)
{
    DW_ASSERT(lua_isnumber(L, index));
    return (unsigned char)lua_tointeger(L, index);
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_pop_value_of<int>(lua_State *L, int index)
{
    DW_ASSERT(lua_isnumber(L, index));
    return lua_tointeger(L, index);
}

//------------------------------------------------------------------------------
/**
*/
template<>
uint32_t _lua_pop_value_of<uint32_t>(lua_State *L, int index)
{
    DW_ASSERT(lua_isnumber(L, index));
    return (uint32_t)lua_tointeger(L, index);
}

//------------------------------------------------------------------------------
/**
*/
template<>
double _lua_pop_value_of<double>(lua_State *L, int index)
{
    DW_ASSERT(lua_isnumber(L, index));
    return lua_tonumber(L, index);
}

//------------------------------------------------------------------------------
/**
*/
template<>
float _lua_pop_value_of<float>(lua_State *L, int index)
{
    DW_ASSERT(lua_isnumber(L, index));
    return (float)(lua_tonumber(L, index));
}

//------------------------------------------------------------------------------
/**
*/
template<>
int64_t _lua_pop_value_of<int64_t>(lua_State *L, int index)
{
    DW_ASSERT(lua_isnumber(L, index));
    return (int64_t)(lua_tonumber(L, index));
}

//------------------------------------------------------------------------------
/**
*/
template<>
uint64_t _lua_pop_value_of<uint64_t>(lua_State *L, int index)
{
    DW_ASSERT(lua_isnumber(L, index));
    return (uint64_t)(lua_tonumber(L, index));
}

//------------------------------------------------------------------------------
/**
*/
template<>
void* _lua_pop_value_of<void*>(lua_State *L, int index)
{
    DW_ASSERT(lua_islightuserdata(L, index));
    return (void*)(lua_topointer(L, index));
}

//------------------------------------------------------------------------------
/**
*/
template<>
DwRectF _lua_pop_value_of<DwRectF>(lua_State *L, int index)
{
    DwRectF rect;
    DW_ASSERT(lua_istable(L, index));

    lua_rawgeti(L, index, 1);
    if (!lua_isnumber(L, -1))
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "x");
    }
    DW_ASSERT(lua_isnumber(L, -1));
    rect.setX( lua_tonumber(L, -1) );
    lua_pop(L, 1);

    lua_rawgeti(L, index, 2);
    if (!lua_isnumber(L, -1))
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "y");
    }
    rect.setY( lua_tonumber(L, -1) );
    lua_pop(L, 1);

    lua_rawgeti(L, index, 3);
    if (!lua_isnumber(L, -1))
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "width");
    }
    rect.setWidth( lua_tonumber(L, -1));
    lua_pop(L, 1);

    lua_rawgeti(L, index, 4);
    if (!lua_isnumber(L, -1))
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "height");
    }
    rect.setHeight( lua_tonumber(L, -1));
    lua_pop(L, 1);

    return rect;
}

//------------------------------------------------------------------------------
/**
*/
template<>
DwRect _lua_pop_value_of<DwRect>(lua_State *L, int index)
{
    return _lua_pop_value_of<DwRectF>(L, index).toRect();
}

//------------------------------------------------------------------------------
/**
*/
template<>
DwPointF _lua_pop_value_of<DwPointF>(lua_State *L, int index)
{
    DwPointF p;
    DW_ASSERT(lua_istable(L, index));

    lua_rawgeti(L, index, 1);
    if (!lua_isnumber(L, -1))
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "x");
    }
    DW_ASSERT(lua_isnumber(L, -1));
    p.setX( lua_tonumber(L, -1) );
    lua_pop(L, 1);

    lua_rawgeti(L, index, 2);
    if (!lua_isnumber(L, -1))
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "y");
    }
    DW_ASSERT(lua_isnumber(L, -1));
    p.setY( lua_tonumber(L, -1) );
    lua_pop(L, 1);

    return p;
}

//------------------------------------------------------------------------------
/**
*/
template<>
DwPoint _lua_pop_value_of<DwPoint>(lua_State *L, int index)
{
    return _lua_pop_value_of<DwPointF>(L, index).toPoint();
}

//------------------------------------------------------------------------------
/**
*/
template<>
DwSizeF _lua_pop_value_of<DwSizeF>(lua_State *L, int index)
{
    DwSizeF size;

    lua_rawgeti(L, index, 1);
    if (!lua_isnumber(L, -1))
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "width");
    }
    size.setWidth( lua_tonumber(L, -1));
    lua_pop(L, 1);

    lua_rawgeti(L, index, 2);
    if (!lua_isnumber(L, -1))
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "height");
    }
    size.setHeight( lua_tonumber(L, -1));
    lua_pop(L, 1);

    return size;
}

//------------------------------------------------------------------------------
/**
*/
template<>
DwSize _lua_pop_value_of<DwSize>(lua_State *L, int index)
{
    return _lua_pop_value_of<DwSizeF>(L, index).toSize();
}

//------------------------------------------------------------------------------
/**
*/
template<>
DwString _lua_pop_value_of<DwString>(lua_State *L, int index)
{
    DW_ASSERT(lua_isstring(L, index));
    DwString s = DwString::fromUtf8(lua_tostring(L, index));
    return s;
}

//------------------------------------------------------------------------------
/**
*/
template<>
bool _lua_pop_value_of<bool>(lua_State *L, int index)
{
    DW_ASSERT(lua_isboolean(L, index));
    bool s = !!(lua_toboolean(L, index));
    return s;
}

//------------------------------------------------------------------------------
/**
*/
template<>
DwColor _lua_pop_value_of<DwColor>(lua_State *L, int index)
{
    DwColor c;
    DW_ASSERT(lua_istable(L, index));

    // red
    lua_rawgeti(L, index, 1);
    if (lua_isnumber(L, -1))
    {
        c.setRed(lua_tointeger(L, -1) );
    }
    else
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "red");
        if (lua_isnumber(L, -1))
        {
            c.setRed(lua_tointeger(L, -1) );
        }
    }
    lua_pop(L, 1);

    // green
    lua_rawgeti(L, index, 2);
    if (lua_isnumber(L, -1))
    {
        c.setGreen(lua_tointeger(L, -1) );
    }
    else
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "green");
        if (lua_isnumber(L, -1))
        {
            c.setGreen(lua_tointeger(L, -1) );
        }
    }
    lua_pop(L, 1);

    // blue
    lua_rawgeti(L, index, 3);
    if (lua_isnumber(L, -1))
    {
        c.setBlue(lua_tointeger(L, -1) );
    }
    else
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "blue");
        if (lua_isnumber(L, -1))
        {
            c.setBlue(lua_tointeger(L, -1) );
        }
    }
    lua_pop(L, 1);

    // alpha
    lua_rawgeti(L, index, 4);
    if (lua_isnumber(L, -1))
    {
        c.setAlpha(lua_tointeger(L, -1) );
    }
    else
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "alpha");
        if (lua_isnumber(L, -1))
        {
            c.setAlpha(lua_tointeger(L, -1) );
        }
    }
    lua_pop(L, 1);

    return c;
}

//------------------------------------------------------------------------------
/**
*/
template<>
DwFont _lua_pop_value_of<DwFont>(lua_State *L, int index)
{
    DwFont f;
    DW_ASSERT(lua_istable(L, index));

    // name
    lua_getfield(L, index, "name");
    if (lua_isstring(L, -1))
    {
        f.setName(_lua_pop_value_of<DwString>(L, -1));
    }
    lua_pop(L, 1);

    // style
    lua_getfield(L, index, "style");
    if (lua_isnumber(L, -1))
    {
        f.setStyle((Dw::FontStyle) lua_tointeger(L, -1));
    }
    lua_pop(L, 1);

    // size
    lua_getfield(L, index, "size");
    if (lua_isnumber(L, -1))
    {
        f.setSize(lua_tonumber(L, -1));
    }
    lua_pop(L, 1);

    // color
    lua_getfield(L, index, "color");
    if (lua_istable(L, -1))
    {
        f.setColor(_lua_pop_value_of<DwColor>(L, -1));
    }
    lua_pop(L, 1);

    return f;
}

//------------------------------------------------------------------------------
/**
*/
template<>
DwMargins _lua_pop_value_of<DwMargins>(lua_State *L, int index)
{
    DwMargins m;
    DW_ASSERT(lua_istable(L, index));

    // left
    lua_rawgeti(L, index, 1);
    if (!lua_isnumber(L, -1))
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "left");
    }
    DW_ASSERT(lua_isnumber(L, -1));
    m.setLeft(lua_tointeger(L, -1));
    lua_pop(L, 1);

    // top
    lua_rawgeti(L, index, 2);
    if (!lua_isnumber(L, -1))
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "top");
    }
    DW_ASSERT(lua_isnumber(L, -1));
    m.setTop(lua_tointeger(L, -1));
    lua_pop(L, 1);

    // right
    lua_rawgeti(L, index, 3);
    if (!lua_isnumber(L, -1))
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "right");
    }
    DW_ASSERT(lua_isnumber(L, -1));
    m.setRight(lua_tointeger(L, -1));
    lua_pop(L, 1);

    // bottom
    lua_rawgeti(L, index, 4);
    if (!lua_isnumber(L, -1))
    {
        lua_pop(L, 1);
        lua_getfield(L, index, "bottom");
    }
    DW_ASSERT(lua_isnumber(L, -1));
    m.setBottom(lua_tointeger(L, -1));
    lua_pop(L, 1);

    return m;
}

//------------------------------------------------------------------------------
/**
*/
template<>
DwVariant _lua_pop_value_of<DwVariant>(lua_State *L, int index)
{
    if (lua_istable(L, index))
    {
        DwVariant v;
        int vtype = -1;
        lua_getfield(L, index, "type");
        if (lua_isnumber(L, -1))
        {
            vtype = lua_tointeger(L, -1);
        }
        lua_pop(L, 1);
        lua_getfield(L, index, "value");

        switch (vtype)
        {
        case DwVariant::Char:
            v = DwVariant(_lua_pop_value_of<char>(L, -1));
            break;
        case DwVariant::UChar:
            v = DwVariant(_lua_pop_value_of<unsigned char>(L, -1));
            break;
        case DwVariant::Bool:
            v =  DwVariant(_lua_pop_value_of<bool>(L, -1));
            break;
        case DwVariant::Int:
            v = DwVariant(_lua_pop_value_of<int>(L, -1));
            break;
        case DwVariant::UInt:
            v = DwVariant(_lua_pop_value_of<unsigned int>(L, -1));
            break;
        case DwVariant::Double:
            v = DwVariant(_lua_pop_value_of<double>(L, -1));
            break;
        case DwVariant::Float:
            v = DwVariant(_lua_pop_value_of<float>(L, -1));
            break;
        case DwVariant::Int64:
            v = DwVariant(_lua_pop_value_of<int64_t>(L, -1));
            break;
        case DwVariant::UInt64:
            v = DwVariant(_lua_pop_value_of<uint64_t>(L, -1));
            break;
        case DwVariant::Ptr:
            v = DwVariant(_lua_pop_value_of<void *>(L, -1));
            break;
        case DwVariant::RectF:
            v = DwVariant(_lua_pop_value_of<DwRectF>(L, -1));
            break;
        case DwVariant::Rect:
            v = DwVariant(_lua_pop_value_of<DwRect>(L, -1));
            break;
        case DwVariant::PointF:
            v = DwVariant(_lua_pop_value_of<DwPointF>(L, -1));
            break;
        case DwVariant::Point:
            v = DwVariant(_lua_pop_value_of<DwPoint>(L, -1));
            break;
        case DwVariant::SizeF:
            v = DwVariant(_lua_pop_value_of<DwSizeF>(L, -1));
            break;
        case DwVariant::Size:
            v = DwVariant(_lua_pop_value_of<DwSize>(L, -1));
            break;
        case DwVariant::String:
            v = DwVariant(_lua_pop_value_of<DwString>(L, -1));
            break;
        case DwVariant::Margins:
            v= DwVariant(_lua_pop_value_of<DwMargins>(L, -1));
        default:
            DW_ASSERT(false && "error variant type");
        }
        lua_pop(L, 1);
        return v;
    }
    if (lua_isnumber(L, index))
    {
        return DwVariant(_lua_pop_value_of<double>(L, index));
    }
    else if ( lua_isstring(L, index) )
    {
        return DwVariant(_lua_pop_value_of<DwString>(L, index));
    }
    else if (lua_isboolean(L, index))
    {
        return DwVariant(_lua_pop_value_of<bool>(L, index));
    }

    return DwVariant();
}

//------------------------------------------------------------------------------
/**
*/
typedef DwGraphicsItem * LUA_GRAPHICSITEM_PTR;
template<>
LUA_GRAPHICSITEM_PTR _lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(lua_State *L, int index)
{
    if ( lua_isnumber(L, index) )
    {
        int id = lua_tointeger(L, index);
        return DwGraphicsItem::findItem(id);
    }
    else if (lua_istable(L, index))
    {
        int id = 0;
        lua_pushstring(L, "_ui_id");
        if (index > 0)
        {
            lua_rawget(L, index);
        }
        else
        {
            lua_rawget(L, index-1);
        }

        if (lua_isnumber(L, -1))
        {
            id = lua_tointeger(L, -1);
        }
        lua_pop(L, 1);

        if (id)
        {
            return DwGraphicsItem::findItem(id);
        }
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
typedef DwAbstractAnimation * LUA_ANIMATION_PTR;
template<>
LUA_ANIMATION_PTR _lua_pop_value_of<LUA_ANIMATION_PTR>(lua_State *L, int index)
{
    DW_ASSERT(lua_isnumber(L, index));
    int id = lua_tointeger(L, index);

    DwAbstractAnimation * anim = DwAbstractAnimation::findAnimation((uint32_t)(id));
    return anim;
}

//------------------------------------------------------------------------------
/**
*/
template<>
DwImage _lua_pop_value_of<DwImage>(lua_State *L, int index)
{
    if (lua_isstring(L, index))
    {
        DwString s = _lua_pop_value_of<DwString>(L, index);
        DwImage img = DwResourceInterface::acquireImage(DwUrl(s));
        return img;
    }
    else if (lua_isnumber(L, index))
    {
        DwImage img(DwAtomicInt(lua_tointeger(L, index)));
        return img;
    }
    else if (lua_isuserdata(L, index))
    {
        DwImage ** p = (DwImage**)luaL_checkudata(L, index, "_lua_image_metatable");
        if (p && *p)
        {
            return DwImage(**p);
        }
    }
    else if (lua_istable(L, index))
    {
        lua_getfield(L, index, "_img_userdata");
        if (lua_isuserdata(L, -1))
        {
            DwImage ** p = (DwImage**)luaL_checkudata(L, -1, "_lua_image_metatable");
            if (p && *p)
            {
                lua_pop(L, 1);
                return DwImage(**p);
            }
        }
        lua_pop(L, 1);
    }

    return DwImage();
}

//------------------------------------------------------------------------------
/**
*/
typedef DwRtti * LUA_RTTI_PTR;
template<>
LUA_RTTI_PTR _lua_pop_value_of<LUA_RTTI_PTR>(lua_State *L, int index)
{
    if (lua_isnumber(L, index))
    {
        DwFourCC cc = _lua_pop_value_of<DwFourCC>(L, index);
        return (LUA_RTTI_PTR)dwFactory()->classRttiOf(cc);
    }
    else if (lua_isstring(L, index))
    {
        DwString s = _lua_pop_value_of<DwString>(L, index);
        return (LUA_RTTI_PTR)dwFactory()->classRttiOf(s);
    }

    return NULL;
}
//------------------------------------------------------------------------------
/**
*/
template<>
LUA_3Int _lua_pop_value_of<LUA_3Int>(lua_State *L, int index)
{
    LUA_3Int m;

    // left
    lua_rawgeti(L, index, 1);
    DW_ASSERT(lua_isnumber(L, -1));
    m.x = (lua_tointeger(L, -1));
    lua_pop(L, 1);

    // top
    lua_rawgeti(L, index, 2);
    DW_ASSERT(lua_isnumber(L, -1));
    m.y = (lua_tointeger(L, -1));
    lua_pop(L, 1);

    // right
    lua_rawgeti(L, index, 3);
    DW_ASSERT(lua_isnumber(L, -1));
    m.z = (lua_tointeger(L, -1));
    lua_pop(L, 1);

    return m;
}

//------------------------------------------------------------------------------
/**
*/
template<>
LUA_3Real _lua_pop_value_of<LUA_3Real>(lua_State *L, int index)
{
    LUA_3Real m;

    // left
    lua_rawgeti(L, index, 1);
    DW_ASSERT(lua_isnumber(L, -1));
    m.x = (lua_tonumber(L, -1));
    lua_pop(L, 1);

    // top
    lua_rawgeti(L, index, 2);
    DW_ASSERT(lua_isnumber(L, -1));
    m.y = (lua_tonumber(L, -1));
    lua_pop(L, 1);

    // right
    lua_rawgeti(L, index, 3);
    DW_ASSERT(lua_isnumber(L, -1));
    m.z = (lua_tonumber(L, -1));
    lua_pop(L, 1);

    return m;
}


//------------------------------------------------------------------------------
/**
*/
template<>
DwUrl _lua_pop_value_of<DwUrl>(lua_State *L, int index)
{
    if (lua_isstring(L, index))
    {
        DwString s = _lua_pop_value_of<DwString>(L, index);
        return DwUrl(s);
    }
    return DwUrl();
}

//------------------------------------------------------------------------------
/**
*/
template<>
DwByteArray _lua_pop_value_of<DwByteArray>(lua_State *L, int index)
{
    if (lua_isstring(L, index))
    {
        size_t len = 0;
        const char* dat = lua_tolstring(L, index, &len);
        if (len > 0)
        {
            return DwByteArray(dat, len);
        }
    }
    return DwByteArray();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<double>(lua_State *L, const double &d)
{
    lua_pushnumber(L, d);
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<float>(lua_State *L, const float &d)
{
    lua_pushnumber(L, d);
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<char>(lua_State *L, const char &d)
{
    lua_pushinteger(L, d);
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<unsigned char>(lua_State *L, const unsigned char &d)
{
    lua_pushinteger(L, d);
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<int>(lua_State *L, const int &d)
{
    lua_pushinteger(L, (d));
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<uint32_t>(lua_State *L, const uint32_t &d)
{
    lua_pushinteger(L, (lua_Integer)(d));
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<int64_t>(lua_State *L, const int64_t &d)
{
    lua_pushnumber(L, (lua_Number)(d));
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<uint64_t>(lua_State *L, const uint64_t &d)
{
    lua_pushnumber(L, (lua_Number)(d));
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
typedef void* LUA_VOID_PTR;
template<>
int _lua_push_value_of<LUA_VOID_PTR>(lua_State *L, const LUA_VOID_PTR &d)
{
    lua_pushlightuserdata(L, (void*)(d));
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<DwRectF>(lua_State *L, const DwRectF &d)
{
    lua_newtable(L);

    lua_pushnumber(L, d.x());
    lua_rawseti(L, -2, 1);

    lua_pushnumber(L, d.x());
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, d.y());
    lua_rawseti(L, -2, 2);

    lua_pushnumber(L, d.y());
    lua_setfield(L, -2, "y");

    lua_pushnumber(L, d.width());
    lua_rawseti(L, -2, 3);

    lua_pushnumber(L, d.width());
    lua_setfield(L, -2, "width");

    lua_pushnumber(L, d.height());
    lua_rawseti(L, -2, 4);

    lua_pushnumber(L, d.height());
    lua_setfield(L, -2, "height");

    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<DwRect>(lua_State *L, const DwRect &d)
{
    return _lua_push_value_of<DwRectF>(L, DwRectF(d));
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<DwPointF>(lua_State *L, const DwPointF &d)
{
    lua_newtable(L);

    lua_pushnumber(L, d.x());
    lua_rawseti(L, -2, 1);

    lua_pushnumber(L, d.y());
    lua_rawseti(L, -2, 2);

    lua_pushnumber(L, d.x());
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, d.y());
    lua_setfield(L, -2, "y");

    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<DwPoint>(lua_State *L, const DwPoint &d)
{
    return _lua_push_value_of<DwPointF>(L, DwPointF(d));
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<DwSizeF>(lua_State *L, const DwSizeF &d)
{
    lua_newtable(L);

    lua_pushnumber(L, d.width());
    lua_rawseti(L, -2, 1);

    lua_pushnumber(L, d.width());
    lua_setfield(L, -2, "width");

    lua_pushnumber(L, d.height());
    lua_rawseti(L, -2, 2);

    lua_pushnumber(L, d.height());
    lua_setfield(L, -2, "height");

    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<DwSize>(lua_State *L, const DwSize &d)
{
    return _lua_push_value_of<DwSizeF>(L, DwSizeF(d));
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<DwString>(lua_State *L, const DwString &d)
{
    if (d.length() > 0)
    {
        lua_pushstring(L, d.toUtf8().data());
    }
    else
    {
        lua_pushstring(L, "");
    }

    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<bool>(lua_State *L, const bool &d)
{
    lua_pushboolean(L, d ? 1 : 0);
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<DwColor>(lua_State *L, const DwColor &d)
{
    lua_newtable(L);

    lua_pushnumber(L, d.red());
    lua_rawseti(L, -2, 1);

    lua_pushnumber(L, d.green());
    lua_rawseti(L, -2, 2);

    lua_pushnumber(L, d.blue());
    lua_rawseti(L, -2, 3);

    lua_pushnumber(L, d.alpha());
    lua_rawseti(L, -2, 4);

    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<DwFont>(lua_State *L, const DwFont &d)
{
    lua_newtable(L);

    _lua_push_value_of<DwString>(L, d.name());
    lua_setfield(L, -2, "name");

    lua_pushinteger(L, (int)d.style());
    lua_setfield(L, -2, "style");

    lua_pushnumber(L, d.size());
    lua_setfield(L, -2, "size");

    _lua_push_value_of<DwColor>(L, d.color());
    lua_setfield(L, -2, "color");

    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<DwMargins>(lua_State *L, const DwMargins &d)
{
    lua_newtable(L);

    // left
    lua_pushinteger(L, d.left());
    lua_rawseti(L, -2, 1);

    lua_pushinteger(L, d.left());
    lua_setfield(L, -2, "left");

    // top
    lua_pushinteger(L, d.top());
    lua_rawseti(L, -2, 2);

    lua_pushinteger(L, d.left());
    lua_setfield(L, -2, "top");

    // right
    lua_pushinteger(L, d.right());
    lua_rawseti(L, -2, 3);

    lua_pushinteger(L, d.right());
    lua_setfield(L, -2, "right");

    // bottom
    lua_pushinteger(L, d.bottom());
    lua_rawseti(L, -2, 4);

    lua_pushinteger(L, d.bottom());
    lua_setfield(L, -2, "bottom");

    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<DwVariant>(lua_State *L, const DwVariant &d)
{
    switch(d.vtype())
    {
    case DwVariant::Char :
        return _lua_push_value_of<char>(L, d.as<char>());
        break;
    case DwVariant::UChar :
        return _lua_push_value_of<unsigned char>(L, d.as<unsigned char>());
        break;
    case DwVariant::Bool :
        return _lua_push_value_of<bool>(L, d.as<bool>());
        break;
    case DwVariant::Int :
        return _lua_push_value_of<int>(L, d.as<int>());
        break;
    case DwVariant::UInt :
        return _lua_push_value_of<uint32_t>(L, d.as<uint32_t>());
        break;
    case DwVariant::Double :
        return _lua_push_value_of<double>(L, d.as<double>());
        break;
    case DwVariant::Float :
        return _lua_push_value_of<float>(L, d.as<float>());
        break;
    case DwVariant::Int64 :
        return _lua_push_value_of<int64_t>(L, d.as<int64_t>());
        break;
    case DwVariant::UInt64 :
        return _lua_push_value_of<uint64_t>(L, d.as<uint64_t>());
        break;
    case DwVariant::Ptr :
        return _lua_push_value_of<void*>(L, d.as<void*>());
        break;
    case DwVariant::RectF :
        return _lua_push_value_of<DwRectF>(L, d.as<DwRectF>());
        break;
    case DwVariant::Rect :
        return _lua_push_value_of<DwRect>(L, d.as<DwRect>());
        break;
    case DwVariant::PointF :
        return _lua_push_value_of<DwPointF>(L, d.as<DwPointF>());
        break;
    case DwVariant::Point :
        return _lua_push_value_of<DwPoint>(L, d.as<DwPoint>());
        break;
    case DwVariant::SizeF :
        return _lua_push_value_of<DwSizeF>(L, d.as<DwSizeF>());
        break;
    case DwVariant::Size :
        return _lua_push_value_of<DwSize>(L, d.as<DwSize>());
        break;
    case DwVariant::String :
        return _lua_push_value_of<DwString>(L, d.as<DwString>());
        break;
    case DwVariant::Margins :
        return _lua_push_value_of<DwMargins>(L, d.as<DwMargins>());
        break;
    default:
        break;
    }

    lua_pushnil(L);
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(lua_State *L, const LUA_GRAPHICSITEM_PTR &d)
{
    if (!d)
    {
        lua_pushnil(L);
        return 1;
    }

    lua_getglobal(L, "GET_GUI_PROXY");
    if (lua_isfunction(L, -1))
    {
        lua_pushinteger(L, d->id());
        int res = lua_pcall(L, 1, 1, 0);
        if (res != 0)
        {
            DwString s = lua_tostring(L, -1);
            DW_UI_LOGER() << "script error: " << s << "\n";
            lua_pop(L, 1); //  pop the error
            DW_ASSERT(false); // go on to push id
        }
        else
        {
            // return the right proxy
            DW_ASSERT(lua_istable(L, -1));
            return 1;
        }
    }

    lua_pop(L, 1);
    lua_pushinteger(L, d->id());

    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<LUA_ANIMATION_PTR>(lua_State *L, const LUA_ANIMATION_PTR &d)
{
    if (!d)
    {
        lua_pushnil(L);
        return 1;
    }

    lua_pushinteger(L, d->id());
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
static int _lua_helper_image_release(lua_State *L)
{
    DwImage *d = *(DwImage **)lua_touserdata(L, 1);
    DW_SAFE_DELETE(d);
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<DwImage>(lua_State *L, const DwImage &d)
{
    lua_getglobal(L, "GET_IMAGE_PROXY");
    if (lua_isfunction(L, -1))
    {
        DwImage *image = new DwImage(d);
        DwImage **ppimage = (DwImage **)lua_newuserdata(L, sizeof(DwImage*));
        *ppimage = image;

        // meta table
        lua_getglobal(L, "_lua_image_metatable");
        if(lua_isnil(L, -1))
        {
            lua_pop(L, 1);// pop nil
            luaL_newmetatable(L, "_lua_image_metatable");

            lua_pushcfunction(L, _lua_helper_image_release);
            lua_setfield(L, -2, "__gc");
        }
        DW_ASSERT(lua_istable(L, -1));
        lua_setmetatable(L, -2);

        int res = lua_pcall(L, 1, 1, 0);
        if (res != 0)
        {
            DwString s = lua_tostring(L, -1);
            DW_UI_LOGER() << "script error: " << s << "\n";
            lua_pop(L, 1); //  pop the error
            DW_ASSERT(false); // go on to push id
        }
        else
        {
            // return the right proxy
            DW_ASSERT(lua_istable(L, -1));
            return 1;
        }
    }

    lua_pop(L, 1);
    DwImage *image = new DwImage(d);
    DwImage **ppimage = (DwImage **)lua_newuserdata(L, sizeof(DwImage*));
    *ppimage = image;

    // meta table
    lua_getglobal(L, "_lua_image_metatable");
    if(lua_isnil(L, -1))
    {
        lua_pop(L, 1);// pop nil
        luaL_newmetatable(L, "_lua_image_metatable");

        lua_pushcfunction(L, _lua_helper_image_release);
        lua_setfield(L, -2, "__gc");
    }
    DW_ASSERT(lua_istable(L, -1));
    lua_setmetatable(L, -2);

    return 1;
}

//------------------------------------------------------------------------------
/**
*/
typedef DwAnchors* LUA_ANCHOR_PTR;
template<>
int _lua_push_value_of<LUA_ANCHOR_PTR>(lua_State *L, const LUA_ANCHOR_PTR &d)
{
    if (!d)
    {
        lua_pushnil(L);
        return 1;
    }

    lua_getglobal(L, "GET_ANCHOR_PROXY");
    if (lua_isfunction(L, -1))
    {
        _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, d->item());
        int res = lua_pcall(L, 1, 1, 0);
        if (res != 0)
        {
            DwString s = lua_tostring(L, -1);
            DW_UI_LOGER() << "script error: " << s << "\n";
            lua_pop(L, 1); //  pop the error
            DW_ASSERT(false); // go on to push id
        }
        else
        {
            // return the right proxy
            DW_ASSERT(lua_istable(L, -1));
            return 1;
        }
    }

    lua_pop(L, 1);
    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, d->item());
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<LUA_RTTI_PTR>(lua_State *L, const LUA_RTTI_PTR &d)
{
    if (!d)
    {
        lua_pushnil(L);
        return 1;
    }

    return _lua_push_value_of<DwFourCC>(L, d->fourCC());
}

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<LUA_3Int>(lua_State *L, const LUA_3Int &d)
{
    lua_newtable(L);

    lua_pushnumber(L, d.x);
    lua_rawseti(L, -2, 1);

    lua_pushnumber(L, d.y);
    lua_rawseti(L, -2, 2);

    lua_pushnumber(L, d.z);
    lua_rawseti(L, -2, 3);

    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
DW_LUABIND_EXPORT int _lua_push_value_of<LUA_3Real>(lua_State *L, const LUA_3Real &d)
{
    lua_newtable(L);

    lua_pushnumber(L, d.x);
    lua_rawseti(L, -2, 1);

    lua_pushnumber(L, d.y);
    lua_rawseti(L, -2, 2);

    lua_pushnumber(L, d.z);
    lua_rawseti(L, -2, 3);

    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<DwUrl>(lua_State *L, const DwUrl &d)
{
    return _lua_push_value_of<DwString>(L, d.asString());
}

//------------------------------------------------------------------------------
/**
*/
template<>
int _lua_push_value_of<DwByteArray>(lua_State *L, const DwByteArray &d)
{
    lua_pushlstring(L, d.constData(), d.size());
    return 1;
}