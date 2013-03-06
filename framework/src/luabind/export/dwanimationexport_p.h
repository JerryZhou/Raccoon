#pragma once
#include "dwexportutil_p.h"
#include "dwgui/dwpropertys.h"
#include "dwgui/dwpropertyanimation.h"

//------------------------------------------------------------------------------
/**
*/
static int animation_create(lua_State *L)
{
    DW_UNUSED(L);
    DwPropertyAnimation *anim = new DwPropertyAnimation(NULL);
    return _lua_push_value_of<LUA_ANIMATION_PTR>(L, anim);
}

//------------------------------------------------------------------------------
/**
    (anim)
*/
static int animation_destroy(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 1);
    DwAbstractAnimation *anim = _lua_pop_value_of<LUA_ANIMATION_PTR>(L, 1);
    if (anim)
    {
        DW_SAFE_DELETE(anim);
        lua_pushboolean(L, true);
        return 1;
    }
    lua_pushboolean(L, false);
    return 1;
}

//------------------------------------------------------------------------------
/**
    (anim, policy)
*/
static int animation_start(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 2);
    DwAbstractAnimation *anim = _lua_pop_value_of<LUA_ANIMATION_PTR>(L, 1);
    if (anim)
    {
        anim->start((Dw::DeletionPolicy)(_lua_pop_value_of<int>(L, 2)));
        lua_pushboolean(L, true);
        return 1;
    }
    lua_pushboolean(L, false);
    return 1;
}

//------------------------------------------------------------------------------
/**
    (anim)
*/
static int animation_stop(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 1);
    DwAbstractAnimation *anim = _lua_pop_value_of<LUA_ANIMATION_PTR>(L, 1);
    if (anim)
    {
        anim->stop();
        lua_pushboolean(L, true);
        return 1;
    }
    lua_pushboolean(L, false);
    return 1;
}

//------------------------------------------------------------------------------
/**
    (anim, delta)
*/
static int animation_setDuration(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 2);
    DwAbstractAnimation *anim = _lua_pop_value_of<LUA_ANIMATION_PTR>(L, 1);
    if (anim)
    {
        anim->setDuration(_lua_pop_value_of<int>(L, 2));
        lua_pushboolean(L, true);
        return 1;
    }
    lua_pushboolean(L, false);
    return 1;
}


//------------------------------------------------------------------------------
/**
    (anim, item)
*/
static int animation_setTarget(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 2);
    DwAbstractAnimation *anim = _lua_pop_value_of<LUA_ANIMATION_PTR>(L, 1);
    if (anim)
    {
        DwGraphicsItem *item = _lua_itemOf(L, 2);
        if (item)
        {
            anim->attachTo(item);
            lua_pushboolean(L, true);
            return 1;
        }
    }
    lua_pushboolean(L, false);
    return 1;
}

//------------------------------------------------------------------------------
/**
    (anim, property)
*/
static int animation_setProperty(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 2);
    DwAbstractAnimation *anim = _lua_pop_value_of<LUA_ANIMATION_PTR>(L, 1);
    if (anim)
    {
        DwPropertyAnimation *panim = dwsafe_cast<DwPropertyAnimation>(anim);
        if (panim)
        {
            panim->setProperty(_lua_pop_value_of<DwString>(L, 2));
            lua_pushboolean(L, true);
            return 1;
        }
    }
    lua_pushboolean(L, false);
    return 1;
}

//------------------------------------------------------------------------------
/**
    (anim, step, v)
*/
static int animation_setStep(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 3);
    DwAbstractAnimation *anim = _lua_pop_value_of<LUA_ANIMATION_PTR>(L, 1);
    if (anim)
    {
        DwVariantAnimation *vanim = dwsafe_cast<DwVariantAnimation>(anim);
        if (vanim)
        {
            vanim->setKeyValueAt(_lua_pop_value_of<DwReal>(L, 2), _lua_pop_value_of<DwVariant>(L, 3));
            lua_pushboolean(L, true);
            return 1;
        }
    }
    lua_pushboolean(L, false);
    return 1;
}

//------------------------------------------------------------------------------
/**
    (anim, curve)
*/
static int animation_setEasingCurve(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 2);
    DwAbstractAnimation *anim = _lua_pop_value_of<LUA_ANIMATION_PTR>(L, 1);
    if (anim)
    {
        DwVariantAnimation *vanim = dwsafe_cast<DwVariantAnimation>(anim);
        if (vanim)
        {
            vanim->setEasingCurve(DwEasingCurve((Dw::Curve)(_lua_pop_value_of<int>(L, 2))));
            lua_pushboolean(L, true);
            return 1;
        }
    }
    lua_pushboolean(L, false);
    return 1;
}

//------------------------------------------------------------------------------
/**
    (anim, b)
*/
static int animation_setSendEvent(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 2);
    DwAbstractAnimation *anim = _lua_pop_value_of<LUA_ANIMATION_PTR>(L, 1);
    if (anim)
    {
        anim->setSendEvent(_lua_pop_value_of<bool>(L, 2));
    }
    return 0;
}
//------------------------------------------------------------------------------
/**
*/
static const struct luaL_reg animationLib [] =
{
    {"create", animation_create},
    {"destroy", animation_destroy},
    {"start", animation_start},
    {"stop", animation_stop},
    {"setDuration", animation_setDuration},
    {"setTarget", animation_setTarget},
    {"setProperty", animation_setProperty},
    {"setStep", animation_setStep},
    {"setEasingCurve", animation_setEasingCurve},
    {"setSendEvent", animation_setSendEvent},
    {NULL, NULL} /* sentinel */
};