#pragma once
#include "dwgui/dwaccessibleobject.h"

//------------------------------------------------------------------------------
class DwAccessibleApplication : public DwAccessibleObject
{
    DW_DECLARE_CLASS(DwAccessibleApplication);
public:
    DwAccessibleApplication();

    // relations
    int childCount() const;
    int indexOfChild(const DwAccessibleInterface*) const;
    Relation relationTo(int, const DwAccessibleInterface *, int) const;

    // navigation
    int childAt(int x, int y) const;
    int navigate(RelationFlag, int, DwAccessibleInterface **) const;

    // properties and state
    DwString text(Text t, int child) const;
    Role role(int child) const;
    State state(int child) const;

    // actions
    int userActionCount(int child) const;
    bool doAction(int action, int child, const DwVariantList &params);
    DwString actionText(int action, Text t, int child) const;
protected:
private:
};// end of DwAccessibleApplication