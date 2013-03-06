#include "stable.h"
#include "dwregion.h"
#include "dwcanvas.h"

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
static Gdiplus::Rect DwRect2GdiRect(const DwRect &rect)
{
    return Gdiplus::Rect(
               INT(rect.left()),
               INT(rect.top()),
               INT(rect.width()),
               INT(rect.height()));
}

//------------------------------------------------------------------------------
/**
*/
DwRegion::DwRegion()
{
    m_nativeRegion = new Gdiplus::Region();
}

//------------------------------------------------------------------------------
/**
*/
DwRegion::DwRegion(const DwRegion &r)
:m_nativeRegion(NULL)
{
    *this = r;
}

//------------------------------------------------------------------------------
/**
*/
DwRegion& DwRegion::operator=(const DwRegion &r)
{
    if (&r == this)
    {
        return *this;
    }

    DW_SAFE_DELETE(m_nativeRegion);
    if (r.m_nativeRegion)
    {
        m_nativeRegion = r.m_nativeRegion->Clone();
    }
    else
    {
        DW_ASSERT("can not happen" && false);
        m_nativeRegion = NULL;
    }

    return *this;
}

//------------------------------------------------------------------------------
/**
*/
DwRegion::DwRegion(int x1, int y1, int x2, int y2, int w, int h)
{
    HRGN hrgn = ::CreateRoundRectRgn(x1, y1, x2, y2, w, h);
    m_nativeRegion = Gdiplus::Region::FromHRGN(hrgn);
    ::DeleteObject(hrgn);
}

//------------------------------------------------------------------------------
/**
*/
DwRegion::DwRegion(DwRect r, int w, int h)
{
    HRGN hrgn = ::CreateRoundRectRgn(r.x(), r.y(), r.right()+1, r.bottom()+1, w, h);
    m_nativeRegion = Gdiplus::Region::FromHRGN(hrgn);
    ::DeleteObject(hrgn);
}

//------------------------------------------------------------------------------
/**
*/
DwRegion::DwRegion(DwRect r)
{
    m_nativeRegion = new Gdiplus::Region(DwRect2GdiRect(r));
}

//------------------------------------------------------------------------------
/**
*/
DwRegion::~DwRegion()
{
    DW_SAFE_DELETE(m_nativeRegion);
}

//------------------------------------------------------------------------------
/**
*/
const Gdiplus::Region *DwRegion::nativeRegion() const
{
    return m_nativeRegion;
}

//------------------------------------------------------------------------------
/**
*/
HRGN DwRegion::nativeRGN() const
{
    return m_nativeRegion->GetHRGN(DwCanvas::defaultGraphics());
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::intersect(const DwRect& rect)
{
    return m_nativeRegion->Intersect(DwRect2GdiRect(rect)) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::intersect(const DwRectF& rect)
{
    return m_nativeRegion->Intersect(DwRect2GdiRect(rect)) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::intersect(const DwRegion* region)
{
    return m_nativeRegion->Intersect(region->nativeRegion()) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::uniond(const DwRect& rect)
{
    return m_nativeRegion->Union(DwRect2GdiRect(rect)) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::uniond(const DwRectF& rect)
{
    return m_nativeRegion->Union(DwRect2GdiRect(rect)) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::uniond(const DwRegion* region)
{
    return m_nativeRegion->Union(region->nativeRegion()) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::xor(const DwRect& rect)
{
    return m_nativeRegion->Xor(DwRect2GdiRect(rect)) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::xor(const DwRectF& rect)
{
    return m_nativeRegion->Xor(DwRect2GdiRect(rect)) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::xor(const DwRegion* region)
{
    return m_nativeRegion->Xor(region->nativeRegion()) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::exclude(const DwRect& rect)
{
    return m_nativeRegion->Exclude(DwRect2GdiRect(rect)) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::exclude(const DwRectF& rect)
{
    return m_nativeRegion->Exclude(DwRect2GdiRect(rect)) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::exclude(const DwRegion* region)
{
    return m_nativeRegion->Exclude(region->nativeRegion()) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::complement(const DwRect& rect)
{
    return m_nativeRegion->Complement(DwRect2GdiRect(rect)) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::complement(const DwRectF& rect)
{
    return m_nativeRegion->Complement(DwRect2GdiRect(rect)) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::complement(const DwRegion* region)
{
    return m_nativeRegion->Complement(region->nativeRegion()) == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
DwRegion::DwRegion(Gdiplus::Region *v)
{
    DW_ASSERT(v);
    m_nativeRegion = v;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::clear()
{
    return m_nativeRegion->MakeEmpty() == Gdiplus::Ok;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::isEmpty()
{
    return boundingRect().isEmpty();
}

//------------------------------------------------------------------------------
/**
*/
DwRect DwRegion::boundingRect()const
{
    Gdiplus::Rect r;
    m_nativeRegion->GetBounds(&r, DwCanvas::defaultGraphics());
    return DwRect(r.GetLeft(), r.GetTop(), r.Width, r.Height);
}

//------------------------------------------------------------------------------
/**
*/
bool DwRegion::isIntersects(const DwRect& rect)const
{
    return !!(m_nativeRegion->IsVisible(DwRect2GdiRect(rect)));
}

//------------------------------------------------------------------------------
/**
*/
DwRegion * DwRegion::clone()
{
    return new DwRegion(m_nativeRegion->Clone());
}