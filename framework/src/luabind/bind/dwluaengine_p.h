#pragma once
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwringlogcase.h"
#include "dwcore/dwtextcodecs.h"

#define LUA_URLPATH       "DW_LUA_URLPATH"

#define LUA_URLPATH_DEFAULT \
    "?.lua;?;" "script:?.lua;script:?;" "bin:?.lua;bin:?;" "home:?.lua;home:?"

/*
** {======================================================
** 'user require' function
** =======================================================
*/
static int readable (const char *filename) {
    bool b = DwResourceInterface::hasExist(DwUrl(DwString::fromUtf8(filename)));
    return b ? 1 : 0;
}

static const char *pushnexttemplate (lua_State *L, const char *path) {
    const char *l;
    while (*path == *LUA_PATHSEP) path++;  /* skip separators */
    if (*path == '\0') return NULL;  /* no more templates */
    l = strchr(path, *LUA_PATHSEP);  /* find next separator */
    if (l == NULL) l = path + strlen(path);
    lua_pushlstring(L, path, l - path);  /* template */
    return l;
}

static const char *findfile (lua_State *L, const char *name,
                             const char *pname) {
     const char *path;
     name = luaL_gsub(L, name, ".", LUA_DIRSEP);
     lua_getglobal(L, "package");
     lua_getfield(L, -1, pname);
     lua_remove(L, -2);
     path = lua_tostring(L, -1);
     if (path == NULL)
         luaL_error(L, LUA_QL("package.%s") " must be a string", pname);
     lua_pushliteral(L, "");  /* error accumulator */
     while ((path = pushnexttemplate(L, path)) != NULL) {
         const char *filename;
         filename = luaL_gsub(L, lua_tostring(L, -1), LUA_PATH_MARK, name);
         lua_remove(L, -2);  /* remove path template */
         if (readable(filename))  /* does file exist and is readable? */
             return filename;  /* return that file name */
         lua_pushfstring(L, "\n\tno file " LUA_QS, filename);
         lua_remove(L, -2);  /* remove file name */
         lua_concat(L, 2);  /* add entry to possible error message */
     }
     return NULL;  /* not found */
}

//------------------------------------------------------------------------------
/**
    ../../test/src/luabind_test/lua/?.lua
    ../testdata/luabind_lua/?.lua
    imp like loader_Lua@loadlib.c
*/
int _user_loader_lua(lua_State *L)
{
    const char *filename;
    const char *name = luaL_checkstring(L, 1);
    filename = findfile(L, name, LUA_URLPATH);
    if (filename == NULL) return 1;  /* library not found in this path */
    DwString s = DwString::fromUtf8(filename);
    DwUrl url(s);

    if (url.scheme() == DwString("file"))
    {
        DwString localPath = url.localPath();
        // convert the path to cap
        luaL_loadfile(L, localPath.toCap().constData());
        return 1;
    }
    else
    {
        DwByteArray data = DwResourceInterface::acquireData(url);
        if (data.size() > 0)
        {
            int res = luaL_loadbuffer(L, data.data(), data.size(), s.toCap().constData());
            if (0 != res)
            {
                DwString s = lua_tostring(L, -1);
                DW_UI_LOGER() << "script error: " << s << "\n";
                DW_ASSERT(false);
            }

            // the ll_require will
            return 1;
        }
    }

    lua_pushstring(L, "not valid user module");
    return 1;
}

int _user_append_search_path(lua_State *L)
{
    size_t len;

    //<stack><global>
    lua_getglobal(L, "package");    
    DW_ASSERT(lua_istable(L, -1));

    //<stack><global><url>
    lua_getfield(L, -1, LUA_URLPATH);
    lua_pushstring(L, ";");
    lua_pushvalue(L, -4);
    lua_concat(L, 3);
    lua_setfield(L, -2, LUA_URLPATH);
    return 0;
}

//------------------------------------------------------------------------------
/**
    move searcher_Lua to index 5, and insert f in index 2
    package.loaders[5] = package.loaders[2]
    package.loaders[2] = f;
*/
bool _user_replace_loader(lua_State *L)
{
    DW_ASSERT(L);

    // set global function
    lua_pushcfunction(L, _user_append_search_path);
    lua_setglobal(L, "dw_append_search_path");

    //<stack><global>
    lua_getglobal(L, "package");    
    DW_ASSERT(lua_istable(L, -1));

    //<stack><global><urlpath>
    lua_pushstring(L, LUA_URLPATH_DEFAULT);
    //<stack><global>
    lua_setfield(L, -2, LUA_URLPATH);

    //<stack><global><loaders>
    lua_getfield(L, -1, "loaders"); // package.loaders  
    DW_ASSERT(lua_istable(L, -1));

    //<stack><global><loaders><loaders2>
    lua_rawgeti(L, -1, 2); // package.loaders[2]    
    //<stack><global><loaders><loaders2><user_load>
    lua_pushcfunction(L, _user_loader_lua);     
    //<stack><global><loaders><loaders2>
    lua_rawseti(L, -3, 2); // package.loaders[2] = f; 
    //<stack><global><loaders>
    lua_rawseti(L, -2, 5); // package.loaders[5] = loaders2; 
    //<stack>
    lua_pop(L, 2);
    return true;
}
