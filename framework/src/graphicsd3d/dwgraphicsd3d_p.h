#pragma once
#include "dwcore/dwsize.h"

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwGraphicsD3D9
{
public:
    /// return the d3d
    static LPDIRECT3D9 d3d9();
    /// return the d3d9
    static LPDIRECT3DDEVICE9 device();
    /// return the setting
    static const D3DPRESENT_PARAMETERS& d3dpp();

    /// should call before any d3d op
    static void setup(HWND hWnd=0);
    /// setup out side
    static void setup(LPDIRECT3D9 d3d, LPDIRECT3DDEVICE9 dev, D3DPRESENT_PARAMETERS pp);
    /// should call before process exit
    static void discard();

    /// should call begin paint
    static void beginPaint();
    /// should call end paint
    static void endPaint();

    /// setup the 2d piple states
    static void setupStates();

    /// choose the texture size
    static void adjustTexSize(DwSize& s);
protected:
private:
};// end of DwGraphicsD3D9
