#pragma once

#include "dwnamespace.h"
#include <dwgraphics/dwcolor.h>
#include <dwcore/dwstring.h>
#include "dwcore/dwrect.h"

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwFont
{
public:
    DwFont();
    DwFont(const DwString& n);
    DwFont(const DwString& n, DwReal s);
    DwFont(const DwString& n, DwReal s, const DwColor &c);

    inline DwString name() const;
    void setName(const DwString &);

    inline Dw::FontStyle style() const;
    void setStyle(Dw::FontStyle );

    inline DwReal size() const;
    void setSize(DwReal );

    inline DwColor color() const;
    void setColor(const DwColor &);

    bool isAvailable() const;

    DwRectF MeasureString(const DwString &s) const;

    DwRectF MeasureString(const DwString &s, const DwRectF &layoutRect) const;

    DwRectF MeasureString(const DwString &s, DwReal width) const;

    bool operator == (const DwFont &other)const;
    bool operator != (const DwFont &other)const;

    // for default font
    static void setDefaultName(const DwString &s);
    static DwString defaultName();

    static void setDefaultSize(DwReal s);
    static DwReal defaultSize();

    static void setDefaultStyle(Dw::FontStyle s);
    static Dw::FontStyle defaultStyle();

    static void setDefaultColor(const DwColor& c);
    static DwColor defaultColor();

protected:
    DwString m_fontName;
    Dw::FontStyle m_fontStyle;
    DwReal m_fontSize;
    DwColor m_fontColor;
};// end of DwFont

//------------------------------------------------------------------------------
/**
*/
inline DwString DwFont::name() const
{
    return m_fontName;
}

//------------------------------------------------------------------------------
/**
*/
inline Dw::FontStyle DwFont::style() const
{
    return m_fontStyle;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwFont::size() const
{
    return m_fontSize;
}

//------------------------------------------------------------------------------
/**
*/
inline DwColor DwFont::color() const
{
    return m_fontColor;
}