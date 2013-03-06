#pragma once
#include "dwgui/dwvariantanimation.h"
#include "dwgui/dwrttiobjectptr.h"

//------------------------------------------------------------------------------
class DwGraphicsItem;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwPropertyAnimation : public DwVariantAnimation
{
    DW_DECLARE_CLASS(DwPropertyAnimation);
public:
    DwPropertyAnimation(DwObject *parent = NULL);
    virtual ~DwPropertyAnimation();

    void setTargetItem(DwGraphicsItem *item);
    DwGraphicsItem *targetItem() const;

    void setProperty(const DwString& );
    DwString propertyOfThis() const;

    virtual void onAttachTo(DwRttiObject *obj);
    virtual void onDettachTo(DwRttiObject *obj);

protected:
    virtual void updateCurrentValue(const DwVariant &v);

    DwRttiObjectPtr<DwGraphicsItem> m_targetItem;
    DwString m_property;
private:
};// end of DwPropertyAnimation
DW_REGISTER_CLASS(DwPropertyAnimation);