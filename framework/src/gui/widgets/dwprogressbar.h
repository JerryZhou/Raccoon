#pragma once
#include "dwgui/dwwidgetitem.h"
#include "dwgraphics/dwcolor.h"
#include "dwcore/dwmargins.h"

//------------------------------------------------------------------------------
class DwGraphicsFrameItem;
class DwGraphicsRectItem;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwProgressBar : public DwWidgetItem
{
    DW_DECLARE_CLASS(DwProgressBar);
public:
    explicit DwProgressBar(DwGraphicsItem *parent = NULL);
    virtual ~DwProgressBar();

    // values
    inline DwReal min() const;
    inline void setMin(DwReal v);
    inline DwReal max() const;
    inline void setMax(DwReal v);
    inline void setRange(DwReal l, DwReal r);
    inline DwReal cur() const;
    inline void setCur(DwReal v);

    // margins
    inline DwMargins margins() const;
    inline void setMargins(DwMargins m);
    inline void setMargins(int l, int t, int r, int b);

    // color stuffs
    inline const DwColor& colorOf(DwWidget::ProgressPhase phase) const;
    inline void setColor(DwWidget::ProgressPhase phase, const DwColor& c);
    inline void setPhase(DwWidget::ProgressPhase phase, DwReal v);

    // flow
    inline DwWidget::Flow flow() const;
    inline void setFlow(DwWidget::Flow f);

    // background
    void setBackground(const DwString& );

    // implement the interface
    virtual DwVariant itemChange(Dw::GraphicsItemChange change, const DwVariant &val);

    virtual DwString toolTips() const;

protected:
    void updateBar();
    DwColor curColor() const;

    DwReal m_min;
    DwReal m_max;
    DwReal m_cur;

    DwReal m_phases[DwWidget::PhaseMax];
    DwColor m_colors[DwWidget::PhaseMax+1];
    DwGraphicsRectItem *m_rect;
    DwMargins m_margins;
    DwWidget::Flow m_flow;
private:

#if DW_OPEN_WIDGET_PROPERTY
    DW_WIDGET_ROOT_PROPERTY_BEGIN(DwProgressBar);
    DW_WIDGET_PROPERTY_NC(cur, DwReal, setCur, cur);
    DW_WIDGET_PROPERTY_END();
#endif
};// end of DwProgressBar
DW_REGISTER_CLASS(DwProgressBar);


//------------------------------------------------------------------------------
/**
*/
inline DwReal DwProgressBar::min() const
{
    return m_min;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwProgressBar::setMin(DwReal v)
{
    m_min = v;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwProgressBar::max() const
{
    return m_max;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwProgressBar::setMax(DwReal v)
{
    m_max = v;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwProgressBar::setRange(DwReal l, DwReal r)
{
    setMin(l);
    setMax(r);
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwProgressBar::cur() const
{
    return m_cur;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwProgressBar::setCur(DwReal v)
{
    if(v > max()) v = max();
    if(v < min()) v = min();
    if (m_cur != v)
    {
        m_cur = v;
        updateBar();
    }
}

//------------------------------------------------------------------------------
/**
*/
inline DwMargins DwProgressBar::margins() const
{
    return m_margins;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwProgressBar::setMargins(DwMargins m)
{
    m_margins = m;
    updateBar();
}

//------------------------------------------------------------------------------
/**
*/
inline void DwProgressBar::setMargins(int l, int t, int r, int b)
{
    setMargins(DwMargins(l, t, r, b));
}

//------------------------------------------------------------------------------
/**
*/
inline const DwColor& DwProgressBar::colorOf(DwWidget::ProgressPhase phase) const
{
    return m_colors[phase];
}

//------------------------------------------------------------------------------
/**
*/
inline void DwProgressBar::setColor(DwWidget::ProgressPhase phase, const DwColor& c)
{
    m_colors[phase] = c;
    updateBar();
}

//------------------------------------------------------------------------------
/**
*/
inline void DwProgressBar::setPhase(DwWidget::ProgressPhase phase, DwReal v)
{
    m_phases[phase] = v;
    updateBar();
}

//------------------------------------------------------------------------------
/**
*/
inline DwWidget::Flow DwProgressBar::flow() const
{
    return m_flow;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwProgressBar::setFlow(DwWidget::Flow f)
{
    m_flow = f;
    updateBar();
}