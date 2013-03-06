#pragma once
#include "dwcore/dwtypes.h"
#include "dwcore/dwrect.h"
#include "dwcore/dwpoint.h"
#include "dwcore/dwsize.h"
#include "dwcore/dwstring.h"
#include "dwcore/dwmargins.h"

//------------------------------------------------------------------------------
class DW_CORE_EXPORT DwVariant
{
public:
    enum Type
    {
        Invalid = 0,

        Char,
        UChar,
        Bool,
        Int,
        UInt,
        Double,
        Float,
        Int64,
        UInt64,
        Ptr,
        RectF,
        Rect,
        PointF,
        Point,
        SizeF,
        Size,
        String,
        Margins,
    };
    DwVariant();
    ~DwVariant();

    inline bool isValid() const
    {
        return type != Invalid;
    }
    bool isNull() const;
    int vtype()const
    {
        return this->type;
    }

    void clear();

    DwVariant(char);
    DwVariant(unsigned char);
    DwVariant(bool b);
    DwVariant(int i);
    DwVariant(uint32_t ui);
    DwVariant(double d);
    DwVariant(float f);
    DwVariant(int64_t);
    DwVariant(uint64_t);
    DwVariant(void* p);
    DwVariant(const DwRectF &r);
    DwVariant(const DwRect &r);
    DwVariant(const DwPointF &p);
    DwVariant(const DwPoint &p);
    DwVariant(const DwSizeF &s);
    DwVariant(const DwSize &s);
    DwVariant(const DwString &s);
    DwVariant(const DwMargins &m);

    DwVariant& operator=(const DwVariant& other);
    DwVariant(const DwVariant& other);

    // will accurate matching
    char toChar(bool *ok = 0) const;
    unsigned char toUChar(bool *ok = 0) const;
    bool toBool(bool *ok = 0) const;
    int toInt(bool *ok = 0) const;
    uint32_t toUInt(bool *ok = 0) const;
    double toDouble(bool *ok = 0) const;
    float toFloat(bool *ok = 0) const;
    int64_t toInt64(bool *ok = 0) const;
    uint64_t toUInt64(bool *ok = 0) const;
    void* toPtr(bool *ok = 0) const;
    DwRectF toRectF(bool *ok = 0) const;
    DwRect toRect(bool *ok = 0) const;
    DwPointF toPointF(bool *ok = 0) const;
    DwPoint toPoint(bool *ok = 0) const ;
    DwSizeF toSizeF(bool *ok = 0) const;
    DwSize toSize(bool *ok = 0) const ;
    DwString toString(bool *ok = 0) const;
    DwMargins toMargins(bool *ok = 0) const;

    // TODO: xx
    // fuzzy matching
    template<typename T>
    T as(bool *ok = 0) const;

    template<>
    char as<char>(bool *ok/* = 0*/) const
    {
        switch (type)
        {
        case Bool:
            return char(toBool(ok));
            break;
        case UChar:
            return char(toUChar(ok));
            break;
        case Int:
            return char(toInt(ok));
            break;
        case Double:
            return char(toDouble(ok));
            break;
        case UInt:
            return char(toUInt(ok));
            break;
        case Int64:
            return char(toInt64(ok));
            break;
        case UInt64:
            return char(toUInt64(ok));
            break;
        case Float:
            return char(toFloat(ok));
            break;
        default:
            break;
        }

        return toChar(ok);
    }

    template<>
    unsigned char as<unsigned char>(bool *ok/* = 0*/) const
    {
        switch (type)
        {
        case Bool:
            return (unsigned char)(toBool(ok));
            break;
        case Char:
            return (unsigned char)(toChar(ok));
            break;
        case Int:
            return (unsigned char)(toInt(ok));
            break;
        case Double:
            return (unsigned char)(toDouble(ok));
            break;
        case UInt:
            return (unsigned char)(toUInt(ok));
            break;
        case Int64:
            return (unsigned char)(toInt64(ok));
            break;
        case UInt64:
            return (unsigned char)(toUInt64(ok));
            break;
        case Float:
            return (unsigned char)(toFloat(ok));
            break;
        default:
            break;
        }
        return toUChar(ok);
    }

    template<>
    bool as<bool>(bool *ok/* = 0*/) const
    {
        switch (type)
        {
        case Char:
            return bool(!!toChar(ok));
            break;
        case UChar:
            return bool(!!toUChar(ok));
            break;
        case Int:
            return bool(!!toInt(ok));
            break;
        case Double:
            return bool(!!toDouble(ok));
            break;
        case UInt:
            return bool(!!toUInt(ok));
            break;
        case Int64:
            return bool(!!toInt64(ok));
            break;
        case UInt64:
            return bool(!!toUInt64(ok));
            break;
        case Float:
            return bool(!!toFloat(ok));
            break;
        default:
            break;
        }

        return toBool(ok);
    }

    template<>
    int as<int>(bool *ok/* = 0*/) const
    {
        switch (type)
        {
        case Char:
            return int(toChar(ok));
            break;
        case UChar:
            return int(toUChar(ok));
            break;
        case Bool:
            return int(toBool(ok));
            break;
        case Double:
            return int(toDouble(ok));
            break;
        case UInt:
            return int(toUInt(ok));
            break;
        case Int64:
            return int(toInt64(ok));
            break;
        case UInt64:
            return int(toUInt64(ok));
            break;
        case Float:
            return int(toFloat(ok));
            break;
        default:
            break;
        }

        return toInt(ok);
    }

    template<>
    uint32_t as<uint32_t>(bool *ok/* = 0*/) const
    {
        switch (type)
        {
        case Char:
            return uint32_t(toChar(ok));
            break;
        case UChar:
            return uint32_t(toUChar(ok));
            break;
        case Bool:
            return uint32_t(toBool(ok));
            break;
        case Double:
            return uint32_t(toDouble(ok));
            break;
        case Int:
            return uint32_t(toInt(ok));
            break;
        case Int64:
            return uint32_t(toInt64(ok));
            break;
        case UInt64:
            return uint32_t(toUInt64(ok));
            break;
        case Float:
            return uint32_t(toFloat(ok));
            break;
        default:
            break;
        }
        return toUInt(ok);
    }

    template<>
    double as<double>(bool *ok/* = 0*/) const
    {
        switch (type)
        {
        case Char:
            return double(toChar(ok));
            break;
        case UChar:
            return double(toUChar(ok));
            break;
        case Bool:
            return double(toBool(ok));
            break;
        case Int:
            return double(toInt(ok));
            break;
        case UInt:
            return double(toUInt(ok));
            break;
        case Int64:
            return double(toInt64(ok));
            break;
        case UInt64:
            return double(toUInt64(ok));
            break;
        case Float:
            return double(toFloat(ok));
            break;
        default:
            break;
        }

        return toDouble(ok);
    }

    template<>
    float as<float>(bool *ok/* = 0*/) const
    {
        switch (type)
        {
        case Char:
            return float(toChar(ok));
            break;
        case UChar:
            return float(toUChar(ok));
            break;
        case Bool:
            return float(toBool(ok));
            break;
        case Int:
            return float(toInt(ok));
            break;
        case UInt:
            return float(toUInt(ok));
            break;
        case Int64:
            return float(toInt64(ok));
            break;
        case UInt64:
            return float(toUInt64(ok));
            break;
        case Double:
            return float(toDouble(ok));
            break;
        default:
            break;
        }

        return toFloat(ok);
    }

    template<>
    int64_t as<int64_t>(bool *ok/* = 0*/) const
    {
        return toInt64(ok);
    }
    template<>
    uint64_t as<uint64_t>(bool *ok/* = 0*/) const
    {
        return toUInt64(ok);
    }
    template<>
    void* as<void*>(bool *ok/* =0 */) const
    {
        return toPtr(ok);
    }

    template<>
    DwRectF as<DwRectF>(bool *ok/* = 0*/) const
    {
        switch (type)
        {
        case Rect:
            return DwRectF(toRect(ok));
            break;
        default:
            break;
        }

        return toRectF(ok);
    }

    template<>
    DwRect as<DwRect>(bool *ok/* = 0*/) const
    {
        switch (type)
        {
        case RectF:
            return toRectF(ok).toRect();
            break;
        default:
            break;
        }

        return toRect(ok);
    }

    template<>
    DwPointF as<DwPointF>(bool *ok/* = 0*/) const
    {
        switch (type)
        {
        case Point:
            return DwPointF(toPoint(ok));
            break;
        default:
            break;
        }

        return toPointF(ok);
    }

    template<>
    DwPoint as<DwPoint>(bool *ok/* = 0*/) const
    {
        switch (type)
        {
        case PointF:
            return toPointF(ok).toPoint();
            break;
        default:
            break;
        }

        return toPoint(ok);
    }

    template<>
    DwSizeF as<DwSizeF>(bool *ok/* = 0*/) const
    {
        switch (type)
        {
        case Size:
            return DwSizeF(toSizeF(ok));
            break;
        default:
            break;
        }

        return toSizeF(ok);
    }

    template<>
    DwSize as<DwSize>(bool *ok/* = 0*/) const
    {
        switch (type)
        {
        case SizeF:
            return toSizeF(ok).toSize();
            break;
        default:
            break;
        }

        return toSize(ok);
    }

    template<>
    DwString as<DwString>(bool *ok/* =0 */) const
    {
        switch (type)
        {
        case Char:
            return DwString(wchar_t(toChar(ok)));
            break;
        case UChar:
            return DwString(wchar_t(toUChar(ok)));
            break;
        case Int:
            return DwString::number(toInt(ok));
            break;
        default:
            break;
        }

        return toString(ok);
    }

    template<>
    DwMargins as<DwMargins>(bool *ok/* =0 */) const
    {
        return toMargins(ok);
    }

    template<typename T>
    operator T () const
    {
        return as<T>();
    }
private:
    void releae();

    union Data
    {
        char c;
        unsigned char uc;
        int i;
        uint32_t u;
        bool b;
        double d;
        float f;
        double real;
        int64_t ll;
        uint64_t ull;
        DwRectF* rectf;
        DwRect* rect;
        DwPointF* pointf;
        DwPoint* point;
        DwSizeF* sizef;
        DwSize* size;
        DwString* s;
        DwMargins* m;
        void *ptr;
    } data;
    uint32_t type : 30;
    uint32_t is_shared : 1;
    uint32_t is_null : 1;
};