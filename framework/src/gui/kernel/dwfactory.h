#pragma once
#include "dwgui/dwwidgetenum.h"
#include "dwcore/dwstring.h"
#include "dwcore/dwmap.h"
#include "dwcore/dwhash.h"
#include "dwcore/dwlocks.h"
#include "dwcore/dwvector.h"

//------------------------------------------------------------------------------
class DwRtti;
class DwRttiObject;
typedef int DwFourCC;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwFactory
{
public:
    DwFactory();
    ~DwFactory();

    void registerClass(const DwRtti* rtti, const DwString& className, const DwFourCC& classFourCC);
    void registerClass(const DwRtti* rtti, const DwString& className);

    bool classExists(const DwString& className) const;
    bool classExists(const DwFourCC classFourCC) const;

    const DwRtti* classRttiOf(const DwString& className) const;
    const DwRtti* classRttiOf(const DwFourCC& classFourCC) const;

    DwRttiObject* create(const DwString& className) const;
    DwRttiObject* create(const DwFourCC classFourCC) const;

    DwVector<DwRtti*> rttis()const;

private:
    DwHash<DwString, const DwRtti*> m_nameTable;     // for fast lookup by class name
    DwMap<DwFourCC, const DwRtti*> m_fourccTable;  // for fast lookup by fourcc code
    DwCriticalSection   m_criticalSection;
    
};// end of DwFactory
DW_GUI_EXPORT DwFactory* dwFactory();