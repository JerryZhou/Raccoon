#pragma once
#include "dwgui/dwaccessibleobject.h"

//------------------------------------------------------------------------------
class DwGraphicsItem;

//------------------------------------------------------------------------------
class DwAccessibleItem : public DwAccessibleObject
{
    DW_DECLARE_CLASS(DwAccessibleItem);
public:
    explicit DwAccessibleItem(DwGraphicsItem *o=NULL, Role r = Client, const DwString& name = DwString());

    int childCount() const;
    int indexOfChild(const DwAccessibleInterface *child) const;
    Relation relationTo(int child, const DwAccessibleInterface *other, int otherChild) const;

    int childAt(int x, int y) const;
    DwRect rect(int child) const;
    int navigate(RelationFlag rel, int entry, DwAccessibleInterface **target) const;

    DwString text(Text t, int child) const;
    Role role(int child) const;
    State state(int child) const;

    int userActionCount(int child) const;
    DwString actionText(int action, Text t, int child) const;
    bool doAction(int action, int child, const DwVariantList &params);

protected:
    ~DwAccessibleItem();
    DwGraphicsItem *widget() const;
    DwRttiObject *parentObject() const;

    // used to navigate the child
    virtual DwVector<DwGraphicsItem*> childWidgets()const;
    DwVector<DwGraphicsItem*> childWidgetsOf(const DwGraphicsItem* obj)const;

    void addControllingSignal(const DwString &signal);
    void setValue(const DwString &value);
    void setDescription(const DwString &desc);
    void setHelp(const DwString &help);
    void setAccelerator(const DwString &accel);

    typedef DwVector<DwString> DwStringList;
    Role m_role;
    DwString m_name;
    DwString m_description;
    DwString m_value;
    DwString m_help;
    DwString m_accelerator;
    DwStringList m_primarySignals;
    const DwAccessibleInterface *m_asking;
};// end of DwAccessibleItem