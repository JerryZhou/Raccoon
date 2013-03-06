#pragma once

#include "dwcore/dwrect.h"
#include "dwcore/dwpoint.h"
#include "dwcore/dwstring.h"
#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwDesktop : public DwRttiObject
{
    DW_DECLARE_CLASS(DwDesktop);
public:
    static DwRect desktopRect();
    static DwRect desktopWorkArea();
    static bool isVirtualDesktop() ;

    static int screenNumber(const DwPoint &) ;
    static int numScreens() ;
    static int screenCount() ;
    static int primaryScreen() ;

    static DwRect screenGeometry(int screen = -1) ;
    static DwRect screenGeometry(const DwPoint &point) 
    { return screenGeometry(screenNumber(point)); }

    static DwRect availableGeometry(int screen = -1) ;
    static DwRect availableGeometry(const DwPoint &point) 
    { return availableGeometry(screenNumber(point)); }

    enum Direction{Up, Down};
    static bool adjustedAvailableRect(DwRect &);
    static DwRect adjustedAvailableRect(DwPoint& p, const DwSize &, Direction dir=Down);
    static DwRect availableRect(DwPoint& p, const DwSize &, Direction dir=Down);
};// end of DwDesktop