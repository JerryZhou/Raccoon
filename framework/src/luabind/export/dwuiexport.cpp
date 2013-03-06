#include "stable.h"
#include "dwuiexport.h"
#include "dwluabind/dwluaengine.h"
#include "dwluabind/dwluaitemlib.h"
#include "dwluabind/dwdelegatelib.h"
#include "dwluabind/dwitemdelegate.h"
#include "dwgui/dwgraphicsitem.h"
#include "dwgui/dwframewindow.h"
#include "dwgui/dwfactory.h"
#include "dwuiexport_p.h"

DW_IMPLEMENT_CLASS(DwUIExport, 'UIRT', DwLuaExport);

//------------------------------------------------------------------------------
/**
*/
DwUIExport::DwUIExport()
{
    setCallExportWhen(Dw::CallWhenItemDestroy);
}

//------------------------------------------------------------------------------
/**
*/
DwUIExport::~DwUIExport()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
bool DwUIExport::exportd( DwLuaEngine *engine )
{
    luaL_register(engine->luaState(), "GUI", uiLib);
    bool isOk = engine->load("script:ui/ui.lua");
    isOk = isOk && engine->load("script:ui/uicom.lua");
    return isOk;
}

//------------------------------------------------------------------------------
/**
*/
static int _script_call_destroy_item(DwGraphicsItem *item, lua_State *L)
{
    lua_getglobal(L, "GUI_CLEAR_PROXY_OF");
    if (lua_isfunction(L, -1))
    {
        lua_pushinteger(L, item->id());
        int res = lua_pcall(L, 1, 0, 0);
        if (res != 0)
        {
            DwString s = lua_tostring(L, -1);
            DW_UI_LOGER() << "script error: " << s << "\n";
            lua_pop(L, 1); //  pop the error
            DW_ASSERT(false); // go on to push id
        }
        return 0;
    }
    lua_pop(L, 1);
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
bool DwUIExport::onItemDestroy(DwGraphicsItem *item)
{
    lua_State *L = m_engine->luaState();

    _script_call_destroy_item(item, L);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t DwUIExport::createItem(const DwString &n, DwGraphicsItem *parent)
{
    lua_State *L = m_engine->luaState();
    DW_UNUSED(L);

    uint32_t id = 0;
    const DwRtti *rtti = dwFactory()->classRttiOf(n);
    if (rtti && rtti->isDerivedFrom(DwGraphicsItem::RTTI()))
    {
        DwRttiObject *obj = rtti->create();
        DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
        if (item)
        {
            id = item->id();
            if (parent)
            {
                item->setParentItem(parent);
            }
        }

        DwFrameWindow *w = dwsafe_cast<DwFrameWindow>(obj);
        if (w)
        {
            w->setAttribute(Dw::WA_DeleteOnClose);
        }
    }

    return id;
}

//------------------------------------------------------------------------------
/**
*/
int DwUIExport::destroyItem(DwGraphicsItem *item)
{
    lua_State *L = m_engine->luaState();

    if (item)
    {
        item->deleteLater();
        lua_pushboolean(L, true);
    }
    else
    {
        lua_pushboolean(L, false);
    }
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
int DwUIExport::setProperty(DwGraphicsItem *item, const DwString &key)
{
    lua_State *L = m_engine->luaState();

    int n = 0;
    if (item)
    {
        DwDelegate* dgate = _delegateOf(item->rtti());
        while (dgate && dgate->rtti()->isDerivedFrom(DwItemDelegate::RTTI()))
        {
            DwDelegate::fn_delegate_set set =  dgate->entrySet(key);
            if (set)
            {
                n = set(item, L);
                break;
            }

            dgate = dgate->parent();
            if (!dgate)
            {
                DW_ASSERT(false && "not exits of property");
                DW_UI_LOGER() << "ui_setProperty error, not exits of property: " << key << "\n";
            }
        }
    }
    else
    {
        DW_ONCE_ASSERT(false);
        DW_UI_LOGER() << "ui_setProperty error, not exits of item: " << key << "\n";
    }
    return n;
}

//------------------------------------------------------------------------------
/**
*/
int DwUIExport::propertyOf(DwGraphicsItem *item, const DwString &key)
{
    lua_State *L = m_engine->luaState();
    int n = 0;

    if (item)
    {
        DwDelegate* dgate = _delegateOf(item->rtti());
        while (dgate && dgate->rtti()->isDerivedFrom(DwItemDelegate::RTTI()))
        {
            DwDelegate::fn_delegate_get get =  dgate->entryGet(key);
            if (get)
            {
                n = get(item, L);
                break;
            }

            dgate = dgate->parent();
            if (!dgate)
            {
                DW_ASSERT(false && "not exits of property");
                DW_UI_LOGER() << "ui_propertyOf error, not exits of property: " << key << "\n";
            }
        }
    }
    else
    {
        DW_ONCE_ASSERT(false);
        DW_UI_LOGER() << "ui_propertyOf error, not exits of item: " << key << "\n";
    }
    return n;
}

//------------------------------------------------------------------------------
/**
*/
int DwUIExport::call(DwGraphicsItem *item, const DwString &key)
{
    lua_State *L = m_engine->luaState();
    int n = 0;

    if (item)
    {
        DwDelegate* dgate = _delegateOf(item->rtti());
        while (dgate && dgate->rtti()->isDerivedFrom(DwItemDelegate::RTTI()))
        {
            DwDelegate::fn_delegate_call call =  dgate->entryCall(key);
            if (call)
            {
                n = call(item, L);
                break;
            }

            dgate = dgate->parent();
            if (!dgate)
            {
                DW_ASSERT(false && "not exits of property");
                DW_UI_LOGER() << "ui_call error, not exits of property: " << key << "\n";
            }
        }
    }
    else
    {
        if (key == "isNull")
        {
            n = _lua_push_value_of<bool>(L, true);
        }
        else if (key == "isValid")
        {
            n = _lua_push_value_of<bool>(L, false);
        }
        else
        {
            DW_ONCE_ASSERT(false);
            DW_UI_LOGER() << "ui_call error, not exits of item: " << key << "\n";
        }
    }
    return n;
}

//------------------------------------------------------------------------------
/**
*/
int DwUIExport::registerItem(DwGraphicsItem *item)
{
    DW_UNUSED(item);
    return 0;
}
