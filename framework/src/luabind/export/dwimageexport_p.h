#pragma once
#include "dwexportutil_p.h"
#include "dwgraphics/dwimage.h"

//------------------------------------------------------------------------------
/**
    new(path)
*/
static int image_create(lua_State *L)
{
    DwImage img = _lua_pop_value_of<DwImage>(L, 1);
    return _lua_push_value_of<DwImage>(L, img);
}

//------------------------------------------------------------------------------
/**
    (img)
*/
static int image_size(lua_State *L)
{
    DwImage img = _lua_pop_value_of<DwImage>(L, 1);
    return _lua_push_value_of<DwSize>(L, img.size());
}

//------------------------------------------------------------------------------
/**
    (img)
*/
static int image_isNull(lua_State *L)
{
    DwImage img = _lua_pop_value_of<DwImage>(L, 1);
    return _lua_push_value_of<bool>(L, img.isNull());
}

//------------------------------------------------------------------------------
/**
    (img)
*/
static int image_uniqueKey(lua_State *L)
{
    DwImage img = _lua_pop_value_of<DwImage>(L, 1);
    return _lua_push_value_of<int>(L, img.uniqueKey());
}

//------------------------------------------------------------------------------
/**
    (img)
*/
static int image_path(lua_State *L)
{
    DwImage img = _lua_pop_value_of<DwImage>(L, 1);
    return _lua_push_value_of<DwString>(L, img.path());
}

//------------------------------------------------------------------------------
/**
    (img)
*/
static int image_frameCount(lua_State *L)
{
    DwImage img = _lua_pop_value_of<DwImage>(L, 1);
    return _lua_push_value_of<int>(L, img.frameCount());
}

//------------------------------------------------------------------------------
/**
    (img, curFrame)
*/
static int image_curActiveFrame(lua_State *L)
{
    DwImage img = _lua_pop_value_of<DwImage>(L, 1);
    return _lua_push_value_of<int>(L, img.curActiveFrame());
}

//------------------------------------------------------------------------------
/**
    (img)
*/
static int image_setActiveFrame(lua_State *L)
{
    DwImage img = _lua_pop_value_of<DwImage>(L, 1);
    img.setActiveFrame(_lua_pop_value_of<int>(L, 2));
    return 0;
}

//------------------------------------------------------------------------------
/**
    (img)
*/
static int image_nextInterval(lua_State *L)
{
    DwImage img = _lua_pop_value_of<DwImage>(L, 1);
    return _lua_push_value_of<int>(L, img.nextInterval());
}

//------------------------------------------------------------------------------
/**
    (cout)
*/
static int image_shrinkCaches(lua_State *L)
{
    int ncout = -1;
    if (lua_gettop(L) >= 1)
    {
        ncout = _lua_pop_value_of<int>(L,1);
    }
    DwImage::shrinkCaches(ncout);
    return 0;
}

//------------------------------------------------------------------------------
/**
    (img)
*/
static int image_hasCachedOf(lua_State *L)
{
    DwString s = _lua_pop_value_of<DwString>(L, 1);
    return _lua_push_value_of<bool>(L, DwImage::hasCachedOf(DwUrl(s).asString()));
}

//------------------------------------------------------------------------------
/**
*/
static const struct luaL_reg imageLib [] =
{
    {"create", image_create},
    {"size", image_size},
    {"isNull", image_isNull},
    {"uniqueKey", image_uniqueKey},
    {"path", image_path},
    {"frameCount", image_frameCount},
    {"curActiveFrame", image_curActiveFrame},
    {"setActiveFrame", image_setActiveFrame},
    {"nextInterval", image_nextInterval},
    {"shrinkCaches", image_shrinkCaches},
    {"hasCachedOf", image_hasCachedOf},
    {NULL, NULL} /* sentinel */
};