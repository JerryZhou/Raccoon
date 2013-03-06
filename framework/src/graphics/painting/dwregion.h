#pragma once
#include "dwcore/dwrect.h"

//------------------------------------------------------------------------------
namespace Gdiplus
{
class Region;
};

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwRegion
{
public:
    DwRegion();
    DwRegion(int x1, int y1, int x2, int y2, int w, int h);
    DwRegion(DwRect r, int w, int h);
    DwRegion(DwRect r);
    DwRegion(const DwRegion &r);
    DwRegion& operator=(const DwRegion &r);
    virtual ~DwRegion();

    bool intersect(const DwRect& rect);
    bool intersect(const DwRectF& rect);
    bool intersect(const DwRegion* region);
    bool uniond(const DwRect& rect);
    bool uniond(const DwRectF& rect);
    bool uniond(const DwRegion* region);
    bool xor(const DwRect& rect);
    bool xor(const DwRectF& rect);
    bool xor(const DwRegion* region);
    bool exclude(const DwRect& rect);
    bool exclude(const DwRectF& rect);
    bool exclude(const DwRegion* region);
    bool complement(const DwRect& rect);
    bool complement(const DwRectF& rect);
    bool complement(const DwRegion* region);

    bool clear();
    bool isEmpty();
    DwRect boundingRect()const;
    bool isIntersects(const DwRect& rect)const;

    DwRegion * clone();

    const Gdiplus::Region *nativeRegion() const;
    HRGN nativeRGN() const;

protected:
    DwRegion(Gdiplus::Region *);

    Gdiplus::Region *m_nativeRegion;
};// end of DwRegion