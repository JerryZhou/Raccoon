#pragma once

class DW_CORE_EXPORT DwMargins
{
public:
    DwMargins()
        : m_left(0)
        , m_top(0)
        , m_right(0)
        , m_bottom(0)
    {
        ;
    }

    inline DwMargins(int l, int t, int r, int b)
        : m_left(l)
        , m_top(t)
        , m_right(r)
        , m_bottom(b)
    {

    }

    inline int left() const
    {
        return m_left;
    }
    inline int top() const
    {
        return m_top;
    }
    inline int right() const
    {
        return m_right;
    }
    inline int bottom() const
    {
        return m_bottom;
    }
    inline void setLeft(int l)
    {
        m_left = l;
    }
    inline void setTop(int t)
    {
        m_top = t;
    }
    inline void setRight(int r)
    {
        m_right = r;
    }
    inline void setBottom(int b)
    {
        m_bottom = b;
    }
    inline bool isZero() const
    {
        return (m_left == 0 && m_top == 0 
            && m_right == 0 
            && m_bottom == 0);
    }

private:
    int m_left;
    int m_top;
    int m_right;
    int m_bottom;
};

