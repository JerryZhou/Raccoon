#pragma once

#include "dwnamespace.h"
#include "../dwcore.h"
#include "dwcore/dwdebug.h"

/************************************************************************
	DwSize
************************************************************************/

class DW_CORE_EXPORT DwSize
{
public:
    DwSize();
    DwSize(int width, int height);

    bool isNull() const;
    bool isEmpty() const;
    bool isValid() const;

    int width() const;
    int height() const;
    void setWidth(int w);
    void setHeight(int h);
    void transpose();

    void scale(int w, int h, Dw::AspectRatioMode mode);
    void scale(const DwSize &s, Dw::AspectRatioMode mode);

    DwSize expandedTo(const DwSize &s) const;
    DwSize boundedTo(const DwSize &s) const;

    int &rwidth();
    int &rheight();

    DwSize &operator+=(const DwSize &s);
    DwSize &operator-=(const DwSize &s);
    DwSize &operator*=(double factor);
    DwSize &operator/=(double divisor);

    friend bool operator==(const DwSize &s1, const DwSize &s2);
    friend bool operator!=(const DwSize &s1, const DwSize &s2);
    friend const DwSize operator+(const DwSize &s1, const DwSize &s2);
    friend const DwSize operator-(const DwSize &s1, const DwSize &s2);
    friend const DwSize operator*(const DwSize &s, double factor);
    friend const DwSize operator*(double factor, const DwSize &s);
    friend const DwSize operator/(const DwSize &s, double divisor);

private:
    int m_wd;
    int m_ht;
};


/************************************************************************
	DwSize inline fuctions
************************************************************************/



inline DwSize::DwSize()
    : m_wd(0), m_ht(0)
{}

inline DwSize::DwSize(int width, int height)
    : m_wd(width), m_ht(height)
{}

inline bool DwSize::isNull() const
{
    return m_wd == 0 && m_ht == 0;
}

inline bool DwSize::isEmpty() const
{
    return m_wd <= 0 || m_ht <= 0;
}

inline bool DwSize::isValid() const
{
    return m_wd >= 0 && m_ht >= 0;
}

inline int DwSize::width() const
{
    return m_wd;
}

inline int DwSize::height() const
{
    return m_ht;
}

inline void DwSize::setWidth(int w)
{
    m_wd = w;
}

inline void DwSize::setHeight(int h)
{
    m_ht = h;
}

inline void DwSize::transpose()
{
    dwSwap(m_wd, m_ht);
}

inline int & DwSize::rwidth()
{
    return m_wd;
}

inline int & DwSize::rheight()
{
    return m_ht;
}

inline void DwSize::scale(const DwSize &s, Dw::AspectRatioMode mode)
{
    return scale(s.m_wd, s.m_ht, mode);
}

inline DwSize DwSize::expandedTo( const DwSize & s) const
{
    return DwSize(dwMax(m_wd, s.m_wd), dwMax(m_ht, s.m_ht));
}

inline DwSize DwSize::boundedTo( const DwSize & s) const
{
    return DwSize(dwMin(m_wd, s.m_wd), dwMin(m_ht, s.m_ht));
}

inline bool	operator==( const DwSize & s1, const DwSize & s2 )
{
    return s1.m_wd == s2.m_wd && s1.m_ht == s2.m_ht;
}

inline bool	operator!=( const DwSize & s1, const DwSize & s2 )
{
    return s1.m_wd != s2.m_wd || s1.m_ht != s2.m_ht;
}

inline const DwSize operator+( const DwSize & s1, const DwSize & s2 )
{
    return DwSize(s1.m_wd, s1.m_ht) += s2;
}

inline const DwSize operator-( const DwSize & s1, const DwSize & s2 )
{
    return DwSize(s1.m_wd, s1.m_ht) -= s2;
}

inline const DwSize operator*( double factor, const DwSize & s )
{
    return DwSize(s.m_wd, s.m_ht) *= factor;
}

inline const DwSize operator*( const DwSize & point, double factor )
{
    return DwSize(point.m_wd, point.m_ht) *= factor;
}

inline const DwSize operator/( const DwSize & point, double divisor )
{
    return DwSize(point.m_wd, point.m_ht) /= divisor;
}


/************************************************************************
	DwSizeF
************************************************************************/

class DW_CORE_EXPORT DwSizeF
{
public:
    DwSizeF();
    DwSizeF(double width, double height);
    DwSizeF(const DwSize &s);

    bool isNull() const;
    bool isEmpty() const;
    bool isValid() const;

    double width() const;
    double height() const;
    void setWidth(double w);
    void setHeight(double h);
    void transpose();

    void scale(double w, double h, Dw::AspectRatioMode mode);
    void scale(const DwSizeF &s, Dw::AspectRatioMode mode);

    DwSizeF expandedTo(const DwSizeF &s) const;
    DwSizeF boundedTo(const DwSizeF &s) const;

    double &rwidth();
    double &rheight();

    DwSize toSize() const;

    DwSizeF &operator+=(const DwSizeF &s);
    DwSizeF &operator-=(const DwSizeF &s);
    DwSizeF &operator*=(double factor);
    DwSizeF &operator/=(double divisor);

    friend bool operator==(const DwSizeF &s1, const DwSizeF &s2);
    friend bool operator!=(const DwSizeF &s1, const DwSizeF &s2);
    friend const DwSizeF operator+(const DwSizeF &s1, const DwSizeF &s2);
    friend const DwSizeF operator-(const DwSizeF &s1, const DwSizeF &s2);
    friend const DwSizeF operator*(const DwSizeF &s, double factor);
    friend const DwSizeF operator*(double factor, const DwSizeF &s);
    friend const DwSizeF operator/(const DwSizeF &s, double divisor);

private:
    double m_wd;
    double m_ht;
};


/************************************************************************
	DwSizeF inline fuctions
************************************************************************/



inline DwSizeF::DwSizeF()
    : m_wd(0.0), m_ht(0.0)
{}

inline DwSizeF::DwSizeF(const DwSize &s)
    : m_wd(s.width()), m_ht(s.height())
{}

inline DwSizeF::DwSizeF(double width, double height)
    : m_wd(width), m_ht(height)
{}

inline bool DwSizeF::isNull() const
{
    return dwIsNull(m_wd) && dwIsNull(m_ht);
}

inline bool DwSizeF::isEmpty() const
{
    return m_wd <= 0 || m_ht <= 0;
}

inline bool DwSizeF::isValid() const
{
    return m_wd >= 0 && m_ht >= 0;
}

inline double DwSizeF::width() const
{
    return m_wd;
}

inline double DwSizeF::height() const
{
    return m_ht;
}

inline void DwSizeF::setWidth(double w)
{
    m_wd = w;
}

inline void DwSizeF::setHeight(double h)
{
    m_ht = h;
}

inline void DwSizeF::transpose()
{
    dwSwap(m_wd, m_ht);
}

inline double & DwSizeF::rwidth()
{
    return m_wd;
}

inline double & DwSizeF::rheight()
{
    return m_ht;
}

inline void DwSizeF::scale(const DwSizeF &s, Dw::AspectRatioMode mode)
{
    return scale(s.m_wd, s.m_ht, mode);
}

inline DwSizeF DwSizeF::expandedTo( const DwSizeF & s) const
{
    return DwSizeF(dwMax(m_wd, s.m_wd), dwMax(m_ht, s.m_ht));
}

inline DwSizeF DwSizeF::boundedTo( const DwSizeF & s) const
{
    return DwSizeF(dwMin(m_wd, s.m_wd), dwMin(m_ht, s.m_ht));
}

inline DwSize DwSizeF::toSize() const
{
    return DwSize(dwRound(m_wd), dwRound(m_ht));
}

inline bool	operator==( const DwSizeF & s1, const DwSizeF & s2 )
{
    return dwFuzzyIsNull( s1.m_wd - s2.m_wd) && dwFuzzyIsNull( s1.m_ht - s2.m_ht);
}

inline bool	operator!=( const DwSizeF & s1, const DwSizeF & s2 )
{
    return s1.m_wd != s2.m_wd || s1.m_ht != s2.m_ht;
}

inline const DwSizeF operator+( const DwSizeF & s1, const DwSizeF & s2 )
{
    return DwSizeF(s1.m_wd, s1.m_ht) += s2;
}

inline const DwSizeF operator-( const DwSizeF & s1, const DwSizeF & s2 )
{
    return DwSizeF(s1.m_wd, s1.m_ht) -= s2;
}

inline const DwSizeF operator*( double factor, const DwSizeF & s )
{
    return DwSizeF(s.m_wd, s.m_ht) *= factor;
}

inline const DwSizeF operator*( const DwSizeF & point, double factor )
{
    return DwSizeF(point.m_wd, point.m_ht) *= factor;
}

inline const DwSizeF operator/( const DwSizeF & point, double divisor )
{
    return DwSizeF(point.m_wd, point.m_ht) /= divisor;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwDebug & DwDebug::operator<< <DwSizeF>(const DwSizeF &t)
{
    m_stream << "( " << t.width() << ", " << t.height() << ")" << KSpace;
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwDebug & DwDebug::operator<< <DwSize>(const DwSize &t)
{
    m_stream << "( " << t.width() << ", " << t.height() << ")" << KSpace;
    return *this;
}