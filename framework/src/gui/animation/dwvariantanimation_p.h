#pragma once
// SHOULD ONLY INCLUDED IN DWVARIANTANIMATION.CPP
//------------------------------------------------------------------------------
/**
*/
static bool animationValueLessThan(const DwVariantAnimation::KeyValue &p1, const DwVariantAnimation::KeyValue &p2)
{
    return p1.first < p2.first;
}

struct animationValueLessThanFunction
        : public eastl::binary_function< DwVariantAnimation::KeyValue ,
          DwVariantAnimation::KeyValue , bool>
{
    bool operator() (const DwVariantAnimation::KeyValue &p1, const DwVariantAnimation::KeyValue &p2) const
    {
        return animationValueLessThan(p1, p2);
    }
};

//------------------------------------------------------------------------------
/**
*/
static DwVariant defaultInterpolator(const DwVariant &, const DwVariant &, DwReal )
{
    return DwVariant();
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline T _dw_interpolator(const T& from, const T& to, DwReal progress)
{
    return (T)(from * (1.0 - progress) + to * progress);
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRectF _dw_interpolator(const DwRectF& f, const DwRectF& t, DwReal progress)
{
    DwReal x1, y1, w1, h1;
    f.getRect(&x1, &y1, &w1, &h1);
    DwReal x2, y2, w2, h2;
    t.getRect(&x2, &y2, &w2, &h2);
    return DwRectF(_dw_interpolator(x1, x2, progress), _dw_interpolator(y1, y2, progress),
                   _dw_interpolator(w1, w2, progress), _dw_interpolator(h1, h2, progress));
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRect _dw_interpolator(const DwRect& f, const DwRect& t, DwReal progress)
{
    int x1, y1, w1, h1;
    f.getRect(&x1, &y1, &w1, &h1);
    int x2, y2, w2, h2;
    t.getRect(&x2, &y2, &w2, &h2);
    return DwRect(_dw_interpolator(x1, x2, progress), _dw_interpolator(y1, y2, progress),
                  _dw_interpolator(w1, w2, progress), _dw_interpolator(h1, h2, progress));
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwPointF _dw_interpolator(const DwPointF& from, const DwPointF& to, DwReal progress)
{
    return from * (1.0 - progress) + to * progress;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwPoint _dw_interpolator(const DwPoint& from, const DwPoint& to, DwReal progress)
{
    return from * (1.0 - progress) + to * progress;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwSizeF _dw_interpolator(const DwSizeF& from, const DwSizeF& to, DwReal progress)
{
    return from * (1.0 - progress) + to * progress;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwSize _dw_interpolator(const DwSize& from, const DwSize& to, DwReal progress)
{
    return from * (1.0 - progress) + to * progress;
}

//------------------------------------------------------------------------------
/**
*/
template<DwVariant::Type kind>
inline DwVariant _dw_v_interpolator(const DwVariant &from, const DwVariant &to, DwReal progress)
{
    return defaultInterpolator(from, to, progress);
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwVariant _dw_v_interpolator<DwVariant::Int>(const DwVariant &from, const DwVariant &to, DwReal progress)
{
    return (int)( _dw_interpolator(1.0 * from.as<int>(), 1.0* to.as<int>(), progress) );
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwVariant _dw_v_interpolator<DwVariant::UInt>(const DwVariant &from, const DwVariant &to, DwReal progress)
{
    return (uint32_t)( _dw_interpolator(1.0 * from.as<uint32_t>(), 1.0* to.as<uint32_t>(), progress) );
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwVariant _dw_v_interpolator<DwVariant::Double>(const DwVariant &from, const DwVariant &to, DwReal progress)
{
    return _dw_interpolator(from.as<double>(), to.as<double>(), progress);
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwVariant _dw_v_interpolator<DwVariant::Float>(const DwVariant &from, const DwVariant &to, DwReal progress)
{
    return _dw_interpolator(from.as<float>(), to.as<float>(), progress);
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwVariant _dw_v_interpolator<DwVariant::Int64>(const DwVariant &from, const DwVariant &to, DwReal progress)
{
    return _dw_interpolator(from.as<int64_t>(), to.as<int64_t>(), progress);
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwVariant _dw_v_interpolator<DwVariant::UInt64>(const DwVariant &from, const DwVariant &to, DwReal progress)
{
    return _dw_interpolator(from.as<uint64_t>(), to.as<uint64_t>(), progress);
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwVariant _dw_v_interpolator<DwVariant::RectF>(const DwVariant &from, const DwVariant &to, DwReal progress)
{
    return _dw_interpolator(from.as<DwRectF>(), to.as<DwRectF>(), progress);
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwVariant _dw_v_interpolator<DwVariant::Rect>(const DwVariant &from, const DwVariant &to, DwReal progress)
{
    return _dw_interpolator(from.as<DwRect>(), to.as<DwRect>(), progress);
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwVariant _dw_v_interpolator<DwVariant::PointF>(const DwVariant &from, const DwVariant &to, DwReal progress)
{
    return _dw_interpolator(from.as<DwPointF>(), to.as<DwPointF>(), progress);
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwVariant _dw_v_interpolator<DwVariant::Point>(const DwVariant &from, const DwVariant &to, DwReal progress)
{
    return _dw_interpolator(from.as<DwPoint>(), to.as<DwPoint>(), progress);
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwVariant _dw_v_interpolator<DwVariant::SizeF>(const DwVariant &from, const DwVariant &to, DwReal progress)
{
    return _dw_interpolator(from.as<DwSizeF>(), to.as<DwSizeF>(), progress);
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwVariant _dw_v_interpolator<DwVariant::Size>(const DwVariant &from, const DwVariant &to, DwReal progress)
{
    return _dw_interpolator(from.as<DwSize>(), to.as<DwSize>(), progress);
}

//------------------------------------------------------------------------------
/**
*/
#ifndef dwFind
#define dwFind eastl::find_if
#endif

//------------------------------------------------------------------------------
/**
*/
#ifndef dwBind2nd
#define dwBind2nd eastl::bind2nd
#endif

//------------------------------------------------------------------------------
/**
*/
#ifndef dwLowerBound
#define dwLowerBound eastl::lower_bound
#endif

//------------------------------------------------------------------------------
/**
*/
#ifndef dwMakePair
#define dwMakePair eastl::make_pair
#endif