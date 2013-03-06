#pragma once
#include "dwgui/dwwidgetitem.h"
#include "dwgui/dwgraphicsframeitem.h"

//------------------------------------------------------------------------------
class DwScrollArea;
class DwPushButton;
class DwScrollBarSlots;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwScrollBar : public DwWidgetItem
{
    DW_DECLARE_CLASS(DwScrollBar);

public:
    explicit DwScrollBar(DwGraphicsItem *parent = NULL);
    virtual ~DwScrollBar();

    void setArea(DwScrollArea*);
    inline DwScrollArea* area() const;
    inline DwPushButton* minHit() const;
    inline DwPushButton* maxHit() const;
    inline DwPushButton* thumb() const;

    void setFlow(DwWidget::Flow f);
    inline DwWidget::Flow flow() const;

    void setRange(DwReal , DwReal , DwReal );
    inline void setCur(DwReal c);
    inline DwReal cur() const;
    inline DwReal step() const;
    inline DwReal min() const;
    inline DwReal max() const;

    void scroll(DwReal delta);
    void scrollUp(DwReal delta);
    void scrollDown(DwReal delta);

    DwReal piexl2Range(int delta);

    void updateState();

protected:
    friend class DwScrollBarSlots;

    void updateThumb();
    void updateArea();
    DwReal thumbRange();
    DwReal thumbStartPoint();

    DwReal m_min;
    DwReal m_max;
    DwReal m_cur;
    DwReal m_step;
    DwWidget::Flow m_flow;

    DwPushButton *m_minHit;
    DwPushButton *m_maxHit;
    DwPushButton *m_thumb;

    DwRttiObjectPtr<DwScrollArea> m_scrollArea;

    DwScrollBarSlots *m_slots;
private:

    DW_WIDGET_PROPERTY_BEGIN(DwScrollBar, DwWidgetItem);
    DW_WIDGET_PROPERTY_NC(cur, DwReal, setCur, cur);
    DW_WIDGET_PROPERTY_END();
};// end of DwScrollBar
DW_REGISTER_CLASS(DwScrollBar);

//------------------------------------------------------------------------------
/**
*/
inline DwScrollArea* DwScrollBar::area() const
{
    return m_scrollArea;
}

//------------------------------------------------------------------------------
/**
*/
inline DwPushButton* DwScrollBar::minHit() const
{
    return m_minHit;
}

//------------------------------------------------------------------------------
/**
*/
inline DwPushButton* DwScrollBar::maxHit() const
{
    return m_maxHit;
}

//------------------------------------------------------------------------------
/**
*/
inline DwPushButton* DwScrollBar::thumb() const
{
    return m_thumb;
}

//------------------------------------------------------------------------------
/**
*/
inline DwWidget::Flow DwScrollBar::flow() const
{
    return m_flow;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwScrollBar::setCur(DwReal c)
{
    m_cur = c;
    updateThumb();
    updateArea();
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwScrollBar::cur() const
{
    return m_cur;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwScrollBar::step() const
{
    return m_step;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwScrollBar::min() const
{
    return m_min;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwScrollBar::max() const
{
    return m_max;
}