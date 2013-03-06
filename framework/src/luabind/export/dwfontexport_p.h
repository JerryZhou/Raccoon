#pragma once
#include "dwexportutil_p.h"
#include "dwgraphics/dwfont.h"

//------------------------------------------------------------------------------
/**
    new(name)
    new(size)
    new(name, size)
    new(name, size, style)
    new(name, size, style, color)
*/
static int font_create(lua_State *L)
{
    if (lua_gettop(L) == 1)
    {
        if (lua_isstring(L, -1))
        {
            return _lua_push_value_of<DwFont>(L, DwFont(_lua_pop_value_of<DwString>(L, -1)));
        }
        else if (lua_isnumber(L, -1))
        {
            return _lua_push_value_of<DwFont>(L,
                                              DwFont(DwFont::defaultName(), _lua_pop_value_of<DwReal>(L, -1)));
        }
    }
    else if (lua_gettop(L) >= 2)
    {
        DwString name = lua_isstring(L, 1) ? _lua_pop_value_of<DwString>(L, 1) : DwFont::defaultName();
        DwReal size = lua_isnumber(L, 2) ? _lua_pop_value_of<DwReal>(L, 2) : DwFont::defaultSize();

        DwFont f(name, size);

        if (lua_gettop(L) >= 3 && lua_isnumber(L, 3))
        {
            f.setStyle((Dw::FontStyle)_lua_pop_value_of<int>(L, 3));
        }

        if (lua_gettop(L) >= 4 && lua_istable(L, 4))
        {
            f.setColor(_lua_pop_value_of<DwColor>(L, 4));
        }

        return _lua_push_value_of<DwFont>(L, f);
    }

    return _lua_push_value_of<DwFont>(L, DwFont());
}

//------------------------------------------------------------------------------
/**
*/
static int font_exists(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) == 1);
    DwFont f = _lua_pop_value_of<DwFont>(L , -1);
    return _lua_push_value_of<bool>(L, f.isAvailable());
}

//------------------------------------------------------------------------------
/**
    font, string, layout/width/nil
*/
static int font_measureString(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) >= 2);

    DwFont f = _lua_pop_value_of<DwFont>(L , 1);
    DwString s = _lua_pop_value_of<DwString>(L, 2);
    if (lua_gettop(L) > 2)
    {
        if (lua_isnumber(L, 3))
        {
            DwReal width = _lua_pop_value_of<DwReal>(L, 3);
            return _lua_push_value_of<DwRectF>(L, f.MeasureString(s, width));
        }
        else if (lua_istable(L, 3))
        {
            DwRectF layout = _lua_pop_value_of<DwRectF>(L, 3);
            return _lua_push_value_of<DwRectF>(L, f.MeasureString(s, layout));
        }
    }

    return _lua_push_value_of<DwRectF>(L, f.MeasureString(s));
}

//------------------------------------------------------------------------------
/**
*/
static int font_setDefaultName(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) == 1);
    DwString s = _lua_pop_value_of<DwString>(L, -1);
    DwFont::setDefaultName(s);
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
static int font_defaultName(lua_State *L)
{
    return _lua_push_value_of<DwString>(L, DwFont::defaultName());
}

//------------------------------------------------------------------------------
/**
*/
static int font_setDefaultSize(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) == 1);
    DwReal s = _lua_pop_value_of<DwReal>(L, -1);
    DwFont::setDefaultSize(s);
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
static int font_defaultSize(lua_State *L)
{
    return _lua_push_value_of<DwReal>(L, DwFont::defaultSize());
}

//------------------------------------------------------------------------------
/**
*/
static int font_setDefaultStyle(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) == 1);
    int s = _lua_pop_value_of<int>(L, -1);
    DwFont::setDefaultStyle((Dw::FontStyle)s);
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
static int font_defaultStyle(lua_State *L)
{
    return _lua_push_value_of<int>(L, (int)(DwFont::defaultStyle()));
}

//------------------------------------------------------------------------------
/**
*/
static int font_setDefaultColor(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) == 1);
    DwColor s = _lua_pop_value_of<DwColor>(L, -1);
    DwFont::setDefaultColor(s);
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
static int font_defaultColor(lua_State *L)
{
    return _lua_push_value_of<DwColor>(L, DwFont::defaultColor());
}

//------------------------------------------------------------------------------
/**
*/
static const struct luaL_reg fontLib [] =
{
    {"create", font_create},

    {"exists", font_exists},

    {"measureString", font_measureString},

    {"setDefaultName", font_setDefaultName},
    {"defaultName", font_defaultName},

    {"setDefaultSize", font_setDefaultSize},
    {"defaultSize", font_defaultSize},

    {"setDefaultStyle", font_setDefaultStyle},
    {"defaultStyle", font_defaultStyle},

    {"setDefaultColor", font_setDefaultColor},
    {"defaultColor", font_defaultColor},

    {NULL, NULL} /* sentinel */
};