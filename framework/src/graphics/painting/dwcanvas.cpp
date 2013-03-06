#include "stable.h"

#include "dwcanvas.h"
#include "dwcore/dwglobalstatic.h"
//#include "text/dwtextutil.h"

//------------------------------------------------------------------------------
/**
*/
struct InitConstGraphics 
{
    Gdiplus::Bitmap content;
    Gdiplus::Graphics gfx;

    InitConstGraphics() :content(24, 24),
        gfx(&content){}
};
//------------------------------------------------------------------------------
/**
*/
struct CanvasConfig 
{
    Dw::TextRenderingHint textRenderingHint;
    Dw::InterpolationMode   interpolationMode;
    Dw::CompositingQuality  compositingQuality;
    InitConstGraphics* constGraphics;

    CanvasConfig()  : constGraphics(NULL)
    {
        textRenderingHint = Dw::TextRenderingHintAntiAlias;
        interpolationMode = Dw::InterpolationModeLowQuality;
        compositingQuality = Dw::CompositingQualityHighSpeed;
    }

    void init()
    {
        DW_ASSERT(!constGraphics);
        constGraphics = new InitConstGraphics();

        constGraphics->gfx.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
        constGraphics->gfx.SetInterpolationMode(Gdiplus::InterpolationModeLowQuality);
        constGraphics->gfx.SetCompositingQuality(Gdiplus::CompositingQualityHighSpeed);
    }

    void unInit()
    {
        delete constGraphics;
        constGraphics = NULL;
    }
};
DW_GLOBAL_STATIC(CanvasConfig, _dw_canvasConfig);

//------------------------------------------------------------------------------
/**
*/
static Gdiplus::PointF DwPoint2GdiPoint(const DwPointF &pt)
{
    return Gdiplus::PointF(Gdiplus::REAL(pt.x()), Gdiplus::REAL(pt.y()));
}

//------------------------------------------------------------------------------
/**
*/
static Gdiplus::SizeF DwSize2GdiSize(const DwSizeF &sz)
{
    return Gdiplus::SizeF(Gdiplus::REAL(sz.width()), Gdiplus::REAL(sz.height()));
}

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
static Gdiplus::Color DwColor2GdiColor(const DwColor &c)
{
    return Gdiplus::Color( c.alpha(), c.red(), c.green(), c.blue() );
}

//------------------------------------------------------------------------------
/**
*/
static Gdiplus::Color DwFontColor2GdiColor(const DwColor &c)
{
    uint8_t a = c.alpha();
    return Gdiplus::Color( a == 255 ? 254 : a, c.red(), c.green(), c.blue() );
}

//------------------------------------------------------------------------------
/**
*/
static Gdiplus::Matrix* DwTransform2GdiMatrix(const DwTransform& transform, Gdiplus::Matrix& m)
{
    m.SetElements(
        Gdiplus::REAL(transform.m11()),
        Gdiplus::REAL(transform.m12()),
        Gdiplus::REAL(transform.m21()),
        Gdiplus::REAL(transform.m22()),
        Gdiplus::REAL(transform.dx()),
        Gdiplus::REAL(transform.dy()));
    return &m;
}

//------------------------------------------------------------------------------
/**
*/
static Gdiplus::CompositingMode DwCompositingMode2GdiMode(Dw::CompositingMode mode)
{
    switch(mode)
    {
    case Dw::CompositingModeSourceOver :
        return Gdiplus::CompositingModeSourceOver;
    case Dw::CompositingModeSourceCopy :
        return Gdiplus::CompositingModeSourceCopy;
    default:
        return Gdiplus::CompositingModeSourceOver;
    }
}

//------------------------------------------------------------------------------
/**
*/
static Dw::CompositingMode GdiCompositingMode2DwMode(Gdiplus::CompositingMode mode)
{
    switch(mode)
    {
    case Gdiplus::CompositingModeSourceOver :
        return Dw::CompositingModeSourceOver;
    case Gdiplus::CompositingModeSourceCopy :
        return Dw::CompositingModeSourceCopy;
    default:
        return Dw::CompositingModeSourceOver;
    }
}

//------------------------------------------------------------------------------
/**
*/
DwCanvas::DwCanvas()
    : m_graphics(NULL)
    , m_bitmap(NULL)
    , m_oldBitmap(NULL)
{
    m_memDC = ::CreateCompatibleDC(NULL);
}

//------------------------------------------------------------------------------
/**
*/
DwCanvas::~DwCanvas()
{
    DW_SAFE_DELETE(m_graphics);
    m_savedStates.clear();
    DeleteObject(m_bitmap);
    DeleteDC(m_memDC);
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::beginePaint(const DwRegion& region)
{
    m_savedStates.clear();
    m_oldBitmap = SelectObject(m_memDC, m_bitmap);
    if (!m_graphics)
    {
        m_graphics = Gdiplus::Graphics::FromHDC(m_memDC);
        if (m_graphics)
        {
            m_graphics->SetCompositingQuality(Gdiplus::CompositingQuality(defaultCompositingQuality()));
            m_graphics->SetInterpolationMode(Gdiplus::InterpolationMode(defaultInterpolationMode()));
            m_graphics->SetTextRenderingHint(Gdiplus::TextRenderingHint(defaultTextRenderingHint()));
        }
        m_graphics->Clear(Gdiplus::Color(0, 0, 0, 0));
    }

    clear(region, DwColor(0, 0, 0, 0));
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::endPaint()
{
    SelectObject(m_memDC, m_oldBitmap);
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::fillRect(const DwRectF &rect, const DwColor &color)
{
    Gdiplus::Color c(color.alpha(), color.red(), color.green(), color.blue());
    Gdiplus::SolidBrush brush(c);
    m_graphics->FillRectangle(&brush, DwRect2GdiRect(rect));
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::fillRegion(const DwRegion &region, const DwColor &color)
{
    Gdiplus::Color c(color.alpha(), color.red(), color.green(), color.blue());
    Gdiplus::SolidBrush brush(c);
    m_graphics->FillRegion(&brush, region.nativeRegion());
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::drawRect(const DwRectF &rect, const DwColor &color)
{
    Gdiplus::Color c(color.alpha(), color.red(), color.green(), color.blue());
    Gdiplus::Pen pen(c);
    m_graphics->DrawRectangle(&pen, DwRect2GdiRect(rect));
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::drawImage(double x, double y, const DwImage &image)
{
    Gdiplus::Bitmap *nativeImage = static_cast<Gdiplus::Bitmap *>(image.nativeImage());
    m_graphics->DrawImage(nativeImage, Gdiplus::REAL(x), Gdiplus::REAL(y));
}

//------------------------------------------------------------------------------
/**
    Improving Performance by Avoiding Automatic Scaling
    see : http://msdn.microsoft.com/en-us/library/ms533829(v=vs.85).aspx
*/
void DwCanvas::drawImage(double x, double y, double width, double height, const DwImage &image)
{
    Gdiplus::Bitmap *nativeImage = static_cast<Gdiplus::Bitmap *>(image.nativeImage());
    m_graphics->DrawImage(nativeImage, Gdiplus::REAL(x), Gdiplus::REAL(y), Gdiplus::REAL(width), Gdiplus::REAL(height));
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::drawImageScaled(const DwRectF &destRect, const DwImage &image, const DwRectF &srcRect )
{
    Gdiplus::Image *nativeImage = static_cast<Gdiplus::Image *>(image.nativeImage());

    m_graphics->DrawImage(
        nativeImage,
        DwRect2GdiRect(destRect),
        DwRect2GdiRect(srcRect),
        Gdiplus::UnitPixel);
}

//------------------------------------------------------------------------------
/**
*/
#define _dw_flip_wh(w, h) ( (w) == 1 ? \
    ( (h) == 1 ? Gdiplus::WrapModeTileFlipXY : Gdiplus::WrapModeTileFlipX ) : \
    ( (h) == 1 ? Gdiplus::WrapModeTileFlipY : Gdiplus::WrapModeTile) )

void DwCanvas::drawImageScaledFlipXY(const DwRectF &destRect, const DwImage &image, const DwRectF &srcRect)
{
    Gdiplus::Image *nativeImage = static_cast<Gdiplus::Image *>(image.nativeImage());

    Gdiplus::TextureBrush brush(nativeImage, 
        _dw_flip_wh(srcRect.width(), srcRect.height()), DwRect2GdiRect(srcRect));
    m_graphics->FillRectangle(&brush, DwRect2GdiRect(destRect));
}

//------------------------------------------------------------------------------
/**
*/
#define _dw_draw_scaled_img(p, dx, dy, dw, dh, sx, sy, sw, sh) \
    { m_graphics->DrawImage(p, Gdiplus::Rect(dx, dy, dw, dh), sx, sy, sw, sh, Gdiplus::UnitPixel); }

#define _dw_draw_scaled_img_flip(p, dx, dy, dw, dh, sx, sy, sw, sh, flip) \
    { Gdiplus::TextureBrush brush(nativeImage, flip, Gdiplus::Rect(sx, sy, sw, sh)); \
        m_graphics->FillRectangle(&brush, Gdiplus::Rect(dx, dy, dw, dh));}

#define _dw_draw_scaled_img_or(p, dx, dy, dw, dh, sx, sy, sw, sh) \
    { if( sw == 1 || sh == 1) { _dw_draw_scaled_img_flip(p, dx, dy, dw, dh, sx, sy, sw, sh, _dw_flip_wh(sw, sh)); }\
      else { _dw_draw_scaled_img(p, dx, dy, dw, dh, sx, sy, sw, sh);} }

#define _dw_v_m(m) ((m) ? 1 : 0)
#define _dw_v_real_wh(n) (((n) >= 2) ? ((n) - 1) : (n))
#define _dw_v_n_w(n, ws) (ws) ? (_dw_v_real_wh(n)) : (n)
#define _dw_v_n_h(n, hs) (hs) ? (_dw_v_real_wh(n)) : (n)

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::drawLine(const DwPointF &from, const DwPointF &to, const DwColor &color)
{
    Gdiplus::Pen pen(Gdiplus::Color(color.alpha(), color.red(), color.green(), color.blue()));
    m_graphics->DrawLine(&pen, DwPoint2GdiPoint(from), DwPoint2GdiPoint(to));
}

//------------------------------------------------------------------------------
/**
	------------------------
	|      cx1   cx2   cx3 |
	|-----------------------
	|cy1 |     |     |     |
	|-----------------------
	|cy2 |     |     |     |
	|-----------------------
	|cy3 |     |     |     |
	------------------------
*/
void DwCanvas::drawBorderImage( const DwRect &rect, const DwImage &image, const DwMargins &margins)
{
    drawBorderImage(rect, DwRect(DwPoint(0, 0), image.size()), image, margins);
}

//------------------------------------------------------------------------------
/**
	------------------------
	|      cx1   cx2   cx3 |
	|-----------------------
	|cy1 |     |     |     |
	|-----------------------
	|cy2 |     |     |     |
	|-----------------------
	|cy3 |     |     |     |
	------------------------
*/
void DwCanvas::drawBorderImage(const DwRect &rect, const DwRect &sourceRect, const DwImage &image, const DwMargins &margins)
{
    Gdiplus::Image *nativeImage = static_cast<Gdiplus::Image *>(image.nativeImage());

    DwSize imageSize = image.size();

    int cx1 = margins.left();//imageSize.width() / 3 + 5;
    int cx3 = margins.right();//imageSize.width() / 3 - 5;
    int cx2 = sourceRect.width() - cx1 - cx3;
    int cy1 = margins.top();//imageSize.height() / 3 - 5;
    int cy3 = margins.bottom();//imageSize.height() / 3 + 5;
    int cy2 = sourceRect.height() - cy1 - cy3;

    int sleft = sourceRect.left();
    int stop = sourceRect.top();

    //corners
    if (cx1)
    {
        if (cy1) _dw_draw_scaled_img(nativeImage, 
            rect.left(), rect.top(), cx1, cy1, 
            sleft, stop, cx1, cy1);
        if (cy3) _dw_draw_scaled_img(nativeImage, 
            rect.left(), rect.top() + rect.height() - cy3, cx1, cy3, 
            sleft, stop + cy1 + cy2, cx1, cy3);
    }

    if (cx3)
    {
        if(cy1) _dw_draw_scaled_img(nativeImage, 
            rect.left() + rect.width() - cx3, rect.top(), cx3, cy1, 
            sleft + cx1 + cx2, stop, cx3, cy1);
        if(cy3) _dw_draw_scaled_img(nativeImage, 
            rect.left() + rect.width() - cx3, rect.top() + rect.height() - cy3, cx3, cy3, 
            sleft + cx1 + cx2, stop + cy1 + cy2, cx3, cy3);
    }

    //////////////////////////////////////////////////////////////////////////
    int ws = _dw_v_m(cx1) + _dw_v_m(cx3);
    int hs = _dw_v_m(cy1) + _dw_v_m(cy3);

    //there is a gdi algothrim problem about uv address, 
    //we should reduce 1 by the width and height before we scale img
    //top bottom border
    if (cx2 > ws)
    {
        DW_ASSERT("width must greather and equal than 2" && cx2 - ws >= 2);   // width - 1
        if (cy1) _dw_draw_scaled_img(nativeImage, 
            rect.left() + cx1 , rect.top(), rect.width() - cx1 - cx3, cy1, 
            sleft + cx1 + _dw_v_m(cx1), stop, _dw_v_n_w(cx2-ws, ws), (cy1));

        if (cy3) _dw_draw_scaled_img(nativeImage, 
            rect.left() + cx1, rect.top() + rect.height() - cy3, rect.width() - cx1 - cx3, cy3, 
            sleft + cx1 + _dw_v_m(cx1), stop + cy1 + cy2, _dw_v_n_w(cx2-ws, ws), (cy3));
    }

    //left right border
    if(cy2 > hs)
    {
        DW_ASSERT("height must greather and equal than 2" && cy2 - hs >= 2);   // height - 1
        if(cx1) _dw_draw_scaled_img(nativeImage, 
            rect.left(), rect.top() + cy1, cx1, rect.height() - cy1 - cy3, 
            sleft, stop + cy1 + _dw_v_m(cy1), (cx1), _dw_v_n_h(cy2 - hs, hs));
        if(cx3) _dw_draw_scaled_img(nativeImage, 
            rect.left() + rect.width() - cx3, rect.top() + cy1, cx3, rect.height() - cy1 - cy3, 
            sleft + cx1 + cx2, stop + cy1 + _dw_v_m(cy1), (cx3), _dw_v_n_h(cy2 - hs, hs));
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::drawText( const DwRectF &rect, const DwString &text )
{
    Gdiplus::SolidBrush b(Gdiplus::Color(254, 0, 0, 0));
    Gdiplus::Font f(L"SimSun", 12., Gdiplus::FontStyleRegular);

    m_graphics->DrawString(
        text.constString(),
        text.length(),
        &f,
        DwRect2GdiRect(rect),
        NULL,
        &b);

    //DwTextUtil::drawText(*this, rect, text, DwFont());
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::drawText(const DwRectF &rect, const DwString &text, const DwString &fontName, int fontSize)
{
    Gdiplus::SolidBrush b(Gdiplus::Color(254, 0, 0, 0));
    Gdiplus::Font f(fontName.constString(), (float)fontSize , Gdiplus::FontStyleRegular);

    m_graphics->DrawString(
        text.constString(),
        text.length(),
        &f,
        DwRect2GdiRect(rect),
        NULL,
        &b);

    //DwTextUtil::drawText(*this, rect, text, DwFont(fontName, fontSize, DwColor(0, 0, 0, 254)));
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::drawText(const DwRectF &rect, const DwString &text, const DwFont &font)
{
	Gdiplus::SolidBrush b(DwFontColor2GdiColor(font.color()));
	Gdiplus::Font f(font.name().constString(), (float)font.size() , Gdiplus::FontStyle(font.style()));

    m_graphics->DrawString(
        text.constString(),
        text.length(),
        &f,
        DwRect2GdiRect(rect),
        NULL,
        &b);

    /*drawRect(rect, Dw::red);
    DwTextUtil::drawText(*this, rect, text, font);*/
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwCanvas::clipRect() const
{
    Gdiplus::Region rgn;
    Gdiplus::Status result = m_graphics->GetClip(&rgn);
    DW_ASSERT(result == Gdiplus::Ok);

    Gdiplus::RectF rect;
    result = rgn.GetBounds(&rect, m_graphics);
    DW_ASSERT(result == Gdiplus::Ok);

    return DwRectF(rect.X, rect.Y, rect.Width, rect.Height);
}

//------------------------------------------------------------------------------
/**
	Internal Linkage, DEFAULT MODE IS CombineModeReplace
*/
static Gdiplus::CombineMode _dw_ClipOp2GidCombineMode(Dw::ClipOperation op)
{
    switch (op)
    {
    case Dw::IntersectClip:
        return Gdiplus::CombineModeIntersect;
        break;
    case Dw::ReplaceClip:
        return Gdiplus::CombineModeReplace;
        break;
    case Dw::UniteClip:
        return Gdiplus::CombineModeUnion;
    default:
        return Gdiplus::CombineModeReplace;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::setClipRect( const DwRectF &rect, Dw::ClipOperation op)
{
    Gdiplus::RectF r = DwRect2GdiRect(rect);
    m_graphics->SetClip(r, _dw_ClipOp2GidCombineMode(op));
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::setClip(const DwRegion *region, Dw::ClipOperation op)
{
    DW_ASSERT(region);
    m_graphics->SetClip(region->nativeRegion(), _dw_ClipOp2GidCombineMode(op));
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::resetClipRect()
{
    m_graphics->ResetClip();
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::setWorldTransform(const DwTransform& transform)
{
    Gdiplus::Matrix m;
    m_graphics->SetTransform(DwTransform2GdiMatrix(transform, m));
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::resetWoldTransform()
{
    m_graphics->ResetTransform();
}

//------------------------------------------------------------------------------
/**
*/
Dw::CompositingMode DwCanvas::compositingMode() const
{
    return GdiCompositingMode2DwMode(m_graphics->GetCompositingMode());
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::setCompositingMode(Dw::CompositingMode mode)
{
    m_graphics->SetCompositingMode(DwCompositingMode2GdiMode(mode));
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::save()
{
    m_savedStates.push_back(m_graphics->Save());
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::restore()
{
    DW_ASSERT(m_savedStates.size() > 0);
    m_graphics->Restore(m_savedStates.back());
    m_savedStates.pop_back();
}

//------------------------------------------------------------------------------
/**
*/
DwColor DwCanvas::pixel(const DwPoint &p) const
{
    HGDIOBJ old = ::SelectObject(m_memDC, m_bitmap);
    COLORREF c = ::GetPixel(m_memDC, p.x(), p.y());
    ::SelectObject(m_memDC, old);
    return DwColor(GetRValue(c), GetGValue(c), GetBValue(c));
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::resize( const DwSize &size )
{
    if(m_canvasSize == size)
    {
        return;
    }

    if(m_bitmap)
    {
        DeleteObject(m_bitmap);
        m_bitmap = NULL;
    }

    BITMAPV5HEADER bmp_header;
    memset(&bmp_header, 0, sizeof(BITMAPV5HEADER));
    bmp_header.bV5Size = sizeof(BITMAPV5HEADER);
    bmp_header.bV5Width = size.width();
    bmp_header.bV5Height = size.height();
    bmp_header.bV5Planes = 1;
    bmp_header.bV5BitCount = 32;
    bmp_header.bV5Compression = BI_RGB;

    BYTE *bits = NULL;
    m_bitmap = CreateDIBSection(m_memDC, (BITMAPINFO*)&bmp_header, DIB_RGB_COLORS, (VOID**)&bits, NULL, 0);
    m_canvasSize = size;

    DW_SAFE_DELETE(m_graphics);
}

//------------------------------------------------------------------------------
/**
*/
DwSize DwCanvas::size() const
{
    return m_canvasSize;
}

//------------------------------------------------------------------------------
/**
*/
Gdiplus::Graphics *DwCanvas::graphics() const
{
    return m_graphics;
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::clear(const DwRegion& region, const DwColor& c)
{
    if (m_graphics)
    {
        if (region.boundingRect().contains(DwRect(DwPoint(0, 0), size())))
        {
            m_graphics->Clear(DwColor2GdiColor(c));
        }
        else
        {
            Gdiplus::CompositingMode oldMode = m_graphics->GetCompositingMode();
            m_graphics->SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
            Gdiplus::SolidBrush brush(DwColor2GdiColor(c));
            m_graphics->FillRegion(&brush, region.nativeRegion());
            m_graphics->SetCompositingMode(oldMode);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::setDefaultTextRenderingHint(Dw::TextRenderingHint hint)
{
    _dw_canvasConfig()->textRenderingHint = hint;
}

//------------------------------------------------------------------------------
/**
*/
Dw::TextRenderingHint DwCanvas::defaultTextRenderingHint()
{
    return _dw_canvasConfig()->textRenderingHint;
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::setDefaultInterpolationMode(Dw::InterpolationMode mode)
{
    _dw_canvasConfig()->interpolationMode = mode;
}

//------------------------------------------------------------------------------
/**
*/
Dw::InterpolationMode DwCanvas::defaultInterpolationMode()
{
    return _dw_canvasConfig()->interpolationMode;
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::setDefaultCompositingQuality(Dw::CompositingQuality quality)
{
    _dw_canvasConfig()->compositingQuality = quality;
}

//------------------------------------------------------------------------------
/**
*/
Dw::CompositingQuality DwCanvas::defaultCompositingQuality()
{
    return _dw_canvasConfig()->compositingQuality;
}

//------------------------------------------------------------------------------
/**
*/
Gdiplus::Graphics *DwCanvas::defaultGraphics()
{
    return &(_dw_canvasConfig()->constGraphics->gfx);
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::initCanvas()
{
    _dw_canvasConfig()->init();
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::unInitCanvas()
{
    _dw_canvasConfig()->unInit();
}
