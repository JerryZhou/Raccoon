#pragma once
#include "dwgui/dwabstractanimation.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAnimationGroup : public DwAbstractAnimation
{
    DW_DECLARE_CLASS(DwAnimationGroup);
public:
    DwAnimationGroup(DwObject *parent= NULL) :
        DwAbstractAnimation(parent)
    {
        ;
    }
protected:
private:
};// end of DwAnimationGroup