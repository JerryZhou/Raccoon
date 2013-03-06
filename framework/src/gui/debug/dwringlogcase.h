#pragma once

#include "dwgui/dwringlog.h"
#include "dwcore/dwrect.h"
#include "dwcore/dwsize.h"
#include "dwcore/dwstring.h"
#include "dwcore/dwpoint.h"
#include "dwcore/dwmargins.h"
#include "dwcore/dwvariant.h"
#include "dwcore/dwvector.h"
#include "dwcore/dwmap.h"
#include "dwcore/dwbytearray.h"
#include "dwgraphics/dwtransform.h"
#include "dwgraphics/dwmatrix.h"
#include "dwgraphics/dwimage.h"
#include "dwgraphics/dwninepatch.h"
#include "dwgui/dwringloglib.h"


//------------------------------------------------------------------------------
/**
*/
template<size_t N>
inline DwRingLog& operator << (DwRingLog& o, char const (&t)[N])
{
    o << (char*)(t);
    return o;
}

//------------------------------------------------------------------------------
/**
*/
template<size_t N>
inline DwRingLog& operator << (DwRingLog& o, wchar_t const (&t)[N])
{
    o << (wchar_t*)(t);
    return o;
}

//------------------------------------------------------------------------------
/**
*/
typedef char * DwCharPtr;
template<>
inline DwRingLog & DwRingLog::operator<< <DwCharPtr>(const DwCharPtr& v)
{
    format("%s", v ? v : "NULL");
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <int>(const int& v)
{
    format("%d", v);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <unsigned int>(const unsigned int& v)
{
    format("%u", v);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <short>(const short& v)
{
    format("%d", v);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <unsigned short>(const unsigned short& v)
{
    format("%u", v);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <long>(const long& v)
{
    format("%d", v);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <unsigned long>(const unsigned long& v)
{
    format("%u", v);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <int64_t>(const int64_t& v)
{
    format("%lld", v);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <uint64_t>(const uint64_t& v)
{
    format("%llu", v);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
typedef void * DwVoidPtr;
template<>
inline DwRingLog & DwRingLog::operator<< <DwVoidPtr>(const DwVoidPtr &p)
{
    if (p)
    {
        format("0x%p", p);
    }
    else
    {
        format("%s", "NULL");
    }
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <char>(const char& v)
{
    format("%c", v);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <unsigned char>(const unsigned char& v)
{
    format("%c", v);
    return *this;
}

// The wchar_t is unsigned short
//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <bool>(const bool& v)
{
    format("%s", v ? "true" : "false");
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <float>(const float& v)
{
    format("%.6f", v);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <double>(const double& v)
{
    format("%.6f", v);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwString>(const DwString& v)
{
    if (v.length() > 0)
    {
        format("%s", v.toUtf8().data());
    }
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
typedef wchar_t * DwWCharPtr;
template<>
inline DwRingLog & DwRingLog::operator<< <DwWCharPtr>(const DwWCharPtr& v)
{
    this->operator<< (DwString((v ? v : L"NULL")));
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwRectF>(const DwRectF& v)
{
    format("(left: %.6f, top: %.6f, width: %.6f, height: %.6f)", v.left(), v.top(), v.width(), v.height());
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwRect>(const DwRect& v)
{
    format("(left: %d, top: %d, width: %d, height: %d)", v.left(), v.top(), v.width(), v.height());
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwSizeF>(const DwSizeF& v)
{
    format("(width: %.6f, height: %.6f)",  v.width(), v.height());
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwSize>(const DwSize& v)
{
    format("(width: %d, height: %d)",  v.width(), v.height());
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwPoint>(const DwPoint& v)
{
    format("(x: %d, y: %d)",  v.x(), v.y());
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwPointF>(const DwPointF& v)
{
    format("(x: %.6f, y: %.6f)",  v.x(), v.y());
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwMargins>(const DwMargins& v)
{
    format("(left: %d, top: %d, right: %d, bottom: %d)", v.left(), v.top(), v.right(), v.bottom());
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwMatrix>(const DwMatrix& v)
{
    format("(m11: %.6f, m12: %.6f, \
m21: %.6f, m22: %.6f, \
dx: %.6f, dy: %.6f)",
           v.m11(), v.m12(),
           v.m21(), v.m22(),
           v.dx(), v.dy());
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwImage>(const DwImage& v)
{
    (*this) << "img: path-"<< v.path() 
        << " uniqueKey-" << v.uniqueKey() 
        <<" size-" << v.size() 
        << " refcount-" << v.refCnt();
    if (v.frameCount() > 1)
    {
        (*this) << " frameCount-" << v.frameCount();
        (*this) << " curActiveFrame-" << v.curActiveFrame();
        (*this) << " nextInterval-" << v.nextInterval();
    }
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwTransform>(const DwTransform& v)
{
    format("(m11: %.6f, m12: %.6f, m13: %.6f, \
m21: %.6f, m22: %.6f, m23: %.6f, \
m31: %.6f, m32: %.6f, m33: %.6f, \
dx: %.6f, dy: %.6f)",
           v.m11(), v.m12(),  v.m13(),
           v.m21(), v.m22(), v.m23(),
           v.m31(), v.m32(), v.m33(),
           v.dx(), v.dy());
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwVariant>(const DwVariant& v)
{
    switch (v.vtype())
    {
    case DwVariant::Char:
        this->operator<<(v.toChar());
        break;
    case DwVariant::UChar:
        this->operator<<(v.toUChar());
        break;
    case DwVariant::Bool:
        this->operator<<(v.toBool());
        break;
    case DwVariant::Int:
        this->operator<<(v.toInt());
        break;
    case DwVariant::UInt:
        this->operator<<(v.toUInt());
        break;
    case DwVariant::Double:
        this->operator<<(v.toDouble());
        break;
    case DwVariant::Float:
        this->operator<<(v.toFloat());
        break;
    case DwVariant::Int64:
        this->operator<<(v.toInt64());
        break;
    case DwVariant::UInt64:
        this->operator<<(v.toUInt64());
        break;
    case DwVariant::Ptr:
        (*this)<<(v.toPtr());
        break;
    case DwVariant::RectF:
        this->operator<<(v.toRectF());
        break;
    case DwVariant::PointF:
        this->operator<<(v.toPointF());
        break;
    case DwVariant::SizeF:
        this->operator<<(v.toSizeF());
        break;
    case DwVariant::String:
        this->operator<<(v.toString());
        break;
    case DwVariant::Invalid:
        (*this) <<"NULL";
    }
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwRingLog & DwRingLog::operator<< <DwNinePatch::NinePatchInfo>(const DwNinePatch::NinePatchInfo& v)
{
    (*this) << "sourceRect: ";
    this->operator<<(v.srcRect);
    (*this) << "margins: ";
    this->operator<<(v.margins);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline DwRingLog& operator << (DwRingLog& o, const DwVector<T> &v)
{
    if (v.size() > 0)
    {
        o << "size: " << v.size() << " ";
        o << v.at(0);
        for (int i=1; i<v.size(); ++i)
        {
            o << ", ";
            o << v.at(i);
        }
    }
    return o;
}

//------------------------------------------------------------------------------
/**
*/
template<typename KEY, typename VALUE>
inline DwRingLog& operator << (DwRingLog& o, const DwMap<KEY, VALUE> &v)
{
    if (v.count() > 0)
    {
        DwMap<KEY, VALUE>::const_iterator ite = v.begin();
        o << "(" << (*ite).first << ", " << (*ite).second << ")";
        for (++ite; ite!=v.end(); ++ite)
        {
            o << ", ";
            o << "(" << (*ite).first << ", " << (*ite).second << ")";
        }
    }
    return o;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline DwString asString(const T& v)
{
    DwRingLog o;
    o << v;
    const char * p = o.data();
    if (strlen(p) > 0)
    {
        return DwString(p);
    }
    return DwString();
}

// THREAD SAFE, ONLY IN ANY THREAD
//------------------------------------------------------------------------------
#define DW_UI_LOGER() (*DwRingLogLib::Instance()->log())
#define DW_UI_LOG(fmt, ...) DwRingLogLib::Instance()->log()->format(fmt,  __VA_ARGS__)
#define DW_UI_LOG_FLUSH(url) DwRingLogLib::Instance()->log()->dump(DwUrl(url))

#define DW_NET_LOGER() (*DwRingLogLib::Instance()->log())
#define DW_NET_LOG(fmt, ...) DwRingLogLib::Instance()->log()->format(fmt,  __VA_ARGS__)
#define DW_NET_LOG_FLUSH(url) DwRingLogLib::Instance()->log()->dump(DwUrl(url))

#define DW_LOGR()   (*DwRingLogLib::Instance()->log())
#define DW_LOG(fmt, ...) DwRingLogLib::Instance()->log()->format(fmt,  __VA_ARGS__)
#define DW_LOG_FLUSH(url) DwRingLogLib::Instance()->log()->dump(DwUrl(url))
#define DW_END "\n"
#define DW_SEP " "