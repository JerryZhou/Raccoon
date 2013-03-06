#pragma once
#include "dwexportutil_p.h"
#include "dwgui/dwanchors.h"


//------------------------------------------------------------------------------
/**
*/
bool _lua_push_i_anchorline(lua_State *L, const DwAnchorLine& anchor)
{
    lua_newtable(L);

    lua_pushinteger(L, anchor.item->id());
    lua_setfield(L, -2, "item");

    lua_pushinteger(L, (int)(anchor.anchorLine));
    lua_setfield(L, -2, "line");

    return true;
}

bool _lua_pop_i_anchorline(lua_State *L, DwAnchorLine& anchor)
{
    if ( lua_istable(L, -1) )
    {
        lua_getfield(L, -1, "item");
        anchor.item = _lua_itemOf(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "line");
        anchor.anchorLine = (Dw::AnchorLine ) ( lua_tointeger(L, -1) );
        lua_pop(L, 1);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool _lua_push_anchorline(lua_State *L, int index, const char* s, const DwAnchorLine& anchor)
{
    if (anchor.item)
    {
        _lua_push_i_anchorline(L, anchor);
        lua_setfield(L, index, s);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool _lua_pop_anchorline(lua_State *L, int index, const char* s, DwAnchorLine& anchor)
{
    bool isOk = false;
    lua_getfield(L, index, s);
    isOk = _lua_pop_i_anchorline(L, anchor);

    lua_pop(L, 1);

    return isOk;
}

//------------------------------------------------------------------------------
/**
    anchorOf(item)
*/
static int anchor_anchorOf(lua_State *L)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        lua_newtable(L);

        DwAnchors *anchors = item->anchors();

        DwAnchorLine lanchor = anchors->left();
        DwAnchorLine ranchor = anchors->right();
        DwAnchorLine tanchor = anchors->top();
        DwAnchorLine banchor = anchors->bottom();

        DwAnchorLine hcanchor = anchors->horizontalCenter();
        DwAnchorLine vcanchor = anchors->verticalCenter();

        DwGraphicsItem * fanchor = anchors->fill();
        DwGraphicsItem * canchor = anchors->centerIn();

        _lua_push_anchorline(L, 2, "left", lanchor);
        _lua_push_anchorline(L, 2, "right", ranchor);
        _lua_push_anchorline(L, 2, "top", tanchor);
        _lua_push_anchorline(L, 2, "bottom", banchor);
        _lua_push_anchorline(L, 2, "hcenter", hcanchor);
        _lua_push_anchorline(L, 2, "vcenter", vcanchor);
        _lua_push_anchorline(L, 2, "fill", DwAnchorLine(fanchor, Dw::Invalid));
        _lua_push_anchorline(L, 2, "centerIn", DwAnchorLine(canchor, Dw::Invalid));
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setAnchor(item, Anchors)
    Anchors = {left = AnchorLine, right = AnchorLine }
    AnchorLine = { item = target, line = l }
*/
static int anchor_setAnchor(lua_State *L)
{
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        item->clearAnchors();
        DwAnchorLine anchor;

        if( _lua_pop_anchorline(L, 2, "left", anchor) )
        {
            item->anchors()->setAnchor(Dw::LeftAnchor, anchor);
        }

        if ( _lua_pop_anchorline(L, 2, "right", anchor))
        {
            item->anchors()->setAnchor(Dw::RightAnchor, anchor);
        }

        if ( _lua_pop_anchorline(L, 2, "top", anchor))
        {
            item->anchors()->setAnchor(Dw::TopAnchor, anchor);
        }

        if ( _lua_pop_anchorline(L, 2, "bottom", anchor))
        {
            item->anchors()->setAnchor(Dw::BottomAnchor, anchor);
        }

        if ( _lua_pop_anchorline(L, 2, "hcenter", anchor))
        {
            item->anchors()->setAnchor(Dw::HCenterAnchor, anchor);
        }

        if ( _lua_pop_anchorline(L, 2, "vcenter", anchor))
        {
            item->anchors()->setAnchor(Dw::VCenterAnchor, anchor);
        }

        if ( _lua_pop_anchorline(L, 2, "fill", anchor))
        {
            item->anchors()->setFill(anchor.item);
        }

        if ( _lua_pop_anchorline(L, 2, "centerIn", anchor))
        {
            item->anchors()->setCenterIn(anchor.item);
        }
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
    setLeft(item, anchorline)
*/
static int anchor_setLeft(lua_State *L)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        DwAnchorLine anchor;
        _lua_pop_i_anchorline(L, anchor);
        item->anchors()->setAnchor(Dw::LeftAnchor, anchor);
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setRight(item, anchorline)
*/
static int anchor_setRight(lua_State *L)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        DwAnchorLine anchor;
        _lua_pop_i_anchorline(L, anchor);
        item->anchors()->setAnchor(Dw::RightAnchor, anchor);
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setTop(item, anchorline)
*/
static int anchor_setTop(lua_State *L)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        DwAnchorLine anchor;
        _lua_pop_i_anchorline(L, anchor);
        item->anchors()->setAnchor(Dw::TopAnchor, anchor);
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setBottom(item, anchorline)
*/
static int anchor_setBottom(lua_State *L)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        DwAnchorLine anchor;
        _lua_pop_i_anchorline(L, anchor);
        item->anchors()->setAnchor(Dw::BottomAnchor, anchor);
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setFill(item, anchorline)
*/
static int anchor_setFill(lua_State *L)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        DwAnchorLine anchor;
        _lua_pop_i_anchorline(L, anchor);
        item->anchors()->setFill(anchor.item);
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setCenterIn(item, anchorline)
*/
static int anchor_setCenterIn(lua_State *L)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        DwAnchorLine anchor;
        _lua_pop_i_anchorline(L, anchor);
        item->anchors()->setCenterIn(anchor.item);
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setHCenter(item, anchorline)
*/
static int anchor_setHCenter(lua_State *L)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        DwAnchorLine anchor;
        _lua_pop_i_anchorline(L, anchor);
        item->anchors()->setAnchor(Dw::HCenterAnchor, anchor);
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setVCenter(item, anchorline)
*/
static int anchor_setVCenter(lua_State *L)
{
    DW_UNUSED(L);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        DwAnchorLine anchor;
        _lua_pop_i_anchorline(L, anchor);
        item->anchors()->setAnchor(Dw::VCenterAnchor, anchor);
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setMargins(item, margins)
*/
static int anchor_setMargins(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 2);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        item->anchors()->setMargins(lua_tonumber(L, 2));
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setLeftMargin(item, margin)
*/
static int anchor_setLeftMargin(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 2);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        item->anchors()->setLeftMargin(lua_tonumber(L, 2));
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setRightMargin(item, margin)
*/
static int anchor_setRightMargin(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 2);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        item->anchors()->setRightMargin(lua_tonumber(L, 2));
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setTopMargin(item, margin)
*/
static int anchor_setTopMargin(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 2);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        item->anchors()->setTopMargin(lua_tonumber(L, 2));
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setBottomMargin(item, margin)
*/
static int anchor_setBottomMargin(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 2);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        item->anchors()->setBottomMargin(lua_tonumber(L, 2));
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setHCenterOffset(item, margin)
*/
static int anchor_setHCenterOffset(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 2);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        item->anchors()->setHorizontalCenterOffset(lua_tonumber(L, 2));
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    setVCenterOffset(item, margin)
*/
static int anchor_setVCenterOffset(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 2);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        item->anchors()->setVerticalCenterOffset(lua_tonumber(L, 2));
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    leftAnchor(item)
*/
static int anchor_leftAnchor(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        _lua_push_i_anchorline(L, item->anchors()->left());
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    rightAnchor(item)
*/
static int anchor_rightAnchor(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        _lua_push_i_anchorline(L, item->anchors()->right());
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    topAnchor(item)
*/
static int anchor_topAnchor(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        _lua_push_i_anchorline(L, item->anchors()->top());
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    bottomAnchor(item)
*/
static int anchor_bottomAnchor(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        _lua_push_i_anchorline(L, item->anchors()->bottom());
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    fillAnchor(item)
*/
static int anchor_fillAnchor(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        _lua_push_i_anchorline(L, DwAnchorLine(item->anchors()->fill(), Dw::Invalid));
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    centerInAnchor(item)
*/
static int anchor_centerInAnchor(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        _lua_push_i_anchorline(L, DwAnchorLine(item->anchors()->centerIn(), Dw::Invalid));
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    hCenterAnchor(item)
*/
static int anchor_hCenterAnchor(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        _lua_push_i_anchorline(L, item->anchors()->horizontalCenter());
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    vCenterAnchor(item)
*/
static int anchor_vCenterAnchor(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        _lua_push_i_anchorline(L, item->anchors()->verticalCenter());
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    margins(item)
*/
static int anchor_margins(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        lua_pushnumber(L, item->anchors()->margins());
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    leftMargin(item)
*/
static int anchor_leftMargin(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        lua_pushnumber(L, item->anchors()->leftMargin());
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    rightMargin(item)
*/
static int anchor_rightMargin(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        lua_pushnumber(L, item->anchors()->rightMargin());
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    topMargin(item)
*/
static int anchor_topMargin(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        lua_pushnumber(L, item->anchors()->topMargin());
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    bottomMargin(item)
*/
static int anchor_bottomMargin(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        lua_pushnumber(L, item->anchors()->bottomMargin());
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    hCenterOffset(item)
*/
static int anchor_hCenterOffset(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        lua_pushnumber(L, item->anchors()->horizontalCenterOffset());
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    vCenterOffset(item)
*/
static int anchor_vCenterOffset(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 1);
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item && item->hasAnchors())
    {
        lua_pushnumber(L, item->anchors()->verticalCenterOffset());
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
static const struct luaL_reg anchorLib [] =
{
    // set an get the item's anchor
    {"anchorOf", anchor_anchorOf},
    {"setAnchor", anchor_setAnchor},

    // set the anchor's left, right, top, bottom
    {"setLeft", anchor_setLeft},
    {"setRight", anchor_setRight},
    {"setTop", anchor_setTop},
    {"setBottom", anchor_setBottom},

    // set the anchor's fill, centerIn, hcenter, vcenter
    {"setFill", anchor_setFill},
    {"setCenterIn", anchor_setCenterIn},
    {"setHCenter", anchor_setHCenter},
    {"setVCenter", anchor_setVCenter},

    // set the anchor's marign
    {"setMargins", anchor_setMargins},
    {"setLeftMargin", anchor_setLeftMargin},
    {"setRightMargin", anchor_setRightMargin},
    {"setTopMargin", anchor_setTopMargin},
    {"setBottomMargin", anchor_setBottomMargin},
    {"setHCenterOffset", anchor_setHCenterOffset},
    {"setVCenterOffset", anchor_setVCenterOffset},

    // get the anchor's left, right, top, bottom
    {"leftAnchor", anchor_leftAnchor},
    {"rightAnchor", anchor_rightAnchor},
    {"topAnchor", anchor_topAnchor},
    {"bottomAnchor", anchor_bottomAnchor},

    // get the anchor's fill, centerIn, hcenter, vcenter
    {"fillAnchor", anchor_fillAnchor},
    {"centerInAnchor", anchor_centerInAnchor},
    {"hCenterAnchor", anchor_hCenterAnchor},
    {"vCenterAnchor", anchor_vCenterAnchor},

    // get the anchor's marigns...
    {"margins", anchor_margins},
    {"leftMargin", anchor_leftMargin},
    {"rightMargin", anchor_rightMargin},
    {"topMargin", anchor_topMargin},
    {"bottomMargin", anchor_bottomMargin},
    {"hCenterOffset", anchor_hCenterOffset},
    {"vCenterOffset", anchor_vCenterOffset},

    {NULL, NULL} /* sentinel */
};