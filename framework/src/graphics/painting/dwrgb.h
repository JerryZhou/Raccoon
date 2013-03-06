#pragma once
#include "dwcore/dwtypes.h"

typedef unsigned int QRgb;                        // RGB triplet

const QRgb  RGB_MASK    = 0x00ffffff;                // masks RGB values

DW_GRAPHICS_EXPORT inline int dwRed(QRgb rgb)                // get red part of RGB
{ return ((rgb >> 16) & 0xff); }

DW_GRAPHICS_EXPORT inline int dwGreen(QRgb rgb)                // get green part of RGB
{ return ((rgb >> 8) & 0xff); }

DW_GRAPHICS_EXPORT inline int dwBlue(QRgb rgb)                // get blue part of RGB
{ return (rgb & 0xff); }

DW_GRAPHICS_EXPORT inline int dwAlpha(QRgb rgb)                // get alpha part of RGBA
{ return rgb >> 24; }

DW_GRAPHICS_EXPORT inline QRgb dwRgb(int r, int g, int b)// set RGB value
{ return (0xffu << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff); }

DW_GRAPHICS_EXPORT inline QRgb dwRgba(int r, int g, int b, int a)// set RGBA value
{ return ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff); }

DW_GRAPHICS_EXPORT inline int dwGray(int r, int g, int b)// convert R,G,B to gray 0..255
{ return (r*11+g*16+b*5)/32; }

DW_GRAPHICS_EXPORT inline int dwGray(QRgb rgb)                // convert RGB to gray 0..255
{ return dwGray(dwRed(rgb), dwGreen(rgb), dwBlue(rgb)); }

DW_GRAPHICS_EXPORT inline bool dwIsGray(QRgb rgb)
{ return dwRed(rgb) == dwGreen(rgb) && dwRed(rgb) == dwBlue(rgb); }