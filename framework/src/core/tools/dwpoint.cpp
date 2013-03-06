#include "stable.h"

#include "dwpoint.h"

/************************************************************************
	DwPoint
************************************************************************/

DwPoint& DwPoint::operator+=( const DwPoint & point )
{
    m_x += point.m_x;
    m_y += point.m_y;
    return *this;
}

DwPoint& DwPoint::operator-=( const DwPoint & point )
{
    m_x -= point.m_x;
    m_y -= point.m_y;
    return *this;
}

DwPoint& DwPoint::operator*=( double factor )
{
    m_x = dwRound(m_x * factor);
    m_y = dwRound(m_y * factor);
    return *this;
}

DwPoint& DwPoint::operator/=( double divisor )
{
    m_x = dwRound(m_x / divisor);
    m_y = dwRound(m_y / divisor);
    return *this;
}


/************************************************************************
	DwPointF
************************************************************************/


DwPointF& DwPointF::operator+=( const DwPointF & point )
{
    m_x += point.m_x;
    m_y += point.m_y;
    return *this;
}

DwPointF& DwPointF::operator-=( const DwPointF & point )
{
    m_x -= point.m_x;
    m_y -= point.m_y;
    return *this;
}

DwPointF& DwPointF::operator*=( double factor )
{
    m_x *= factor;
    m_y *= factor;
    return *this;
}

DwPointF& DwPointF::operator/=( double divisor )
{
    m_x /= divisor;
    m_y /= divisor;
    return *this;
}
