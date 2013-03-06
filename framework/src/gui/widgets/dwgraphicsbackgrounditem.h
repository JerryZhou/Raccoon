#pragma once
#include "dwgui/dwgraphicsninepatchitem.h"

// The Resource Format : Normal | Hover | Pressdown | SFocus | Disable
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsBackgroundItem : public DwGraphicsItem
{
    DW_DECLARE_CLASS(DwGraphicsBackgroundItem);
public:
    explicit DwGraphicsBackgroundItem(DwGraphicsItem *parent=NULL);
    virtual ~DwGraphicsBackgroundItem();

    // url
    void setURL(const DwUrl& uri);
    DwUrl url() const;
    void setPixmap(const DwImage& img);
    DwImage pixmap() const;

    // states control
    void setDisable(bool b);
    void setHover(bool b);
    void setPressedDown(bool b);
    void setFocus(bool b);

    inline bool isDisable() const;
    inline bool isHover() const;
    inline bool isPressedDown() const;
    inline bool isFocus() const;

    // animations
    void setAnimationWhen(DwWidget::FrameState state, bool b);
    bool isAnimationWhen(DwWidget::FrameState state)const;

    // set We need draw center
    void setDrawCenter(bool b);
    bool isDrawCenter() const;

    // used for multi state
    void setIdxOffset(int idxOst);
    int idxOffset() const;

    // color
    DwColor color() const;
    void setColor(const DwColor &c);

    virtual void paint(DwPainter *painter, 
        const DwStyleOptionGraphicsItem *option);// = 0;

protected:
    enum {EraseIn, EraseOut};

    DwGraphicsNinePatchItem *ninesOf(int i);
    void changeState(DwWidget::FrameState, bool b);
    void startAnimation(DwWidget::FrameState, bool b);
    void appendAnimation(DwGraphicsItem *item, DwReal start, DwReal end, int duration);
    void clearAnimation(DwGraphicsItem *item);
    void inOutStateWhenStateChange(DwWidget::FrameState state, bool b, 
        DwWidget::FrameState &inS, DwWidget::FrameState &outS);
    void updateSize();
    bool m_isDisable : 1;
    bool m_isHover : 1;
    bool m_isPressedDown : 1;
    bool m_isFocus : 1;
    bool m_needDrawCenter : 1;
    uint16_t m_animationWhen;
    uint16_t m_idxOffset;
    DwColor m_color;
    DwGraphicsNinePatchItem *m_nines[2];
};// end of DwGraphicsBackgroundItem
DW_REGISTER_CLASS(DwGraphicsBackgroundItem)

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsBackgroundItem::isDisable() const
{
    return m_isDisable;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsBackgroundItem::isHover() const
{
    return m_isHover;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsBackgroundItem::isPressedDown() const
{
    return m_isPressedDown;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsBackgroundItem::isFocus() const
{
    return m_isFocus;
}

//------------------------------------------------------------------------------
/**
*/
inline int DwGraphicsBackgroundItem::idxOffset() const
{
    return m_idxOffset;
}