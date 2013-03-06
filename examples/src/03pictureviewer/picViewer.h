#pragma once

#include "dwgui/dwapplication.h"
#include "dwgui/dwmainwindow.h"
#include "dwgui/dwapplauncher.h"
#include "dwgui/dwwin32registry.h"
#include "dwgui/dwcommandlineargs.h"
#include "dwgui/dwringlogcase.h"
#include "dwgui/dwpluginlib.h"
#include "dwgui/dwplugin.h"
#include "dwgui/dwresourceinterface.h"
#include "resource.h"

//------------------------------------------------------------------------------
bool showImage(DwFrameWindow *window, const DwCommandLineArgs &args);
bool showImageInViewer(DwFrameWindow *window, const DwUrl& url);