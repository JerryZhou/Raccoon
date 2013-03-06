#pragma once
#include "dwgui/dwaccessible.h"
#include "dwgui/dwrttiobjectptr.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAccessibleObject : public DwAccessibleInterface
{
    DW_DECLARE_ABSTRACT_CLASS(DwAccessibleObject);
public:
    explicit DwAccessibleObject(DwRttiObject *obj);
    virtual ~DwAccessibleObject();

    bool isValid() const;
    DwRttiObject *object() const;

    // properties
    DwRect rect(int child) const;
    void setText(Text t, int child, const DwString &text);

    // actions
    int userActionCount(int child) const;
    bool doAction(int action, int child, const DwVariantList &params);
    DwString actionText(int action, Text t, int child) const;

protected:
    DwRttiObjectPtr<DwRttiObject> m_object;
};// end of DwAccessibleObject