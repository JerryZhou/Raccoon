#pragma once
#include "dwgui/dwwidgetenum.h"
#include "dwgui/dwgraphicspixmapitem.h"
#include "dwcore/dwmargins.h"
#include "dwgraphics/dwimage.h"
#include "dwgraphics/dwcolor.h"
#include "dwgui/dwurl.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwgraphicsmarginspixmapitem.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsFrameItem : public DwGraphicsItem
{
    DW_DECLARE_CLASS(DwGraphicsFrameItem);
public:
    explicit DwGraphicsFrameItem(DwGraphicsItem *parent = NULL);
    virtual ~DwGraphicsFrameItem();

    virtual void paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option);// = 0;

    void setDisable(bool b);
    void setHover(bool b);
    void setPressedDown(bool b);
    void setFocus(bool b);

    inline bool isDisable() const;
    inline bool isHover() const;
    inline bool isPressedDown() const;
    inline bool isFocus() const;

    void setMargins(const DwMargins& );
    inline const DwMargins& margins() const;

    void setImage(DwWidget::FrameState state, const DwUrl& image);
    void setImage(DwWidget::FrameState state, const DwImage &img);
    DwImage imageOf(DwWidget::FrameState state) const;

    void setMask(const DwUrl& url);
    DwImage mask() const;

    void setBackgroundColor(const DwColor &c);
    inline const DwColor& backgroundColor() const;

    virtual void onFrameOut();

    void updateFrame();
    void updateFrame(int interval);

    void setKeepUpdate(bool b);
    bool isKeepUpdate() const;

    void setDrawCenter(bool b);
    bool isDrawCenter() const;

    void setAnimationWhen(DwWidget::FrameState state, bool b);
    bool isAnimationWhen(DwWidget::FrameState state)const;

protected:
    friend class DwGraphicsFrameSlots;
    friend class DwGraphicsTimer;

    void changeState(DwWidget::FrameState, bool b);
    void startAnimation(DwWidget::FrameState, bool b);
    void appendAnimation(DwGraphicsItem *item, DwReal start, DwReal end, int duration);
    void updateInner();
    DwImage currentImage() const;
    DwGraphicsMarginsPixmapItem *pixmapOf(DwWidget::FrameState state);

    bool m_isDisable : 1;
    bool m_isHover : 1;
    bool m_isPressedDown : 1;
    bool m_isFocus : 1;
    bool m_needDrawCenter : 1;
    bool m_keepUpdate : 1;
    DwMargins m_margins;
    DwColor m_backgroundColor;

    uint16_t m_animationWhen;
    DwGraphicsMarginsPixmapItem *m_pixmaps[DwWidget::SStateMax+1];

private:
};// end of DwGraphicsFrameItem
DW_REGISTER_CLASS(DwGraphicsFrameItem);

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsFrameItem::isDisable() const
{
    return m_isDisable;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsFrameItem::isHover() const
{
    return m_isHover;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsFrameItem::isPressedDown() const
{
    return m_isPressedDown;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsFrameItem::isFocus() const
{
    return m_isFocus;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwMargins& DwGraphicsFrameItem::margins() const
{
    return m_margins;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwColor& DwGraphicsFrameItem::backgroundColor() const
{
    return m_backgroundColor;
}