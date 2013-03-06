#pragma once
#include "dwgui/dwsimpleedit.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwMultiEdit : public DwSimpleEdit
{
    DW_DECLARE_CLASS(DwMultiEdit);
public:
    explicit DwMultiEdit(DwGraphicsItem *parent = NULL);
    virtual ~DwMultiEdit();

    // set the org conetnt rect
    void setOrgContentRect(const DwRect &r);
    // the org content rect
    DwRectF orgContentRect() const;
    // update the OrgContentRect
    void updateOrgContent();
protected:
private:
};// end of DwMultiEdit
DW_REGISTER_CLASS(DwMultiEdit);