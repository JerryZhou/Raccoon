#include "stable.h"
#include "dwcolumnpositioner.h"
#include "dwanchors.h"
#include "graphicsview/dwdummys.h"

DW_IMPLEMENT_CLASS(DwColumnPositioner, 'GCPR', DwBasePositioner);

// NB! THE ALGORITHEM FROM QT4.7(DECLARATIVE)
//------------------------------------------------------------------------------
/**
*/
DwColumnPositioner::DwColumnPositioner(DwGraphicsItem *parent/* = NULL*/, DwGraphicsScene *scene/* = NULL*/)
    : DwBasePositioner(parent, scene)
{
    m_positionType = DwBasePositioner::Vertical;
}

//------------------------------------------------------------------------------
/**
*/
DwColumnPositioner::~DwColumnPositioner()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwColumnPositioner::doPositioning(DwSizeF *contentSize)
{
    DW_ASSERT(contentSize);
    DwReal voffset = 0;

    for (int ii = 0; ii < m_positionedItems.count(); ++ii)
    {
        const PositionedItem &child = m_positionedItems.at(ii);
        if (!child.item || !child.isVisible)
            continue;

        if(child.item->y() != voffset)
            positionY(voffset, child);

        contentSize->setWidth(dwMax(contentSize->width(), child.item->width()));

        voffset += child.item->height();
        voffset += spacing();
    }

    contentSize->setHeight(voffset - spacing());
}

//------------------------------------------------------------------------------
/**
*/
void DwColumnPositioner::reportConflictingAnchors()
{
    for (int ii = 0; ii < m_positionedItems.count(); ++ii)
    {
        const PositionedItem &child = m_positionedItems.at(ii);
        if (child.item && child.item->hasAnchors())
        {
            DwAnchors *anchors = child.item->anchors();

            int usedAnchors = anchors->usedAnchors();
            if (usedAnchors & Dw::VerticalAnchor_Mask ||
                    anchors->fill() || anchors->centerIn())
            {
                m_anchorConflict = true;
                break;
            }
        }
    }
    if (m_anchorConflict)
    {
        DW_UI_LOGER() << name() << DW_TR("Cannot specify top, bottom, verticalCenter, fill or centerIn anchors for items inside Column");
    }
}