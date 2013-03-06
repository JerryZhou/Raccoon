#include "stable.h"

#include "dwgui/dwgui.h"
#include "dwapp/dwapp.h"
#include "dwluabind/dwluabind.h"

int lua_app_quit(lua_State *)
{
    dwApp()->quit();
    return 0;
}

//////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR , int )
{
    DwMainApplication app;

    // init the script engine
    DwLuaEngine engine;
    engine.setupWithAllExporters();

    // load a example
    engine.load("home:main.lua");

    // C++ to lua
    engine.registerScript(lua_app_quit, "quit", "app");

    // lua to C++
    engine.call("showMainWindow", "");
    engine.run("showWindow2()");

    return app.exec();
}