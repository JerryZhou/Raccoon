#include "stable.h"

#include "dwcolor.h"

//------------------------------------------------------------------------------
/**
*/
DwColor::DwColor(Dw::GlobalColor color)
{
#define QRGB(r, g, b) \
    QRgb(((0xffu << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff)))
#define QRGBA(r, g, b, a) \
    QRgb(((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff))

    static const QRgb global_colors[] = {
        //QRGB(255, 255, 255), // Qt::color0
        //QRGB(  0,   0,   0), // Qt::color1
        QRGB(  0,   0,   0), // black
        QRGB(255, 255, 255), // white
        /*
         * From the "The Palette Manager: How and Why" by Ron Gery,
         * March 23, 1992, archived on MSDN:
         *
         *     The Windows system palette is broken up into two
         *     sections, one with fixed colors and one with colors
         *     that can be changed by applications. The system palette
         *     predefines 20 entries; these colors are known as the
         *     static or reserved colors and consist of the 16 colors
         *     found in the Windows version 3.0 VGA driver and 4
         *     additional colors chosen for their visual appeal.  The
         *     DEFAULT_PALETTE stock object is, as the name implies,
         *     the default palette selected into a device context (DC)
         *     and consists of these static colors. Applications can
         *     set the remaining 236 colors using the Palette Manager.
         *
         * The 20 reserved entries have indices in [0,9] and
         * [246,255]. We reuse 17 of them.
         */
        QRGB(128, 128, 128), // index 248   medium gray
        QRGB(160, 160, 164), // index 247   light gray
        QRGB(192, 192, 192), // index 7     light gray
        QRGB(255,   0,   0), // index 249   red
        QRGB(  0, 255,   0), // index 250   green
        QRGB(  0,   0, 255), // index 252   blue
        QRGB(  0, 255, 255), // index 254   cyan
        QRGB(255,   0, 255), // index 253   magenta
        QRGB(255, 255,   0), // index 251   yellow
        QRGB(128,   0,   0), // index 1     dark red
        QRGB(  0, 128,   0), // index 2     dark green
        QRGB(  0,   0, 128), // index 4     dark blue
        QRGB(  0, 128, 128), // index 6     dark cyan
        QRGB(128,   0, 128), // index 5     dark magenta
        QRGB(128, 128,   0), // index 3     dark yellow
        QRGBA(0, 0, 0, 0)    //             transparent
    };
#undef QRGB
#undef QRGBA

    setRgb(dwRed(global_colors[color]),
           dwGreen(global_colors[color]),
           dwBlue(global_colors[color]),
           dwAlpha(global_colors[color]));
}

//------------------------------------------------------------------------------
/**
*/
void DwColor::setRgb(int r, int g, int b, int a/* = 255*/)
{
    m_color.rgba.red = r;
    m_color.rgba.green = g;
    m_color.rgba.blue = b;
    m_color.rgba.alpha = a;
}