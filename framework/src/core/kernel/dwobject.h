#pragma once

#include <dwcore/dwshareddata.h>
#include <dwcore/dwstring.h>
#include <dwcore/dwvector.h>
#include <dwcore/dwset.h>

class DW_CORE_EXPORT DwObject
{
    DW_DISABLE_COPY(DwObject)

public:
    explicit DwObject(DwObject *parent = NULL);
    virtual ~DwObject();

    DwString objectName() const;
    void setObjectName(const DwString &name);

    int childCount() const;
    DwObject *child(int i) const;

    DwObject *parent() const
    {
        return m_parent;
    }
    void setParent(DwObject *);

    bool operator==(const DwObject &other) const;

    static void addGuard(DwObject **ptr);
    static void removeGuard(DwObject **ptr);
    static void clearGuard(DwObject *ptr);
    static void changeGuard(DwObject **ptr, DwObject *o);

private:
    DwObject *m_parent;
    DwString m_objectName;
    DwVector<DwObject*> m_children;
};
