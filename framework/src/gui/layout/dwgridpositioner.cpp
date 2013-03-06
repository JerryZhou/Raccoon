#include "stable.h"
#include "dwgridpositioner.h"
#include "graphicsview/dwdummys.h"
#include "dwanchors.h"

DW_IMPLEMENT_CLASS(DwGridPositioner, 'GGPR', DwBasePositioner);

// NB! THE ALGORITHEM FROM QT4.7(DECLARATIVE)
//------------------------------------------------------------------------------
/**
*/
DwGridPositioner::DwGridPositioner(DwGraphicsItem *parent/* = NULL*/, DwGraphicsScene *scene/* = NULL*/)
    : DwBasePositioner(parent, scene)
    , m_rows(0)
    , m_columns(0)
    , m_flow(LeftToRight)
{
    m_positionType = DwBasePositioner::Both;
}

//------------------------------------------------------------------------------
/**
*/
DwGridPositioner::~DwGridPositioner()
{

}

//------------------------------------------------------------------------------
/**
*/
void DwGridPositioner::setRows(const int rows)
{
    if (m_rows != rows)
    {
        m_rows = rows;
        prePositioning();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGridPositioner::setColumns(const int columns)
{
    if (m_columns != columns)
    {
        m_columns = columns;
        prePositioning();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGridPositioner::setFlow(Flow f)
{
    if (m_flow != f)
    {
        m_flow = f;
        prePositioning();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGridPositioner::doPositioning(DwSizeF *contentSize)
{
    int c = m_columns;
    int r = m_rows;
    //Is allocating the extra DwVector too much overhead?
    DwVector<PositionedItem> visibleItems;//we aren't concerned with invisible items
    visibleItems.reserve(m_positionedItems.count());
    for(int i=0; i < m_positionedItems.count(); i++)
    {
        if(m_positionedItems[i].item && m_positionedItems[i].isVisible)
        {
            visibleItems.append(m_positionedItems[i]);
        }
    }

    int numVisible = visibleItems.count();
    if (m_columns <= 0 && m_rows <= 0)
    {
        c = 4;
        r = (numVisible+3)/4;
    }
    else if (m_rows <= 0)
    {
        r = (numVisible+(m_columns-1))/m_columns;
    }
    else if (m_columns <= 0)
    {
        c = (numVisible+(m_rows-1))/m_rows;
    }

    DwVector<DwReal> maxColWidth;
    DwVector<DwReal> maxRowHeight;
    int childIndex =0;
    if (m_flow == LeftToRight)
    {
        for (int i=0; i < r; i++)
        {
            for (int j=0; j < c; j++)
            {
                if (j==0) maxRowHeight.append(0);
                if (i==0) maxColWidth.append(0);

                if (childIndex == visibleItems.count())
                    break;

                const PositionedItem &child = visibleItems.at(childIndex++);
                if (child.item->width() > maxColWidth[j])
                {
                    maxColWidth[j] = child.item->width();
                }
                if (child.item->height() > maxRowHeight[i])
                {
                    maxRowHeight[i] = child.item->height();
                }
            }
        }
    }
    else
    {
        for (int j=0; j < c; j++)
        {
            for (int i=0; i < r; i++)
            {
                if (j==0) maxRowHeight.append(0);
                if (i==0) maxColWidth.append(0);

                if (childIndex == m_positionedItems.count())
                    break;

                const PositionedItem &child = visibleItems.at(childIndex++);
                if (child.item->width() > maxColWidth[j])
                {
                    maxColWidth[j] = child.item->width();
                }
                if (child.item->height() > maxRowHeight[i])
                {
                    maxRowHeight[i] = child.item->height();
                }
            }
        }
    }

    DwReal xoffset=0;
    DwReal yoffset=0;
    int curRow =0;
    int curCol =0;
    for (int i = 0; i < visibleItems.count(); ++i)
    {
        const PositionedItem &child = visibleItems.at(i);
        if((child.item->x() != xoffset) || (child.item->y() != yoffset))
        {
            positionX(xoffset, child);
            positionY(yoffset, child);
        }

        if (m_flow == LeftToRight)
        {
            contentSize->setWidth(dwMax(contentSize->width(), xoffset + child.item->width()));
            contentSize->setHeight(yoffset + maxRowHeight[curRow]);

            xoffset += maxColWidth[curCol] + spacing();
            curCol++;
            curCol %= c;
            if (!curCol)
            {
                yoffset += maxRowHeight[curRow] + spacing();
                xoffset = 0;
                curRow++;
                if (curRow>=r)
                    break;
            }
        }
        else
        {
            contentSize->setHeight(dwMax(contentSize->height(), yoffset + child.item->height()));
            contentSize->setWidth(xoffset + maxColWidth[curCol]);

            yoffset += maxRowHeight[curRow] + spacing();
            curRow++;
            curRow %= r;
            if (!curRow)
            {
                xoffset += maxColWidth[curCol] + spacing();
                yoffset=0;
                curCol++;
                if (curCol>=c)
                    break;
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGridPositioner::reportConflictingAnchors()
{
    for (int ii = 0; ii < m_positionedItems.count(); ++ii)
    {
        const PositionedItem &child = m_positionedItems.at(ii);
        if (child.item && child.item->hasAnchors())
        {
            DwAnchors *anchors = child.item->anchors();
            if (anchors && (anchors->usedAnchors() || anchors->fill() || anchors->centerIn()))
            {
                m_anchorConflict = true;
                break;
            }
        }
    }
    if (m_anchorConflict)
    {
        DW_UI_LOGER() << name() << DW_TR("Cannot specify anchors for items inside Grid");
    }
}