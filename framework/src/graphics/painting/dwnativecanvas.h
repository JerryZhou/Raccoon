#pragma once
#include "dwgraphics/dwcanvas.h"

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwNativeCanvas : public DwCanvas
{
public:
    DwNativeCanvas();

    bool translucent() const;
    void setTranslucent(bool b);

    void setWinHwnd(HWND hWnd);
    HWND winHwnd()const;

    virtual void beginePaint(const DwRegion& region);
    virtual void endPaint();

protected:
    DwRegion m_dirtyRegion;
    HWND m_hWnd;
    bool m_translucent;
};// end of DwNativeCanvas