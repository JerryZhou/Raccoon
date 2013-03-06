#include "stable.h"
#include "dwgraphicsframeitem.h"
#include "dwgraphics/dwpainter.h"
#include "dwgui/dwtimer.h"
#include "dwgui/dwpropertyanimation.h"
#include "dwgui/dwanchors.h"

DW_IMPLEMENT_CLASS(DwGraphicsFrameItem, 'DGFM', DwGraphicsItem);

//------------------------------------------------------------------------------
class DwGraphicsTimer : public sigslot::has_slots
{
public:
    DwGraphicsTimer()
        : m_minInterval(0)
    {
        m_timer.sigTimeout.connect(this, &DwGraphicsTimer::onFrameOut);
    }

    virtual ~DwGraphicsTimer()
    {
        m_timer.sigTimeout.disconnect(this);
        m_timer.stop();
    }

    void start(DwGraphicsFrameItem *)
    {
        ;
    }

    void stop(DwGraphicsFrameItem *item)
    {
        m_items.remove(item);
    }

    void update(DwGraphicsFrameItem *item, int interval)
    {
        if (interval)
        {
            m_items[item] = interval;
        }
        else
        {
            stop(item);
        }

        if (interval < m_minInterval || m_minInterval == 0)
        {
            updateInterval();
        }
    }

    void onFrameOut()
    {
        int elaspes = m_timer.interval();

        DwMap<DwGraphicsFrameItem *, int> copyItems = m_items;
        DwMap<DwGraphicsFrameItem *, int>::iterator ite = copyItems.begin();
        while(ite != copyItems.end())
        {
            if (ite->second > 0)
            {
                ite->second -= elaspes;
                if (ite->second <= 0)
                {
                    ite->first->onFrameOut();
                }
            }
            ++ite;
        }

        updateInterval();
    }

    void updateInterval()
    {
        int newInterval = INT_MAX;
        DwMap<DwGraphicsFrameItem *, int>::iterator ite = m_items.begin();
        while(ite != m_items.end())
        {
            if (ite->second > 0 && newInterval > ite->second)
            {
                newInterval = ite->second;
            }
            ++ite;
        }
        if (newInterval != m_timer.interval())
        {
            m_minInterval = newInterval;
            m_timer.start(newInterval);
        }
    }

    int m_minInterval;
    DwMap<DwGraphicsFrameItem *, int> m_items;
    DwTimer m_timer;
};
DW_GLOBAL_STATIC(DwGraphicsTimer, _dw_graphicsTimer);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsFrameItem::DwGraphicsFrameItem(DwGraphicsItem *parent/* = NULL*/)
    : DwGraphicsItem(parent)
    , m_isDisable(0)
    , m_isFocus(0)
    , m_isHover(0)
    , m_isPressedDown(0)
    , m_margins(0, 0, 0, 0)
    , m_keepUpdate(true)
    , m_needDrawCenter(true)
    , m_animationWhen(0)
{
    setName(L"frameitem");
    m_backgroundColor.setAlpha(0);
    memset(m_pixmaps, 0, sizeof(m_pixmaps));
    _dw_graphicsTimer()->start(this);

    // default will be animation of hover state
    setAnimationWhen(DwWidget::SHover, true);

    // accessible
    setAccessibleName(tr("raccoon margins frame item"));
    setAccessibleDescription(tr("raccoon margins frame item"));
    setWhatsThis(tr("raccoon base margins frame item"));
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsFrameItem::~DwGraphicsFrameItem()
{
    _dw_graphicsTimer()->stop(this);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::paint(DwPainter *painter, const DwStyleOptionGraphicsItem *)
{
    if (m_backgroundColor.alpha() != 0)
    {
        painter->fillRect(boundingRect().toRect(), m_backgroundColor);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::setDisable(bool b)
{
    if (m_isDisable == b)
    {
        return;
    }

    m_isDisable = b;
    updateFrame();
    updateInner();
    changeState(DwWidget::SDisable, b);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::setHover(bool b)
{
    if (m_isHover == b)
    {
        return;
    }
    m_isHover = b;
    updateFrame();
    updateInner();
    changeState(DwWidget::SHover, b);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::setPressedDown(bool b)
{
    if (m_isPressedDown == b)
    {
        return;
    }
    m_isPressedDown = b;
    updateFrame();
    updateInner();
    changeState(DwWidget::SPressDown, b);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::setFocus(bool b)
{
    if (m_isFocus == b)
    {
        return;
    }
    m_isFocus = b;
    updateFrame();
    updateInner();
    changeState(DwWidget::SFocus, b);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::setMargins(const DwMargins& m)
{
    m_margins = m;
    for (int i=0; i<_countof(m_pixmaps); ++i)
    {
        if (m_pixmaps[i])
        {
            m_pixmaps[i]->setMargins(m);
        }
    }
    update();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::setImage(DwWidget::FrameState state, const DwUrl& image)
{
    setImage(state, DwResourceInterface::acquireImage(image));
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::setImage(DwWidget::FrameState state, const DwImage &img)
{
    DW_ASSERT(state >= 0 && state < _countof(m_pixmaps));
    pixmapOf(state)->setPixmap(img);
    updateFrame();
    update();
}

//------------------------------------------------------------------------------
/**
*/
DwImage DwGraphicsFrameItem::imageOf(DwWidget::FrameState state) const
{
    DW_ASSERT(state >= 0 && state < _countof(m_pixmaps));
    if (m_pixmaps[state])
    {
        return m_pixmaps[state]->pixmap();
    }
    return DwImage();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::setMask(const DwUrl& url)
{
    setImage(DwWidget::SStateMax, url);
}

//------------------------------------------------------------------------------
/**
*/
DwImage DwGraphicsFrameItem::mask() const
{
    return imageOf(DwWidget::SStateMax);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::setBackgroundColor(const DwColor &c)
{
    m_backgroundColor = c;
    update();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::changeState(DwWidget::FrameState state, bool b)
{
    DW_ASSERT(state >= 0 && state < _countof(m_pixmaps));
    if (!m_pixmaps[state] || m_pixmaps[state]->pixmap().isNull())
    {
        return;
    }

    if (isAnimationWhen(state))
    {
        startAnimation(state, b);
    }
    else
    {
        pixmapOf(state)->setVisible(b);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::startAnimation(DwWidget::FrameState state, bool b)
{
    DW_ASSERT(state >= 0 && state < _countof(m_pixmaps));
    pixmapOf(state)->setVisible(true);
    if (b)
    {
        appendAnimation(pixmapOf(state), 0.0, 1.0, 150);
    }
    else
    {
        appendAnimation(pixmapOf(state), 1.0, 0.0, 380);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::appendAnimation(DwGraphicsItem *item, DwReal start, DwReal end, int duration)
{
    DW_ASSERT(item);
    item->clearAnimations();

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
DwGraphicsMarginsPixmapItem *DwGraphicsFrameItem::pixmapOf(DwWidget::FrameState state)
{
    DW_ASSERT(state >= 0 && state < _countof(m_pixmaps));
    if (!m_pixmaps[state])
    {
        m_pixmaps[state] = new DwGraphicsMarginsPixmapItem(this);
        m_pixmaps[state]->setZValue(state);
        m_pixmaps[state]->anchors()->setFill(this);
        m_pixmaps[state]->setMargins(m_margins);
        m_pixmaps[state]->setDrawCenter(m_needDrawCenter);
        if (state != DwWidget::SNormal)
        {
            m_pixmaps[state]->setVisible(false);
        }
    }
    return m_pixmaps[state];
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::updateInner()
{
    if (m_keepUpdate)
    {
        update();
    }
}

//------------------------------------------------------------------------------
/**
	Disable > PressDown > Focus > Hover > Normal
*/
DwImage DwGraphicsFrameItem::currentImage() const
{
    if (isDisable()
            && m_pixmaps[DwWidget::SDisable]
            && !m_pixmaps[DwWidget::SDisable]->pixmap().isNull())
    {
        return m_pixmaps[DwWidget::SDisable]->pixmap();
    }
    if (isPressedDown()
            && m_pixmaps[DwWidget::SPressDown]
            &&!m_pixmaps[DwWidget::SPressDown]->pixmap().isNull())
    {
        return m_pixmaps[DwWidget::SPressDown]->pixmap();
    }
    if (isFocus()
            && m_pixmaps[DwWidget::SFocus]
            && !m_pixmaps[DwWidget::SFocus]->pixmap().isNull())
    {
        return m_pixmaps[DwWidget::SHover]->pixmap();
    }
    if (isHover()
            && m_pixmaps[DwWidget::SHover]
            && !m_pixmaps[DwWidget::SHover]->pixmap().isNull())
    {
        return m_pixmaps[DwWidget::SHover]->pixmap();
    }

    if (m_pixmaps[DwWidget::SNormal])
    {
        return m_pixmaps[DwWidget::SNormal]->pixmap();
    }

    return DwImage();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::onFrameOut()
{
    DwImage curImage = currentImage();
    if (!curImage.isNull())
    {
        int frameCount = curImage.frameCount();
        if (frameCount > 1)
        {
            int curActive = curImage.curActiveFrame();
            curActive = ++curActive %frameCount;
            const_cast<DwImage&>(curImage).setActiveFrame(curActive);

            int interval = curImage.nextInterval();
            updateFrame(interval);

            update();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::updateFrame()
{
    DwImage curImage = currentImage();
    if (!curImage.isNull())
    {
        int frameCount = curImage.frameCount();
        if (frameCount > 1)
        {
            int interval = curImage.nextInterval();
            // skip the empty frame

            if (interval <=0 )
            {
                while(interval <= 0 && frameCount > 0)
                {
                    onFrameOut();
                    --frameCount;
                    interval = curImage.nextInterval();
                }
            }
            else
            {
                updateFrame(interval);
            }

        }
        else
        {
            updateFrame(0);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::updateFrame(int interval)
{
    if (interval > 0)
    {
        _dw_graphicsTimer()->update(this, interval);
    }
    else
    {
        _dw_graphicsTimer()->stop(this);
    }

}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::setKeepUpdate(bool b)
{
    m_keepUpdate = b;
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsFrameItem::isKeepUpdate() const
{
    return m_keepUpdate;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::setDrawCenter(bool b)
{
    m_needDrawCenter = b;
    for (int i=0; i<_countof(m_pixmaps); ++i)
    {
        if (m_pixmaps[i])
        {
            m_pixmaps[i]->setDrawCenter(b);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsFrameItem::setAnimationWhen(DwWidget::FrameState state, bool b)
{
    if (b)
    {
        m_animationWhen |= (1<<state);
    }
    else
    {
        m_animationWhen &= ~(1<<state);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsFrameItem::isAnimationWhen(DwWidget::FrameState state) const
{
    return !!(m_animationWhen & (1<<state));
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsFrameItem::isDrawCenter() const
{
    return m_needDrawCenter;
}