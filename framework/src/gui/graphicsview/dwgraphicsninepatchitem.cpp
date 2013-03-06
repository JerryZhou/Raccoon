#include "stable.h"
#include "dwgraphicsninepatchitem.h"
#include "dwgraphics/dwpainter.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwanchors.h"

DW_IMPLEMENT_CLASS(DwGraphicsNinePatchItem, 'NPPM', DwGraphicsItem);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsNinePatchItem::DwGraphicsNinePatchItem(DwGraphicsItem *parent/*=NULL*/)
: DwGraphicsItem(parent)
, m_idx(0)
, m_drawCenter(true)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsNinePatchItem::DwGraphicsNinePatchItem(const DwUrl& uri, int idx/*=0*/, DwGraphicsItem *parent/*=NULL*/)
: DwGraphicsItem(parent)
, m_drawCenter(true)
{
    setNinePatch(uri, idx);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsNinePatchItem::paint(DwPainter *painter, const DwStyleOptionGraphicsItem *)
{
    if (!m_pixmap.isNull())
    {
        DwNinePatch::NinePatchInfo info = DwNinePatch::ninePatchInfo(m_pixmap, m_idx);
        if (info.margins.isZero())
        {
            DwRect dst = boundingRect().toAlignedRect();
            painter->drawImageScaled(DwRect(dst.topLeft(), info.srcRect.size()), m_pixmap, info.srcRect);
        }
        else
        {
            painter->drawNinePatchs(boundingRect().toAlignedRect(), m_idx, m_pixmap, m_drawCenter);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsNinePatchItem::setPixmap(const DwUrl &uri)
{
    DwImage pixmap = DW_PIXMAP_ASYNC(uri, this);
    if (m_pixmap != pixmap)
    {
        m_pixmap = pixmap;
        updateSize();
        update();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsNinePatchItem::setPixmap(const DwImage& pixmap, bool force/* = false*/)
{
    if (m_pixmap != pixmap || force)
    {
        m_pixmap = pixmap;
        updateSize();
        update();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsNinePatchItem::setIdx(int idx)
{
    if (m_idx != idx)
    {
        m_idx = idx;
        updateSize();
        update();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsNinePatchItem::setNinePatch(const DwUrl &uri, int idx)
{
    setPixmap(uri);
    setIdx(idx);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsNinePatchItem::setDrawCenter(bool b)
{
    if (m_drawCenter == b)
    {
        return;
    }
    m_drawCenter = b;
    update();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsNinePatchItem::updateSize()
{
    if (!hasAnchors() || !anchors()->fill())
    {
        DwNinePatch::NinePatchInfo info = DwNinePatch::ninePatchInfo(m_pixmap, m_idx);
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