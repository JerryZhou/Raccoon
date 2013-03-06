#include "stable.h"

#include "dwgui/dwgui.h"
#include "strsafe.h"
#include "dwluabind/dwluainterface.h"
#include "dwgui/dwcommandlineargs.h"


//////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR , int )
{
    DwApplication app;

    // init the script engine
    DwLuaEngine engine;
    engine.setup();

    // add all inline exporter
    DwLuaInterface::addInlineExporters(&engine);

    // load a example
    engine.load("home:testdata/luabind_lua/tcpclient.lua");

    return app.exec();
}