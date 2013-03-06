#include "stable.h"
#include "dwgraphicsbackgrounditem.h"
#include "dwgui/dwanchors.h"
#include "dwgui/dwpropertyanimation.h"
#include "graphicsview/dwdummys.h"
#include "dwgraphics/dwpainter.h"
#include "dwgui/dwresourceinterface.h"

DW_IMPLEMENT_CLASS(DwGraphicsBackgroundItem, 'GBEM', DwGraphicsItem);


//------------------------------------------------------------------------------
/**
*/
DwGraphicsBackgroundItem::DwGraphicsBackgroundItem( DwGraphicsItem *parent/*=NULL*/ )
: DwGraphicsItem(parent)
, m_isDisable(0)
, m_isFocus(0)
, m_isHover(0)
, m_isPressedDown(0)
, m_needDrawCenter(true)
, m_animationWhen(0)
, m_idxOffset(0)
, m_color(0, 0, 0, 0)
{
    m_nines[EraseOut] = new DwGraphicsNinePatchItem(this);
    m_nines[EraseIn] = new DwGraphicsNinePatchItem(this);
    
    m_nines[EraseOut]->anchors()->setFill(this);
    m_nines[EraseIn]->anchors()->setFill(this);

    // default will be animation of hover state
    setAnimationWhen(DwWidget::SHover, true);
    //setAnimationWhen(DwWidget::SFocus, true);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsBackgroundItem::~DwGraphicsBackgroundItem()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::setURL( const DwUrl& uri )
{
    // try to accept the async event
    DW_PIXMAP_ASYNC(uri, this);
    // set the url to the real widget
    m_nines[EraseIn]->setPixmap(uri);
    m_nines[EraseOut]->setPixmap(uri);
    // may be we are the alone widget
    updateSize();
}

//------------------------------------------------------------------------------
/**
*/
DwUrl DwGraphicsBackgroundItem::url() const
{
    return DwUrl(m_nines[EraseIn]->pixmap().path());
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::setPixmap(const DwImage& img)
{
    m_nines[EraseIn]->setPixmap(img);
    m_nines[EraseOut]->setPixmap(img);

    updateSize();
}

//------------------------------------------------------------------------------
/**
*/
DwImage DwGraphicsBackgroundItem::pixmap() const
{
    return m_nines[EraseIn]->pixmap();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::setDisable( bool b )
{
    if (m_isDisable == b)
    {
        return;
    }

    m_isDisable = b;
    changeState(DwWidget::SDisable, b);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::setHover( bool b )
{
    if (m_isHover == b)
    {
        return;
    }
    m_isHover = b;
    changeState(DwWidget::SHover, b);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::setPressedDown( bool b )
{
    if (m_isPressedDown == b)
    {
        return;
    }
    m_isPressedDown = b;
    changeState(DwWidget::SPressDown, b);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::setFocus( bool b )
{
    if (m_isFocus == b)
    {
        return;
    }
    m_isFocus = b;
    changeState(DwWidget::SFocus, b);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::setAnimationWhen(DwWidget::FrameState state, bool b)
{
    if (b)
    {
        m_animationWhen |= (1<<state);
    }
    else
    {
        m_animationWhen &= ~(1<<state);
    }

    update();
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsBackgroundItem::isAnimationWhen(DwWidget::FrameState state) const
{
    return !!(m_animationWhen & (1<<state));
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::setDrawCenter(bool b)
{
    m_nines[EraseIn]->setDrawCenter(b);
    m_nines[EraseOut]->setDrawCenter(b);
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsBackgroundItem::isDrawCenter() const
{
    return m_needDrawCenter;
}

//------------------------------------------------------------------------------
/**
*/
void  DwGraphicsBackgroundItem::setIdxOffset(int idxOst)
{
    m_idxOffset = idxOst;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsNinePatchItem *DwGraphicsBackgroundItem::ninesOf(int i)
{
    return m_nines[i];
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::changeState(DwWidget::FrameState state, bool b)
{
    // pp state
    DwWidget::FrameState inS, outS;
    inOutStateWhenStateChange(state, b, inS, outS);
    DW_CHECK(inS != outS);

    if (isAnimationWhen(inS) || isAnimationWhen(outS))
    {
        // rember in anim
        bool isAnimationIn = false;
        int inIdx = (int)inS+idxOffset();
        int outIdx = (int)outS+idxOffset();

        // in
        ninesOf(EraseIn)->setVisible(true);
        if (isAnimationWhen(inS))
        {
            isAnimationIn = true;
            if (ninesOf(EraseIn)->idx() != inIdx)
            {
                ninesOf(EraseIn)->setIdx(inIdx);
                appendAnimation(ninesOf(EraseIn), 0.0, 1.0, 150);
            }
        }
        else
        {
            clearAnimation(ninesOf(EraseIn));
            ninesOf(EraseIn)->setOpacity(1.0);
            ninesOf(EraseIn)->setIdx(inIdx);
        }

        // out
        // arealdy animation in, so we cannot be invisible(or there is a flash )
        if (isAnimationIn)
        {
            clearAnimation(ninesOf(EraseOut));
            ninesOf(EraseOut)->setOpacity(1.0);
            ninesOf(EraseOut)->setIdx(outIdx);
            ninesOf(EraseOut)->setVisible(true);
        }
        else
        {
            // TODO: Realy need to keep the state's idxs are different from each other!!
            if (false && isAnimationWhen(outS) )
            {
                ninesOf(EraseOut)->setVisible(true);
                if (ninesOf(EraseOut)->idx() != (outIdx))
                {
                    ninesOf(EraseOut)->setIdx(outIdx);
                    appendAnimation(ninesOf(EraseOut), 1.0, 0.0, 380);
                }
            }
            else
            {
                clearAnimation(ninesOf(EraseOut));
                ninesOf(EraseOut)->setOpacity(1.0);
                ninesOf(EraseOut)->setVisible(false);
                ninesOf(EraseOut)->setIdx(outIdx);
            }
        }
    }
    else
    {
        ninesOf(EraseIn)->setVisible(true);
        ninesOf(EraseIn)->setOpacity(1.0);
        ninesOf(EraseIn)->setIdx(inS+idxOffset());

        ninesOf(EraseOut)->setVisible(false);
        ninesOf(EraseOut)->setOpacity(1.0);
        ninesOf(EraseOut)->setIdx(outS+idxOffset());
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::appendAnimation(DwGraphicsItem *item, DwReal start, DwReal end, int duration)
{
    clearAnimation(item);

    DwPropertyAnimation *anims = new DwPropertyAnimation(NULL);
    anims->setTargetItem(item);
    anims->setProperty("opacity");
    anims->setStartValue(start);
    anims->setEndValue(end);
    anims->setEasingCurve(Dw::InOutQuad);
    anims->setDuration(duration);
    anims->start(Dw::DeleteWhenStopped);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::clearAnimation(DwGraphicsItem *item)
{
    DW_ASSERT(item);
    item->clearAnimations();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::inOutStateWhenStateChange(DwWidget::FrameState state, bool b, 
                               DwWidget::FrameState &inS, DwWidget::FrameState &outS)
{
    DW_UNUSED(state);
    DW_UNUSED(b);
    DwImage pixmap = ninesOf(EraseIn)->pixmap();

    outS = (DwWidget::FrameState)ninesOf(EraseIn)->idx();

    if (m_isDisable 
        && DwNinePatch::isValid(pixmap, idxOffset() + DwWidget::SDisable)){
        inS = DwWidget::SDisable;
    }
    else if (m_isPressedDown 
        && DwNinePatch::isValid(pixmap, idxOffset() + DwWidget::SPressDown)){
        inS = DwWidget::SPressDown;
    }
    else if (m_isFocus 
        && DwNinePatch::isValid(pixmap, idxOffset() + DwWidget::SFocus)){
        inS = DwWidget::SFocus;
    }
    else if (m_isHover 
        && DwNinePatch::isValid(pixmap, idxOffset() + DwWidget::SHover)){
        inS = DwWidget::SHover;
    }
    else{
        inS = DwWidget::SNormal;
    }
}

//------------------------------------------------------------------------------
/**
*/
DwColor DwGraphicsBackgroundItem::color() const
{
    return m_color;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::setColor(const DwColor &c)
{
    if (m_color != c)
    {
        m_color = c;
        update();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::paint(DwPainter *painter, 
                                     const DwStyleOptionGraphicsItem *option)
{
    DW_UNUSED(option);
    if (m_color.alpha() != 0)
    {
        painter->fillRect(boundingRect(), m_color);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsBackgroundItem::updateSize()
{
    // TODO: check if the anchor already takeship of size
    if (!hasAnchors() || !anchors()->fill())
    {
        if (DwNinePatch::isValid(m_nines[EraseIn]->pixmap(), m_nines[EraseIn]->idx()))
        {
            DwNinePatch::NinePatchInfo info = DwNinePatch::ninePatchInfo(m_nines[EraseIn]->pixmap(), 
                m_nines[EraseIn]->idx());
            if (info.margins.isZero())
            {
                setSize(info.srcRect.size());
            }
            else 
            {
                if(!info.margins.left() && !info.margins.right())
                {
                    setWidth(info.srcRect.width());
                }
                if (!info.margins.top() && !info.margins.bottom())
                {
                    setHeight(info.srcRect.height());
                }
            }
        }
    }
}