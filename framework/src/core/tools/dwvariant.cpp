#include "stable.h"

#include "dwvariant.h"
#include "dwvariant_p.h"

DwVariant::DwVariant()
{
    is_null = true;
    type = Invalid;
}

DwVariant::~DwVariant()
{
    releae();
}

bool DwVariant::isNull() const
{
    return is_null;
}

void DwVariant::clear()
{
    releae();
}

DW_VARAINT_CONSTRUCTOR(char, Char, c);
DW_VARAINT_CONSTRUCTOR(unsigned char, UChar, uc);
DW_VARAINT_CONSTRUCTOR(bool, Bool, b);
DW_VARAINT_CONSTRUCTOR(int, Int, i);
DW_VARAINT_CONSTRUCTOR(uint32_t, UInt, u);
DW_VARAINT_CONSTRUCTOR(double, Double, d);
DW_VARAINT_CONSTRUCTOR(float, Float, f);
DW_VARAINT_CONSTRUCTOR(int64_t, Int64, ll);
DW_VARAINT_CONSTRUCTOR(uint64_t, UInt64, ull);
DW_VARAINT_CONSTRUCTOR(void*, Ptr, ptr);
DW_VARIANT_PCONSTRUCTOR(DwRectF, RectF, rectf);
DW_VARIANT_PCONSTRUCTOR(DwRect, Rect, rect);
DW_VARIANT_PCONSTRUCTOR(DwPointF, PointF, pointf);
DW_VARIANT_PCONSTRUCTOR(DwPoint, Point, point);
DW_VARIANT_PCONSTRUCTOR(DwSizeF, SizeF, sizef);
DW_VARIANT_PCONSTRUCTOR(DwSize, Size, size);
DW_VARIANT_PCONSTRUCTOR(DwString, String, s);
DW_VARIANT_PCONSTRUCTOR(DwMargins, Margins, m);

DwVariant& DwVariant::operator=(const DwVariant& other)
{
    if (this == &other)
    {
        return *this;
    }

    if (type != other.type)
    {
        releae();
    }
    switch (other.type)
    {
        DW_VARIANT_OPERATOR(char, Char, c);
        DW_VARIANT_OPERATOR(unsigned char, UChar, uc);
        DW_VARIANT_OPERATOR(bool, Bool, b);
        DW_VARIANT_OPERATOR(int, Int, i);
        DW_VARIANT_OPERATOR(uint32_t, UInt, u);
        DW_VARIANT_OPERATOR(double, Double, d);
        DW_VARIANT_OPERATOR(float, Float, f);
        DW_VARIANT_OPERATOR(int64_t, Int64, ll);
        DW_VARIANT_OPERATOR(uint64_t, UInt64, ull);
        DW_VARIANT_OPERATOR(void*, Ptr, ptr);
        DW_VARIANT_POPERATOR(DwRectF, RectF, rectf);
        DW_VARIANT_POPERATOR(DwRect, Rect, rect);
        DW_VARIANT_POPERATOR(DwPointF, PointF, pointf);
        DW_VARIANT_POPERATOR(DwPoint, Point, point);
        DW_VARIANT_POPERATOR(DwSizeF, SizeF, sizef);
        DW_VARIANT_POPERATOR(DwSize, Size, size);
        DW_VARIANT_POPERATOR(DwString, String, s);
        DW_VARIANT_POPERATOR(DwMargins, Margins, m);
    }
    type = other.type;
    is_null = other.is_null;

    return *this;
}

DwVariant::DwVariant(const DwVariant& other)
{
    is_null = true;
    type = Invalid;
    *this = other;
}

DW_VARIANT_TO_WITH(char, Char, c, 0);
DW_VARIANT_TO_WITH(unsigned char, UChar, uc, 0);
DW_VARIANT_TO_WITH(bool, Bool, b, false);
DW_VARIANT_TO_WITH(int, Int, i, 0);
DW_VARIANT_TO_WITH(uint32_t, UInt, u, 0);
DW_VARIANT_TO_WITH(double, Double, d, 0.0);
DW_VARIANT_TO_WITH(float, Float, f, 0.0f);
DW_VARIANT_TO_WITH(int64_t, Int64, ll, 0);
DW_VARIANT_TO_WITH(uint64_t, UInt64, ull, 0);
DW_VARIANT_TO_WITH(void*, Ptr, ptr, NULL);
DW_VARIANT_PTO(DwRectF, RectF, rectf);
DW_VARIANT_PTO(DwRect, Rect, rect);
DW_VARIANT_PTO(DwPointF, PointF, pointf);
DW_VARIANT_PTO(DwPoint, Point, point);
DW_VARIANT_PTO(DwSizeF, SizeF, sizef);
DW_VARIANT_PTO(DwSize, Size, size);
DW_VARIANT_PTO(DwString, String, s);
DW_VARIANT_PTO(DwMargins, Margins, m);

void DwVariant::releae()
{
    switch (type)
    {
        DW_VARIANT_RELEASE(char, Char, c);
        DW_VARIANT_RELEASE(unsigned char, UChar, uc);
        DW_VARIANT_RELEASE(bool, Bool, b);
        DW_VARIANT_RELEASE(int, Int, i);
        DW_VARIANT_RELEASE(uint32_t, UInt, u);
        DW_VARIANT_RELEASE(double, Double, d);
        DW_VARIANT_RELEASE(float, Float, f);
        DW_VARIANT_RELEASE(int64_t, Int64, ll);
        DW_VARIANT_RELEASE(uint64_t, UInt64, ull);
        DW_VARIANT_RELEASE(void*, Ptr, ptr);
        DW_VARIANT_PRELEASE(DwRectF, RectF, rectf);
        DW_VARIANT_PRELEASE(DwRect, Rect, rect);
        DW_VARIANT_PRELEASE(DwPointF, PointF, pointf);
        DW_VARIANT_PRELEASE(DwPoint, Point, point);
        DW_VARIANT_PRELEASE(DwSizeF, SizeF, sizef);
        DW_VARIANT_PRELEASE(DwSize, Size, size);
        DW_VARIANT_PRELEASE(DwString, String, s);
        DW_VARIANT_PRELEASE(DwMargins, Margins, m);
    default:
        break;
    }

    is_null = true;
    type = Invalid;
}
