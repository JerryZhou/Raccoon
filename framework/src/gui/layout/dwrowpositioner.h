#pragma once
#include "dwgui/dwbasepositioner.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwRowPositioner : public DwBasePositioner
{
    DW_DECLARE_CLASS(DwRowPositioner);
public:
    DwRowPositioner(DwGraphicsItem *parent = NULL, DwGraphicsScene *scene = NULL);
    virtual ~DwRowPositioner();

protected:
    virtual void doPositioning(DwSizeF *contentSize);
    virtual void reportConflictingAnchors();
private:
};// end of DwRowPositioner
DW_REGISTER_CLASS(DwRowPositioner);