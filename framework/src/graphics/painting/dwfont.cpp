#include "stable.h"
#include "dwfont.h"
#include "dwcore/dwglobalstatic.h"

//------------------------------------------------------------------------------
/**
*/
static Gdiplus::RectF DwRect2GdiRect(const DwRectF &rect)
{
    return Gdiplus::RectF(
               Gdiplus::REAL(rect.left()),
               Gdiplus::REAL(rect.top()),
               Gdiplus::REAL(rect.width()),
               Gdiplus::REAL(rect.height()));
}

//------------------------------------------------------------------------------
/**
*/
struct FontConfig
{
    DwString name;
    Dw::FontStyle style;
    DwReal size;
    DwColor color;
    FontConfig()
        : name(L"Microsoft YaHei") //
        //: name(L"Consolas")
        , size(10)
        , style(Dw::FontStyleRegular)
        , color(0, 0, 0) // black
    {
        // if the default no available
        Gdiplus::FontFamily family(name.constString());
        if (!family.IsAvailable())
        {
            // Get The System Default Font
            WCHAR familyName[LF_FACESIZE] = {0};
            HDC hdc = ::CreateCompatibleDC(NULL);
            HFONT h = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            Gdiplus::Font f(hdc, h);
            Gdiplus::FontFamily family;
            if ( f.GetFamily(&family) == Gdiplus::Ok )
            {
                family.GetFamilyName(familyName);
                name = familyName;
                size = f.GetSize();
            }
            DeleteObject(h);
            DeleteDC(hdc);
        }
    }
};
DW_GLOBAL_STATIC(FontConfig, _dw_fontConfig);

//------------------------------------------------------------------------------
/**
*/
void DwFont::setDefaultName(const DwString &s)
{
    _dw_fontConfig()->name = s;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwFont::defaultName()
{
    return _dw_fontConfig()->name;
}

//------------------------------------------------------------------------------
/**
*/
void DwFont::setDefaultSize(DwReal s)
{
    _dw_fontConfig()->size = s;
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwFont::defaultSize()
{
    return _dw_fontConfig()->size;
}

//------------------------------------------------------------------------------
/**
*/
void DwFont::setDefaultStyle(Dw::FontStyle s)
{
    _dw_fontConfig()->style = s;
}

//------------------------------------------------------------------------------
/**
*/
Dw::FontStyle DwFont::defaultStyle()
{
    return _dw_fontConfig()->style;
}

//------------------------------------------------------------------------------
/**
*/
void DwFont::setDefaultColor(const DwColor& c)
{
    _dw_fontConfig()->color = c;
}

//------------------------------------------------------------------------------
/**
*/
DwColor DwFont::defaultColor()
{
    return _dw_fontConfig()->color;
}

//------------------------------------------------------------------------------
/**
*/
DwFont::DwFont()
    : m_fontName(_dw_fontConfig()->name)
    , m_fontSize(_dw_fontConfig()->size)
    , m_fontStyle(_dw_fontConfig()->style)
    , m_fontColor(_dw_fontConfig()->color)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwFont::DwFont(const DwString &n)
    : m_fontName(n) //
    , m_fontSize(_dw_fontConfig()->size)
    , m_fontStyle(_dw_fontConfig()->style)
    , m_fontColor(_dw_fontConfig()->color)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwFont::DwFont(const DwString &n, DwReal s)
    : m_fontName(n) //
    , m_fontSize(s)
    , m_fontStyle(_dw_fontConfig()->style)
    , m_fontColor(_dw_fontConfig()->color)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwFont::DwFont(const DwString& n, DwReal s, const DwColor &c)
: m_fontName(n) //
, m_fontSize(s)
, m_fontStyle(_dw_fontConfig()->style)
, m_fontColor(c)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwFont::setName(const DwString &n)
{
    m_fontName = n;
}

//------------------------------------------------------------------------------
/**
*/
void DwFont::setStyle(Dw::FontStyle style)
{
    m_fontStyle = style;
}

//------------------------------------------------------------------------------
/**
*/
void DwFont::setSize(DwReal s)
{
    m_fontSize = s;
}

//------------------------------------------------------------------------------
/**
*/
void DwFont::setColor(const DwColor &c)
{
    m_fontColor = c;
}

//------------------------------------------------------------------------------
/**
*/
bool DwFont::isAvailable() const
{
    Gdiplus::FontFamily family(m_fontName.constString());
    if (family.IsAvailable())
    {
        Gdiplus::Font font(&family, Gdiplus::REAL(m_fontSize), (Gdiplus::FontStyle)m_fontStyle);
        return !!(font.IsAvailable());
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwFont::MeasureString(const DwString &s) const
{
    HDC hdc = ::CreateCompatibleDC(NULL);
    Gdiplus::Graphics graphics(hdc);
    Gdiplus::FontFamily family(m_fontName.constString());
    Gdiplus::Font font(&family, Gdiplus::REAL(m_fontSize), (Gdiplus::FontStyle)m_fontStyle);
    Gdiplus::RectF rect;
    Gdiplus::StringFormat sf(Gdiplus::StringFormat::GenericDefault());
    sf.SetFormatFlags(sf.GetFormatFlags() | Gdiplus::StringFormatFlagsMeasureTrailingSpaces);
    graphics.MeasureString(s.constString(), s.length(), &font, Gdiplus::PointF(0., 0.), &sf, &rect);
    ::DeleteDC(hdc);
    return DwRectF(rect.X, rect.Y, ceilf(rect.Width), (rect.Height));
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwFont::MeasureString(const DwString &s, const DwRectF &layoutRect) const
{
    HDC hdc = ::CreateCompatibleDC(NULL);
    Gdiplus::Graphics graphics(hdc);
    Gdiplus::FontFamily family(m_fontName.constString());
    Gdiplus::Font font(&family, Gdiplus::REAL(m_fontSize), (Gdiplus::FontStyle)m_fontStyle);
    Gdiplus::RectF rect;
    Gdiplus::StringFormat sf(Gdiplus::StringFormat::GenericDefault());
    sf.SetFormatFlags(sf.GetFormatFlags() | Gdiplus::StringFormatFlagsMeasureTrailingSpaces);
    graphics.MeasureString(s.constString(), s.length(), &font, DwRect2GdiRect(layoutRect), &sf, &rect);
    DeleteDC(hdc);
    return DwRectF(rect.X, rect.Y, ceilf(rect.Width), (rect.Height));
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwFont::MeasureString(const DwString &s, DwReal width) const
{
    HDC hdc = ::CreateCompatibleDC(NULL);
    Gdiplus::Graphics graphics(hdc);
    Gdiplus::FontFamily family(m_fontName.constString());
    Gdiplus::Font font(&family, Gdiplus::REAL(m_fontSize), (Gdiplus::FontStyle)m_fontStyle);
    Gdiplus::SizeF size;
    Gdiplus::StringFormat sf(Gdiplus::StringFormat::GenericDefault());
    sf.SetFormatFlags(sf.GetFormatFlags() | Gdiplus::StringFormatFlagsMeasureTrailingSpaces | Gdiplus::StringFormatFlagsNoClip);
    graphics.MeasureString(s.constString(), s.length(), &font, Gdiplus::SizeF(Gdiplus::REAL(width), 0.0f), &sf, &size);
    DeleteDC(hdc);

    return DwRectF(0, 0, ceilf(size.Width), (size.Height));
}

//------------------------------------------------------------------------------
/**
*/
bool DwFont::operator == (const DwFont &other)const
{
    if (this == &other)
    {
        return true;
    }

    if (m_fontName == other.m_fontName && m_fontStyle == other.m_fontStyle
            && m_fontColor == other.m_fontColor && m_fontSize == other.m_fontSize)
    {
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwFont::operator != (const DwFont &other)const
{
    return !(operator == (other));
}