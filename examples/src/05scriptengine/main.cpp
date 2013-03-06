#include "stable.h"

#include "dwgui/dwgui.h"
#include "dwluabind/dwluainterface.h"

//////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR , int )
{
    DwApplication app;

    // init the script engine
    DwLuaEngine engine;
    engine.setupWithAllExporters();

    // load a example
    engine.load("home:testdata/luabind_lua/scriptengine.lua");

    return app.exec();
}