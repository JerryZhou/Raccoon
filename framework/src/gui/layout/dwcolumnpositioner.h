#pragma once
#include "dwbasepositioner.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwColumnPositioner : public DwBasePositioner
{
    DW_DECLARE_CLASS(DwColumnPositioner);
public:
    DwColumnPositioner(DwGraphicsItem *parent = NULL, DwGraphicsScene *scene = NULL);
    virtual ~DwColumnPositioner();

protected:
    virtual void doPositioning(DwSizeF *contentSize);
    virtual void reportConflictingAnchors();
private:
};// end of DwColumnPositioner
DW_REGISTER_CLASS(DwColumnPositioner);