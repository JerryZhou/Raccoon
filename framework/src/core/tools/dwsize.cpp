#include "stable.h"

#include "dwsize.h"

/************************************************************************
	DwSize
************************************************************************/

void DwSize::scale( int w, int h, Dw::AspectRatioMode mode )
{
    if(mode == Dw::IgnoreAspectRatio || m_wd == 0 || m_ht == 0)
    {
        m_wd = w;
        m_ht = h;
    }
    else
    {
        bool isUseHeight = false;
        int64_t ratioW = (int64_t)((int64_t)m_wd * (int64_t)h * 1.0 / m_ht);

        if (mode == Dw::KeepAspectRatio)
        {
            isUseHeight = (ratioW <= w);
        }
        else	//Dw::KeepAspectRatioByExpanding:
        {
            isUseHeight = (ratioW >= w);
        }

        if (isUseHeight)
        {
            m_wd = (int)ratioW;
            m_ht = h;
        }
        else
        {
            m_ht = (int)((int64_t)w * (int64_t)m_ht * 1.0 / m_wd);
            m_wd = w;
        }
    }
}

DwSize& DwSize::operator+=( const DwSize & s )
{
    m_wd += s.m_wd;
    m_ht += s.m_ht;
    return *this;
}

DwSize& DwSize::operator-=( const DwSize & s )
{
    m_wd -= s.m_wd;
    m_ht -= s.m_ht;
    return *this;
}

DwSize& DwSize::operator*=( double factor )
{
    m_wd = dwRound(m_wd * factor);
    m_ht = dwRound(m_ht * factor);
    return *this;
}

DwSize& DwSize::operator/=( double divisor )
{
    m_wd = dwRound(m_wd / divisor);
    m_ht = dwRound(m_ht / divisor);
    return *this;
}



/************************************************************************
	DwSizeF
************************************************************************/

void DwSizeF::scale( double w, double h, Dw::AspectRatioMode mode )
{
    if(mode == Dw::IgnoreAspectRatio || dwFuzzyIsNull(m_wd) || dwFuzzyIsNull(m_ht))
    {
        m_wd = w;
        m_ht = h;
    }
    else
    {
        bool isUseHeight = false;
        double ratioW = m_wd * h / m_ht;

        if (mode == Dw::KeepAspectRatio)
        {
            isUseHeight = (ratioW <= w);
        }
        else	//Dw::KeepAspectRatioByExpanding:
        {
            isUseHeight = (ratioW >= w);
        }

        if (isUseHeight)
        {
            m_wd = (int)ratioW;
            m_ht = h;
        }
        else
        {
            m_ht = w * m_ht / m_wd;
            m_wd = w;
        }
    }
}

DwSizeF& DwSizeF::operator+=( const DwSizeF & s )
{
    m_wd += s.m_wd;
    m_ht += s.m_ht;
    return *this;
}

DwSizeF& DwSizeF::operator-=( const DwSizeF & s )
{
    m_wd -= s.m_wd;
    m_ht -= s.m_ht;
    return *this;
}

DwSizeF& DwSizeF::operator*=( double factor )
{
    m_wd *= factor;
    m_ht *= factor;
    return *this;
}

DwSizeF& DwSizeF::operator/=( double divisor )
{
    m_wd /= divisor;
    m_ht /= divisor;
    return *this;
}
