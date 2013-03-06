#pragma once

#include "../dwcore.h"
#include "dwcore/dwdebug.h"

/************************************************************************
	DwPoint
************************************************************************/

class DW_CORE_EXPORT DwPoint
{
public:
    DwPoint();
    DwPoint( int x, int y );

    bool	isNull() const;

    int		x() const;
    int		y() const;
    void	setX( int x );
    void	setY( int y );

    int&	rx();
    int&	ry();

    int		manhattanLength() const;

    DwPoint&	operator+=( const DwPoint & point );
    DwPoint&	operator-=( const DwPoint & point );
    DwPoint&	operator*=( double factor );
    DwPoint&	operator/=( double divisor );

    friend bool	operator==( const DwPoint & p1, const DwPoint & p2 );
    friend bool	operator!=( const DwPoint & p1, const DwPoint & p2 );
    friend const DwPoint	operator+( const DwPoint & p1, const DwPoint & p2 );
    friend const DwPoint	operator-( const DwPoint & p1, const DwPoint & p2 );
    friend const DwPoint	operator-( const DwPoint & point );
    friend const DwPoint	operator*( const DwPoint & point, double factor );
    friend const DwPoint	operator*( double factor, const DwPoint & point );
    friend const DwPoint	operator/( const DwPoint & point, double divisor );

private:
    int m_x;
    int m_y;
};

/************************************************************************
	DwPoint inline fuctions
************************************************************************/


inline DwPoint::DwPoint()
    : m_x(0), m_y(0)
{
}

inline DwPoint::DwPoint( int x, int y )
    : m_x(x), m_y(y)
{
}

inline bool DwPoint::isNull() const
{
    return m_x == 0 && m_y == 0;
}

inline int DwPoint::x() const
{
    return m_x;
}

inline int DwPoint::y() const
{
    return m_y;
}

inline void DwPoint::setX( int x )
{
    m_x = x;
}

inline void DwPoint::setY( int y )
{
    m_y = y;
}

inline int& DwPoint::rx()
{
    return m_x;
}

inline int& DwPoint::ry()
{
    return m_y;
}

inline int DwPoint::manhattanLength() const
{
    return dwAbs(m_x) + dwAbs(m_y);
}

inline bool	operator==( const DwPoint & p1, const DwPoint & p2 )
{
    return p1.m_x == p2.m_x && p1.m_y == p2.m_y;
}

inline bool	operator!=( const DwPoint & p1, const DwPoint & p2 )
{
    return p1.m_x != p2.m_x || p1.m_y != p2.m_y;
}

inline const DwPoint operator+( const DwPoint & p1, const DwPoint & p2 )
{
    return DwPoint(p1.m_x, p1.m_y) += p2;
}

inline const DwPoint operator-( const DwPoint & p1, const DwPoint & p2 )
{
    return DwPoint(p1.m_x, p1.m_y) -= p2;
}

inline const DwPoint operator-( const DwPoint & point )
{
    return DwPoint(- point.m_x, - point.m_y);
}

inline const DwPoint operator*( double factor, const DwPoint & point )
{
    return DwPoint(point.m_x, point.m_y) *= factor;
}

inline const DwPoint operator*( const DwPoint & point, double factor )
{
    return DwPoint(point.m_x, point.m_y) *= factor;
}

inline const DwPoint operator/( const DwPoint & point, double divisor )
{
    return DwPoint(point.m_x, point.m_y) /= divisor;
}


/************************************************************************
	DwPonitF
************************************************************************/

class DW_CORE_EXPORT DwPointF
{
public:
    DwPointF();
    DwPointF( double x, double y );
    DwPointF(const DwPoint &p);

    bool	isNull() const;

    double	x() const;
    double	y() const;
    void	setX( double x );
    void	setY( double y );

    double&	rx();
    double&	ry();

    double	manhattanLength() const;

    DwPointF&	operator+=( const DwPointF & point );
    DwPointF&	operator-=( const DwPointF & point );
    DwPointF&	operator*=( double factor );
    DwPointF&	operator/=( double divisor );

    friend bool	operator==( const DwPointF & p1, const DwPointF & p2 );
    friend bool	operator!=( const DwPointF & p1, const DwPointF & p2 );
    friend const DwPointF	operator+( const DwPointF & p1, const DwPointF & p2 );
    friend const DwPointF	operator-( const DwPointF & p1, const DwPointF & p2 );
    friend const DwPointF	operator-( const DwPointF & point );
    friend const DwPointF	operator*( const DwPointF & point, double factor );
    friend const DwPointF	operator*( double factor, const DwPointF & point );
    friend const DwPointF	operator/( const DwPointF & point, double divisor );

    DwPoint toPoint() const;

private:
    double m_x;
    double m_y;
};


/************************************************************************
	DwPointF inline fuctions
************************************************************************/


inline DwPointF::DwPointF()
    : m_x(0.0), m_y(0.0)
{
}

inline DwPointF::DwPointF(const DwPoint &p)
    : m_x(p.x()), m_y(p.y())
{
}

inline DwPointF::DwPointF( double x, double y )
    : m_x(x), m_y(y)
{
}

inline bool DwPointF::isNull() const
{
    return dwIsNull(m_x) && dwIsNull(m_y) ;
}

inline double DwPointF::x() const
{
    return m_x;
}

inline double DwPointF::y() const
{
    return m_y;
}

inline void DwPointF::setX( double x )
{
    m_x = x;
}

inline void DwPointF::setY( double y )
{
    m_y = y;
}

inline double& DwPointF::rx()
{
    return m_x;
}

inline double& DwPointF::ry()
{
    return m_y;
}

inline double DwPointF::manhattanLength() const
{
    return dwAbs(m_x) + dwAbs(m_y);
}

inline bool	operator==( const DwPointF & p1, const DwPointF & p2 )
{
    return dwFuzzyIsNull(p1.m_x - p2.m_x ) && dwFuzzyIsNull(p1.m_y - p2.m_y);
}

inline bool	operator!=( const DwPointF & p1, const DwPointF & p2 )
{
    return !dwFuzzyIsNull(p1.m_x - p2.m_x ) || !dwFuzzyIsNull(p1.m_y - p2.m_y);
}

inline const DwPointF operator+( const DwPointF & p1, const DwPointF & p2 )
{
    return DwPointF(p1.m_x, p1.m_y) += p2;
}

inline const DwPointF operator-( const DwPointF & p1, const DwPointF & p2 )
{
    return DwPointF(p1.m_x, p1.m_y) -= p2;
}

inline const DwPointF operator-( const DwPointF & point )
{
    return DwPointF(- point.m_x, - point.m_y);
}

inline const DwPointF operator*( double factor, const DwPointF & point )
{
    return DwPointF(point.m_x, point.m_y) *= factor;
}

inline const DwPointF operator*( const DwPointF & point, double factor )
{
    return DwPointF(point.m_x, point.m_y) *= factor;
}

inline const DwPointF operator/( const DwPointF & point, double divisor )
{
    return DwPointF(point.m_x, point.m_y) /= divisor;
}

inline DwPoint DwPointF::toPoint() const
{
    return DwPoint(dwRound(m_x), dwRound(m_y));
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwDebug & DwDebug::operator<< <DwPoint>(const DwPoint &t)
{
    m_stream << "( " << t.x() << ", " << t.y() << ")" << KSpace;
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwDebug & DwDebug::operator<< <DwPointF>(const DwPointF &t)
{
    m_stream << "( " << t.x() << ", " << t.y() << ")" << KSpace;
    return *this;
}