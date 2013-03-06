#pragma once

#include "dwpoint.h"
#include "dwsize.h"
#include "dwcore/dwdebug.h"

/************************************************************************
	DwRect
************************************************************************/


class DW_CORE_EXPORT DwRect
{
public:
    DwRect();
    DwRect(const DwPoint &topleft, const DwPoint &bottomright);
    DwRect(const DwPoint &topleft, const DwSize &size);
    DwRect(int left, int top, int width, int height);

    bool isNull() const;
    bool isEmpty() const;
    bool isValid() const;

    int x() const;
    int y() const;
    int left() const;
    int top() const;
    int right() const;
    int bottom() const;
    DwRect normalized() const;

    void setX(int x);
    void setY(int y);
    void setLeft(int pos);
    void setTop(int pos);
    void setRight(int pos);
    void setBottom(int pos);

    int width() const;
    int height() const;
    DwSize size() const;

    void setWidth(int w);
    void setHeight(int h);
    void setSize(const DwSize &s);

    DwPoint topLeft() const;
    DwPoint bottomRight() const;
    DwPoint topRight() const;
    DwPoint bottomLeft() const;
    DwPoint center() const;

    void setTopLeft(const DwPoint &p);
    void setBottomRight(const DwPoint &p);
    void setTopRight(const DwPoint &p);
    void setBottomLeft(const DwPoint &p);

    void moveTo(int x, int y);
    void moveTo(const DwPoint &p);
    void moveLeft(int pos);
    void moveTop(int pos);
    void moveRight(int pos);
    void moveBottom(int pos);
    void moveTopLeft(const DwPoint &p);
    void moveBottomRight(const DwPoint &p);
    void moveTopRight(const DwPoint &p);
    void moveBottomLeft(const DwPoint &p);
    void moveCenter(const DwPoint &p);

    void translate(int x, int y);
    void translate(const DwPoint &p);
    DwRect translated(int x, int y) const;
    DwRect translated(const DwPoint &p) const;

    void adjust(int x1, int y1, int x2, int y2);
    DwRect adjusted(int x1, int y1, int x2, int y2) const;

    void setRect(int x, int y, int w, int h);
    void getRect(int *x, int *y, int *w, int *h) const;

    void setCoords(int x1, int y1, int x2, int y2);
    void getCoords(int *x1, int *y1, int *x2, int *y2) const;

    bool contains(int x, int y) const;
    bool contains(int x, int y, bool proper) const;
    bool contains(const DwPoint &p, bool proper = false) const;
    bool contains(const DwRect &r, bool proper = false) const;

    //DwRect unite(const DwRect &r) const;
    DwRect united(const DwRect &r) const;
    //DwRect intersect(const DwRect &r) const;
    DwRect intersected(const DwRect &r) const;
    bool intersects(const DwRect &r) const;

    DwRect operator|(const DwRect &r) const;
    DwRect operator&(const DwRect &r) const;
    DwRect& operator|=(const DwRect &r);
    DwRect& operator&=(const DwRect &r);

    friend bool operator==(const DwRect &r1, const DwRect &r2);
    friend bool operator!=(const DwRect &r1, const DwRect &r2);

private:
    int m_x1;
    int m_y1;
    int m_x2;
    int m_y2;
};

/************************************************************************
	DwSize inline functions
************************************************************************/

inline DwRect::DwRect()
    : m_x1(0), m_y1(0), m_x2(-1), m_y2(-1)
{}

inline DwRect::DwRect(const DwPoint &topleft, const DwPoint &bottomright)
    : m_x1(topleft.x()), m_y1(topleft.y()), m_x2(bottomright.x()), m_y2(bottomright.y())
{}

inline DwRect::DwRect(const DwPoint &topleft, const DwSize &size)
{
    m_x1 = topleft.x();
    m_y1 = topleft.y();
    m_x2 = m_x1 + size.width() - 1;
    m_y2 = m_y1 + size.height() - 1;
}

inline DwRect::DwRect(int left, int top, int width, int height)
    : m_x1(left), m_y1(top), m_x2(left + width - 1), m_y2(top + height - 1)
{}

inline bool DwRect::isNull() const
{
    // both the width and the height is 0
    return m_x2 == m_x1 - 1 && m_y2 == m_y1 - 1;
}

inline bool DwRect::isEmpty() const
{
    return m_x1 > m_x2 || m_y1 > m_y2;
}

inline bool DwRect::isValid() const
{
    // == !isEmpty()
    return m_x1 <= m_x2 && m_y1 <= m_y2;;
}

inline int DwRect::x() const
{
    return m_x1;
}

inline int DwRect::y() const
{
    return m_y1;
}

inline int DwRect::left() const
{
    return m_x1;
}

inline int DwRect::top() const
{
    return m_y1;
}

inline int DwRect::right() const
{
    return m_x2;
}

inline int DwRect::bottom() const
{
    return m_y2;
}

inline void DwRect::setX(int x)
{
    m_x1 = x;
}

inline void DwRect::setY(int y)
{
    m_y1 = y;
}

inline void DwRect::setLeft(int pos)
{
    m_x1 = pos;
}

inline void DwRect::setTop(int pos)
{
    m_y1 = pos;
}

inline void DwRect::setRight(int pos)
{
    m_x2 = pos;
}

inline void DwRect::setBottom(int pos)
{
    m_y2 = pos;
}

inline int DwRect::width() const
{
    return m_x2 - m_x1 + 1;
}

inline int DwRect::height() const
{
    return m_y2 - m_y1 + 1;
}

inline DwSize DwRect::size() const
{
    return DwSize(width(), height());
}

inline void DwRect::setWidth(int w)
{
    m_x2 = m_x1 + w - 1;
}

inline void DwRect::setHeight(int h)
{
    m_y2 = m_y1 + h - 1;
}

inline void DwRect::setSize(const DwSize &s)
{
    setWidth(s.width());
    setHeight(s.height());
}

inline DwPoint DwRect::topLeft() const
{
    return DwPoint(m_x1, m_y1);
}

inline DwPoint DwRect::bottomRight() const
{
    return DwPoint(m_x2, m_y2);
}

inline DwPoint DwRect::topRight() const
{
    return DwPoint(m_x2, m_y1);
}

inline DwPoint DwRect::bottomLeft() const
{
    return DwPoint(m_x1, m_y2);
}

inline DwPoint DwRect::center() const
{
    return DwPoint((m_x1 + m_x2) / 2, (m_y1 + m_y2)/2);
}

inline void DwRect::setTopLeft(const DwPoint &p)
{
    setLeft(p.x());
    setTop(p.y());
}

inline void DwRect::setBottomRight(const DwPoint &p)
{
    setRight(p.x());
    setBottom(p.y());
}

inline void DwRect::setTopRight(const DwPoint &p)
{
    setRight(p.x());
    setTop(p.y());
}

inline void DwRect::setBottomLeft(const DwPoint &p)
{
    setLeft(p.x());
    setBottom(p.y());
}

inline void DwRect::moveTo(int x, int y)
{
    // calc x2/y2's offset
    m_x2 += x - m_x1;
    m_y2 += y - m_y1;
    m_x1 = x;
    m_y1 = y;
}

inline void DwRect::moveTo(const DwPoint &p)
{
    moveTo(p.x(), p.y());
}

inline void DwRect::moveLeft(int pos)
{
    m_x2 += pos - m_x1;
    m_x1 = pos;
}

inline void DwRect::moveTop(int pos)
{
    m_y2 += pos - m_y1;
    m_y1 = pos;
}

inline void DwRect::moveRight(int pos)
{
    m_x1 += pos - m_x2;
    m_x2 = pos;
}

inline void DwRect::moveBottom(int pos)
{
    m_y1 += pos - m_y2;
    m_y2 = pos;
}

inline void DwRect::moveTopLeft(const DwPoint &p)
{
    moveLeft(p.x());
    moveTop(p.y());
}

inline void DwRect::moveBottomRight(const DwPoint &p)
{
    moveRight(p.x());
    moveBottom(p.y());
}

inline void DwRect::moveTopRight(const DwPoint &p)
{
    moveRight(p.x());
    moveTop(p.y());
}

inline void DwRect::moveBottomLeft(const DwPoint &p)
{
    moveLeft(p.x());
    moveBottom(p.y());
}

inline void DwRect::translate(int x, int y)
{
    m_x1 += x;
    m_y1 += y;
    m_x2 += x;
    m_y2 += y;
}

inline void DwRect::translate(const DwPoint &p)
{
    translate(p.x(), p.y());
}

inline DwRect DwRect::translated(int x, int y) const
{
    return DwRect(DwPoint(m_x1 + x, m_y1 + y), DwPoint(m_x2 + x, m_y2 + y));
}

inline DwRect DwRect::translated(const DwPoint &p) const
{
    return translated(p.x(), p.y());
}

inline void DwRect::adjust(int x1, int y1, int x2, int y2)
{
    m_x1 += x1;
    m_y1 += y1;
    m_x2 += x2;
    m_y2 += y2;
}

inline DwRect DwRect::adjusted(int x1, int y1, int x2, int y2) const
{
    return DwRect(DwPoint(m_x1 + x1, m_y1 + y1), DwPoint(m_x2 + x2, m_y2 + y2));
}

inline void DwRect::getRect(int *x, int *y, int *w, int *h) const
{
    *x = m_x1;
    *y = m_y1;
    *w = m_x2 - m_x1 + 1;
    *h = m_y2 - m_y1 + 1;
}

inline void DwRect::setRect(int x, int y, int w, int h)
{
    m_x1 = x;
    m_y1 = y;
    m_x2 = x + w - 1;
    m_y2 = y + h - 1;
}

inline void DwRect::setCoords(int x1, int y1, int x2, int y2)
{
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
}

inline void DwRect::getCoords(int *x1, int *y1, int *x2, int *y2) const
{
    *x1 = m_x1;
    *y1 = m_y1;
    *x2 = m_x2;
    *y2 = m_y2;
}

inline bool DwRect::contains(int x, int y) const
{
    return contains(x, y, false);
}

inline bool DwRect::contains(const DwPoint &p, bool proper /*= false*/) const
{
    return contains(p.x(), p.y(), proper);
}

//inline DwRect DwRect::unite(const DwRect &r) const
//{	return *this | r;	}

inline DwRect DwRect::united(const DwRect &r) const
{
    return *this | r;
}

//inline DwRect DwRect::intersect(const DwRect &r) const
//{	return *this & r;}

inline DwRect DwRect::intersected(const DwRect &r) const
{
    return *this & r;
}

inline bool DwRect::intersects(const DwRect &r) const
{
    return !(intersected(r).isNull());
}

inline DwRect& DwRect::operator|=(const DwRect &r)
{
    *this = *this | r;
    return *this;
}

inline DwRect& DwRect::operator&=(const DwRect &r)
{
    *this = *this & r;
    return *this;
}

inline bool operator==(const DwRect &r1, const DwRect &r2)
{
    return ( r1.m_x1 == r2.m_x1 && r1.m_x2 == r2.m_x2 )
           && ( r1.m_y1 == r2.m_y1 && r1.m_y2 == r2.m_y2 );
}

inline bool operator!=(const DwRect &r1, const DwRect &r2)
{
    return ( r1.m_x1 != r2.m_x1 || r1.m_x2 != r2.m_x2 )
           || ( r1.m_y1 != r2.m_y1 || r1.m_y2 != r2.m_y2 );
}


/************************************************************************
	DwRectF
************************************************************************/


class DW_CORE_EXPORT DwRectF
{
public:
    DwRectF();
    DwRectF(const DwPointF &topleft, const DwPointF &bottomright);
    DwRectF(const DwPointF &topleft, const DwSizeF &size);
    DwRectF(double left, double top, double width, double height);
    DwRectF(const DwRect &r);

    bool isNull() const;
    bool isEmpty() const;
    bool isValid() const;

    double x() const;
    double y() const;
    double left() const;
    double top() const;
    double right() const;
    double bottom() const;
    DwRectF normalized() const;

    void setX(double x);
    void setY(double y);
    void setLeft(double pos);
    void setTop(double pos);
    void setRight(double pos);
    void setBottom(double pos);

    double width() const;
    double height() const;
    DwSizeF size() const;

    void setWidth(double w);
    void setHeight(double h);
    void setSize(const DwSizeF &s);

    DwPointF topLeft() const;
    DwPointF bottomRight() const;
    DwPointF topRight() const;
    DwPointF bottomLeft() const;
    DwPointF center() const;

    void setTopLeft(const DwPointF &p);
    void setBottomRight(const DwPointF &p);
    void setTopRight(const DwPointF &p);
    void setBottomLeft(const DwPointF &p);

    void moveTo(double x, double y);
    void moveTo(const DwPointF &p);
    void moveLeft(double pos);
    void moveTop(double pos);
    void moveRight(double pos);
    void moveBottom(double pos);
    void moveTopLeft(const DwPointF &p);
    void moveBottomRight(const DwPointF &p);
    void moveTopRight(const DwPointF &p);
    void moveBottomLeft(const DwPointF &p);
    void moveCenter(const DwPointF &p);

    void translate(double x, double y);
    void translate(const DwPointF &p);
    DwRectF translated(double x, double y) const;
    DwRectF translated(const DwPointF &p) const;

    void adjust(double x1, double y1, double x2, double y2);
    DwRectF adjusted(double x1, double y1, double x2, double y2) const;

    void setRect(double x, double y, double w, double h);
    void getRect(double *x, double *y, double *w, double *h) const;

    void setCoords(double x1, double y1, double x2, double y2);
    void getCoords(double *x1, double *y1, double *x2, double *y2) const;

    bool contains(double x, double y) const;
    bool contains(const DwPointF &p) const;
    bool contains(const DwRectF &r) const;

    DwRectF united(const DwRectF &r) const;
    DwRectF intersected(const DwRectF &r) const;
    bool intersects(const DwRectF &r) const;

    DwRect toRect() const;
    DwRect toAlignedRect () const;

    DwRectF operator|(const DwRectF &r) const;
    DwRectF operator&(const DwRectF &r) const;
    DwRectF& operator|=(const DwRectF &r);
    DwRectF& operator&=(const DwRectF &r);

    friend bool operator==(const DwRectF &r1, const DwRectF &r2);
    friend bool operator!=(const DwRectF &r1, const DwRectF &r2);

private:
    double m_xp;
    double m_yp;
    double m_wd;
    double m_ht;
};

/************************************************************************
	DwSizeF inline functions
************************************************************************/

inline DwRectF::DwRectF()
    : m_xp(0.0), m_yp(0.0), m_wd(0.0), m_ht(0.0)
{}

inline DwRectF::DwRectF(const DwRect &r)
    : m_xp(r.x()), m_yp(r.y()), m_wd(r.width()), m_ht(r.height())
{
}

inline DwRectF::DwRectF(const DwPointF &topleft, const DwPointF &bottomright)
{
    m_xp = topleft.x();
    m_yp = topleft.y();
    m_wd = bottomright.x() - m_xp;
    m_ht = bottomright.y() - m_yp;
}

inline DwRectF::DwRectF(const DwPointF &topleft, const DwSizeF &size)
    : m_xp(topleft.x()), m_yp(topleft.y()), m_wd(size.width()), m_ht(size.height())
{}

inline DwRectF::DwRectF(double left, double top, double width, double height)
    : m_xp(left), m_yp(top), m_wd(width), m_ht( height)
{}

inline bool DwRectF::isNull() const
{
    // both the width and the height is 0
    return dwFuzzyIsNull(m_wd) && dwFuzzyIsNull(m_ht);
}

inline bool DwRectF::isEmpty() const
{
    return m_wd <= 0.0 || m_ht <= 0.0;
}

inline bool DwRectF::isValid() const
{
    // == !isEmpty()
    return m_wd > 0.0 && m_ht > 0.0;;
}

inline double DwRectF::x() const
{
    return m_xp;
}

inline double DwRectF::y() const
{
    return m_yp;
}

inline double DwRectF::left() const
{
    return m_xp;
}

inline double DwRectF::top() const
{
    return m_yp;
}

inline double DwRectF::right() const
{
    return m_xp + m_wd;
}

inline double DwRectF::bottom() const
{
    return m_yp + m_ht;
}

inline void DwRectF::setX(double x)
{
    double offset = x - m_xp;
    m_xp += offset;
    m_wd -= offset;
}

inline void DwRectF::setY(double y)
{
    double offset = y - m_yp;
    m_yp += offset;
    m_ht -= offset;
}

inline void DwRectF::setLeft(double pos)
{
    setX(pos);
}

inline void DwRectF::setTop(double pos)
{
    setY(pos);
}

inline void DwRectF::setRight(double pos)
{
    m_wd = pos - m_xp;
}

inline void DwRectF::setBottom(double pos)
{
    m_ht = pos - m_yp;
}

inline double DwRectF::width() const
{
    return m_wd;
}

inline double DwRectF::height() const
{
    return m_ht;
}

inline DwSizeF DwRectF::size() const
{
    return DwSizeF(width(), height());
}

inline void DwRectF::setWidth(double w)
{
    m_wd = w;
}

inline void DwRectF::setHeight(double h)
{
    m_ht = h;
}

inline void DwRectF::setSize(const DwSizeF &s)
{
    setWidth(s.width());
    setHeight(s.height());
}

inline DwPointF DwRectF::topLeft() const
{
    return DwPointF(m_xp, m_yp);
}

inline DwPointF DwRectF::bottomRight() const
{
    return DwPointF(m_xp + m_wd, m_yp + m_ht);
}

inline DwPointF DwRectF::topRight() const
{
    return DwPointF(m_xp + m_wd, m_yp);
}

inline DwPointF DwRectF::bottomLeft() const
{
    return DwPointF(m_xp, m_yp + m_ht);
}

inline DwPointF DwRectF::center() const
{
    return DwPointF(m_xp + m_wd / 2, m_yp + m_ht / 2);
}

inline void DwRectF::setTopLeft(const DwPointF &p)
{
    setLeft(p.x());
    setTop(p.y());
}

inline void DwRectF::setBottomRight(const DwPointF &p)
{
    setRight(p.x());
    setBottom(p.y());
}

inline void DwRectF::setTopRight(const DwPointF &p)
{
    setRight(p.x());
    setTop(p.y());
}

inline void DwRectF::setBottomLeft(const DwPointF &p)
{
    setLeft(p.x());
    setBottom(p.y());
}

inline void DwRectF::moveTo(double x, double y)
{
    m_xp = x;
    m_yp = y;
}

inline void DwRectF::moveTo(const DwPointF &p)
{
    moveTo(p.x(), p.y());
}

inline void DwRectF::moveLeft(double pos)
{
    m_xp = pos;
}

inline void DwRectF::moveTop(double pos)
{
    m_yp = pos;
}

inline void DwRectF::moveRight(double pos)
{
    m_xp = pos - m_wd;
}

inline void DwRectF::moveBottom(double pos)
{
    m_yp = pos - m_ht;
}

inline void DwRectF::moveTopLeft(const DwPointF &p)
{
    moveLeft(p.x());
    moveTop(p.y());
}

inline void DwRectF::moveBottomRight(const DwPointF &p)
{
    moveRight(p.x());
    moveBottom(p.y());
}

inline void DwRectF::moveTopRight(const DwPointF &p)
{
    moveRight(p.x());
    moveTop(p.y());
}

inline void DwRectF::moveBottomLeft(const DwPointF &p)
{
    moveLeft(p.x());
    moveBottom(p.y());
}

inline void DwRectF::moveCenter( const DwPointF &p )
{
    m_xp = p.x() - m_wd / 2;
    m_yp = p.y() - m_ht / 2;
}

inline void DwRectF::translate(double x, double y)
{
    m_xp += x;
    m_yp += y;
}

inline void DwRectF::translate(const DwPointF &p)
{
    translate(p.x(), p.y());
}

inline DwRectF DwRectF::translated(double x, double y) const
{
    return DwRectF(m_xp + x, m_yp + y, m_wd, m_ht);
}

inline DwRectF DwRectF::translated(const DwPointF &p) const
{
    return translated(p.x(), p.y());
}

inline void DwRectF::adjust(double x1, double y1, double x2, double y2)
{
    m_xp += x1;
    m_yp += y1;
    m_wd += x2 - x1;
    m_ht += y2 - y1;
}

inline DwRectF DwRectF::adjusted(double x1, double y1, double x2, double y2) const
{
    return DwRectF(DwPointF(m_xp + x1, m_yp + y1), DwSizeF(m_wd + x2 - x1, m_ht + y2 - y1));
}

inline void DwRectF::getRect(double *x, double *y, double *w, double *h) const
{
    *x = m_xp;
    *y = m_yp;
    *w = m_wd;
    *h = m_ht;
}

inline void DwRectF::setRect(double x, double y, double w, double h)
{
    m_xp = x;
    m_yp = y;
    m_wd = w;
    m_ht = h;
}

inline void DwRectF::setCoords(double x1, double y1, double x2, double y2)
{
    m_xp = x1;
    m_yp = y1;
    m_wd = x2 - x1;
    m_ht = y2 - y1;
}

inline void DwRectF::getCoords(double *x1, double *y1, double *x2, double *y2) const
{
    *x1 = m_xp;
    *y1 = m_yp;
    *x2 = m_xp + m_wd;
    *y2 = m_yp + m_ht;
}

inline bool DwRectF::contains(const DwPointF &p) const
{
    return contains(p.x(), p.y());
}

inline DwRectF DwRectF::united(const DwRectF &r) const
{
    return *this | r;
}

inline DwRectF DwRectF::intersected(const DwRectF &r) const
{
    return *this & r;
}

inline bool DwRectF::intersects(const DwRectF &r) const
{
    return !(intersected(r).isNull());
}

inline DwRect DwRectF::toRect() const
{
    return DwRect(dwRound(m_xp), dwRound(m_yp), dwRound(m_wd), dwRound(m_ht));
}

inline DwRectF& DwRectF::operator|=(const DwRectF &r)
{
    *this = *this | r;
    return *this;
}

inline DwRectF& DwRectF::operator&=(const DwRectF &r)
{
    *this = *this & r;
    return *this;
}

inline bool operator==(const DwRectF &r1, const DwRectF &r2)
{
    return ( dwFuzzyIsNull(r1.m_xp - r2.m_xp)
             && dwFuzzyIsNull(r1.m_wd - r2.m_wd)
             && dwFuzzyIsNull(r1.m_yp - r2.m_yp)
             && dwFuzzyIsNull(r1.m_ht - r2.m_ht)
           );
}

inline bool operator!=(const DwRectF &r1, const DwRectF &r2)
{
    return ( !dwFuzzyIsNull(r1.m_xp - r2.m_xp)
             || !dwFuzzyIsNull(r1.m_wd - r2.m_wd)
             || !dwFuzzyIsNull(r1.m_yp - r2.m_yp)
             || !dwFuzzyIsNull(r1.m_ht - r2.m_ht)
           );

}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwDebug & DwDebug::operator<< <DwRect>(const DwRect &t)
{
    m_stream << "( " << t.left() << ", " << t.top() << ", " << t.right() << ", " << t.bottom() << ")" << KSpace;
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwDebug & DwDebug::operator<< <DwRectF>(const DwRectF &t)
{
    m_stream << "( " << t.left() << ", " << t.top() << ", " << t.right() << ", " << t.bottom() << ")" << KSpace;
    return *this;
}


