#pragma once

#include <dwcore/dwtypes.h>
#include "dwgraphics/dwrgb.h"
#include "dwnamespace.h"

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwColor
{
public:
    DwColor();
    DwColor(Dw::GlobalColor c);
    DwColor(int r, int g, int b, int a = 255);

    inline int red() const;
    inline int green() const;
    inline int blue() const;
    inline int alpha() const;

    inline void setRed(int r);
    inline void setGreen(int g);
    inline void setBlue(int b);
    inline void setAlpha(int a);

    inline bool operator == (const DwColor &other) const;
    inline bool operator != (const DwColor &other) const;

    void setRgb(int r, int g, int b, int a = 255);

    inline uint32_t rgba() const;
private:
    union
    {
        struct
        {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            uint8_t alpha;
        } rgba;
        uint8_t array[4];
        uint32_t rgba32;
    } m_color;
};// end of DwColor

//------------------------------------------------------------------------------
/**
*/
inline DwColor::DwColor()
{
    setRgb(0, 0, 0, 255);
}

//------------------------------------------------------------------------------
/**
*/
inline DwColor::DwColor(int r, int g, int b, int a/* = 255*/)
{
    setRgb(r, g, b, a);
}

//------------------------------------------------------------------------------
/**
*/
inline int DwColor::red() const
{
    return m_color.rgba.red;
}

//------------------------------------------------------------------------------
/**
*/
inline int DwColor::green() const
{
    return m_color.rgba.green;
}

//------------------------------------------------------------------------------
/**
*/
inline int DwColor::blue() const
{
    return m_color.rgba.blue;
}

//------------------------------------------------------------------------------
/**
*/
inline int DwColor::alpha() const
{
    return m_color.rgba.alpha;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwColor::setRed(int r)
{
    m_color.rgba.red = r;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwColor::setGreen(int g)
{
    m_color.rgba.green = g;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwColor::setBlue(int b)
{
    m_color.rgba.blue = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwColor::setAlpha(int a)
{
    m_color.rgba.alpha = a;
}

//------------------------------------------------------------------------------
/**
*/
bool DwColor::operator == (const DwColor &other) const
{
    if(m_color.rgba.red == other.red() &&
        m_color.rgba.green == other.green() &&
        m_color.rgba.blue == other.blue() &&
        m_color.rgba.alpha == other.alpha() )
        return true;

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwColor::operator != (const DwColor &other) const
{
    return ! operator==(other);
}

//------------------------------------------------------------------------------
/**
*/
uint32_t DwColor::rgba() const
{
    return m_color.rgba32;
}