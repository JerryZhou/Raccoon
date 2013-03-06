#include "stable.h"

#include "dwrect.h"

/************************************************************************
	DwRect
************************************************************************/

DwRect DwRect::normalized() const
{
    DwRect rc;
    if (m_x2 < m_x1 - 1)
    {
        // swap bad m_x values
        rc.m_x1 = m_x2;
        rc.m_x2 = m_x1;
    }
    else
    {
        rc.m_x1 = m_x1;
        rc.m_x2 = m_x2;
    }
    if (m_y2 < m_y1 - 1)
    {
        // swap bad m_y values
        rc.m_y1 = m_y2;
        rc.m_y2 = m_y1;
    }
    else
    {
        rc.m_y1 = m_y1;
        rc.m_y2 = m_y2;
    }
    return rc;
}

void DwRect::moveCenter( const DwPoint &p )
{
    int w = m_x2 - m_x1;
    int h = m_y2 - m_y1;
    m_x1 = p.x() - w / 2;
    m_y1 = p.y() - h / 2;
    m_x2 = m_x1 + w;
    m_y2 = m_y1 + h;
}

bool DwRect::contains( int x, int y, bool proper) const
{
    int left = m_x1;
    int right = m_x2;
    if (m_x2 < m_x1 - 1)
    {
        left = m_x2;
        right = m_x1;
    }

    // proper is true, not contains the edge
    if (proper)
    {
        if (x <= left || x >= right)
        {
            return false;
        }
    }
    else
    {
        if (x < left || x > right)
        {
            return false;
        }
    }

    int top = m_y1;
    int bottom = m_y2;
    if (m_y2 < m_y1 - 1)
    {
        top = m_y2;
        bottom = m_y1;
    }

    // proper is true, not contains the edge
    if (proper)
    {
        if (y <= top || y >= bottom)
        {
            return false;
        }
    }
    else
    {
        if (y < top || y > bottom)
        {
            return false;
        }
    }
    return true;
}

bool DwRect::contains( const DwRect &r, bool proper /*= false*/ ) const
{
    if (isNull() || r.isNull())
    {
        return false;
    }

    return (contains(r.topLeft(), proper)
            && contains(r.topRight(), proper)
            && contains(r.bottomLeft(), proper)
            && contains(r.bottomRight(), proper)
           );
}

DwRect DwRect::operator|( const DwRect &r ) const
{
    if (isNull())
    {
        return r;
    }
    if (r.isNull())
    {
        return *this;
    }

    int left1 = m_x1;
    int right1 = m_x1;
    if (width() < 0)
    {
        left1 = m_x2;
    }
    else
    {
        right1 = m_x2;
    }

    int left2 = r.m_x1;
    int right2 = r.m_x1;
    if (r.width() < 0)
    {
        left2 = r.m_x2;
    }
    else
    {
        right2 = r.m_x2;
    }

    int top1 = m_y1;
    int bottom1 = m_y1;
    if (height() < 0)
    {
        top1 = m_y2;
    }
    else
    {
        bottom1 = m_y2;
    }

    int top2 = r.m_y1;
    int bottom2 = r.m_y1;
    if (r.height() < 0)
    {
        top2 = r.m_y2;
    }
    else
    {
        bottom2 = r.m_y2;
    }

    DwRect rc;
    rc.m_x1 = dwMin(left1, left2);
    rc.m_x2 = dwMax(right1, right2);
    rc.m_y1 = dwMin(top1, top2);
    rc.m_y2 = dwMax(bottom1, bottom2);

    return rc;
}

DwRect DwRect::operator&( const DwRect &r ) const
{
    if (isNull() || r.isNull())
    {
        return DwRect();
    }

    int left1 = m_x1;
    int right1 = m_x1;
    if (width() < 0)
    {
        left1 = m_x2;
    }
    else
    {
        right1 = m_x2;
    }

    int left2 = r.m_x1;
    int right2 = r.m_x1;
    if (r.width() < 0)
    {
        left2 = r.m_x2;
    }
    else
    {
        right2 = r.m_x2;
    }

    //X axis not intersect
    if (left1 > right2 || left2 > right1)
    {
        return DwRect();
    }

    int top1 = m_y1;
    int bottom1 = m_y1;
    if (height() < 0)
    {
        top1 = m_y2;
    }
    else
    {
        bottom1 = m_y2;
    }

    int top2 = r.m_y1;
    int bottom2 = r.m_y1;
    if (r.height() < 0)
    {
        top2 = r.m_y2;
    }
    else
    {
        bottom2 = r.m_y2;
    }

    //Y axis not intersect
    if (top1 > bottom2 || top2 > bottom1)
    {
        return DwRect();
    }

    DwRect rc;
    rc.m_x1 = dwMax(left1, left2);
    rc.m_x2 = dwMin(right1, right2);
    rc.m_y1 = dwMax(top1, top2);
    rc.m_y2 = dwMin(bottom1, bottom2);

    return rc;
}


/************************************************************************
	DwRectF
************************************************************************/


DwRectF DwRectF::normalized() const
{
    DwRectF rc(*this);
    if (m_wd < 0)
    {
        // swap bad m_x values
        rc.m_xp = right();
        rc.m_wd = - m_wd;
    }

    if (m_ht < 0)
    {
        // swap bad m_y values
        rc.m_yp = bottom();
        rc.m_ht = - m_ht;
    }
    return rc;
}

bool DwRectF::contains( double x, double y) const
{
    double theLeft = left();
    double theRight = right();
    if (theRight < theLeft)
    {
        theLeft = right();
        theRight = left();
    }

    if (x < theLeft || x > theRight)
    {
        return false;
    }

    double theTop = top();
    double theBottom = bottom();
    if (m_ht < m_yp - 1)
    {
        theTop = bottom();
        theBottom = top();
    }

    if (y < theTop || y > theBottom)
    {
        return false;
    }

    return true;
}

bool DwRectF::contains( const DwRectF &r ) const
{
    if (isNull() || r.isNull())
    {
        return false;
    }

    return (contains(r.topLeft())
            && contains(r.topRight())
            && contains(r.bottomLeft())
            && contains(r.bottomRight())
           );
}

DwRectF DwRectF::operator|( const DwRectF &r ) const
{
    if (isNull())
    {
        return r;
    }
    if (r.isNull())
    {
        return *this;
    }

    double left1 = left();
    double right1 = right();
    if (width() < 0)
    {
        dwSwap(left1, right1);
    }

    double left2 = r.left();
    double right2 = r.right();
    if (r.width() < 0)
    {
        dwSwap(left2, right2);
    }

    double top1 = top();
    double bottom1 = bottom();
    if (height() < 0)
    {
        dwSwap(top1, bottom1);
    }

    double top2 = r.top();
    double bottom2 = r.bottom();
    if (r.height() < 0)
    {
        dwSwap(top2, bottom2);
    }

    DwRectF rc;
    rc.setLeft( dwMin(left1, left2) );
    rc.setRight( dwMax(right1, right2) );
    rc.setTop( dwMin(top1, top2) );
    rc.setBottom( dwMax(bottom1, bottom2) );

    return rc;
}

DwRectF DwRectF::operator&( const DwRectF &r ) const
{
    if (isNull() || r.isNull())
    {
        return DwRectF();
    }

    double left1 = left();
    double right1 = right();
    if (width() < 0)
    {
        dwSwap(left1, right1);
    }

    double left2 = r.left();
    double right2 = r.right();
    if (r.width() < 0)
    {
        dwSwap(left2, right2);
    }

    if (left1 > right2 || left2 > right1)
    {
        return DwRectF();
    }

    double top1 = top();
    double bottom1 = bottom();
    if (height() < 0)
    {
        dwSwap(top1, bottom1);
    }

    double top2 = r.top();
    double bottom2 = r.bottom();
    if (r.height() < 0)
    {
        dwSwap(top2, bottom2);
    }

    if (top1 > bottom2 || top2 > bottom1)
    {
        return DwRectF();
    }

    DwRectF rc;
    rc.setLeft( dwMax(left1, left2) );
    rc.setRight( dwMin(right1, right2) );
    rc.setTop( dwMax(top1, top2) );
    rc.setBottom( dwMin(bottom1, bottom2) );

    return rc;
}

//	return a smallest rect that can contains this rectf
DwRect DwRectF::toAlignedRect () const
{
    int xmin = int(dwFloor( left() ));
    int xmax = int(dwCeil( right() ));
    int ymin = int(dwFloor( top() ));
    int ymax = int(dwCeil( bottom() ));
    return DwRect(xmin, ymin, xmax - xmin, ymax - ymin);
}
