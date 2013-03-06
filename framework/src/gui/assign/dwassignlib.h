#pragma once

#include "dwgui/dwrttiobject.h"
#include "dwgui/dwurl.h"
#include "dwgui/dwassign.h"
#include "dwcore/dwlocks.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAssignLib : public DwRttiObject
{
    DW_DECLARE_CLASS(DwAssignLib);
public:
    DwAssignLib();
    virtual ~DwAssignLib();

    void setup();
    void discard();
    bool isValid() const;

    void setAssign(const DwAssign& assign);
    bool hasAssign(const DwString& assignName) const;
    DwString assignOf(const DwString& assignName) const;
    void clearAssign(const DwString& assignName);
    DwVector<DwAssign> assigns() const;
    DwUrl resolveAssigns(const DwUrl& url) const;
    DwString resolveAssignsInString(const DwString& urlString) const;

    static void initAssignLib();
    static void unInitAssignLib();

private:
    void setupSystemAssigns();
    void setupProjectAssigns();

    bool m_isValid;
    // TODO: USE HASH MAP
    DwMap<DwString, DwString> m_assignTable;
    DwCriticalSection m_criticalSection;
};// end of DwAssignLib

//------------------------------------------------------------------------------
DW_GUI_EXPORT DwAssignLib* dwAssignLib();