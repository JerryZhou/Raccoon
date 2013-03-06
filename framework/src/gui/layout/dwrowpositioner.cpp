#include "stable.h"
#include "dwrowpositioner.h"
#include "dwanchors.h"
#include "dwgui/dwringlogcase.h"
#include "graphicsview/dwdummys.h"

DW_IMPLEMENT_CLASS(DwRowPositioner, 'GRPR', DwBasePositioner);

// NB! THE ALGORITHEM FROM QT4.7(DECLARATIVE)
//------------------------------------------------------------------------------
/**
*/
DwRowPositioner::DwRowPositioner(DwGraphicsItem *parent/* = NULL*/, DwGraphicsScene *scene/* = NULL*/)
    : DwBasePositioner(parent, scene)
{
    m_positionType = DwBasePositioner::Horizontal;
}

//------------------------------------------------------------------------------
/**
*/
DwRowPositioner::~DwRowPositioner()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwRowPositioner::doPositioning(DwSizeF *contentSize)
{
    DwReal hoffset = 0;

    for (int ii = 0; ii < m_positionedItems.count(); ++ii)
    {
        const PositionedItem &child = m_positionedItems.at(ii);
        if (!child.item || !child.isVisible)
            continue;

        if(child.item->x() != hoffset)
            positionX(hoffset, child);

        contentSize->setHeight(dwMax(contentSize->height(), child.item->height()));

        hoffset += child.item->width();
        hoffset += spacing();
    }

    contentSize->setWidth(hoffset - spacing());
}

//------------------------------------------------------------------------------
/**
*/
void DwRowPositioner::reportConflictingAnchors()
{
    for (int ii = 0; ii < m_positionedItems.count(); ++ii)
    {
        const PositionedItem &child = m_positionedItems.at(ii);
        if (child.item && child.item->hasAnchors())
        {
            DwAnchors *anchors = child.item->anchors();
            int usedAnchors = anchors->usedAnchors();
            if (usedAnchors & Dw::Horizontal_Mask ||
                    anchors->fill() || anchors->centerIn())
            {
                m_anchorConflict = true;
                break;
            }
        }
    }
    if (m_anchorConflict)
    {
        DW_UI_LOGER() << name() << DW_TR("Cannot specify left, right, horizontalCenter, fill or centerIn anchors for items inside Row");
    }
}