#pragma once
#include "dwcanvas.h"

//------------------------------------------------------------------------------
class DwCanvasWindowData;

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwCanvasWindow : public DwCanvas
{
    _declare_data(DwCanvasWindow);
public:
    DwCanvasWindow();
    virtual ~DwCanvasWindow();

    // setup the canvas from the window
    void setup(HWND hWnd);

    virtual void beginPaint(const DwRect &s, const DColor& c);
    virtual void endPaint();

protected:
    virtual void resize(const DwSize& s);

    void setupOffscreenSurface();
    bool isNeedOffscreenSurface();
    void updateOffscreenSurface();
};// end of DwCanvasWindow