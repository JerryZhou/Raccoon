#pragma once
#include "dwgui/dwrtti.h"
#include "dwgui/dwkerneltype.h"
#include "dwcore/dwdebug.h"

// THE RTTIOBJECT
// CAN USING WEAK PTR,
// CAN DUMPING DEBUG INFORMATION
// CAN DELTETED LATER
// OVERLOADED NEW AND FREE
// CAN USING RTTI SYSTEM
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwRttiObject
{
    DW_DECLARE_CLASS(DwRttiObject);
public:
    // default constructor, all rttiobject need a default constructor
    DwRttiObject();
    // copy constructor
    DwRttiObject(const DwRttiObject& other);
    // default destructor
    virtual ~DwRttiObject();
    // when dump the objects , will output related information
    virtual void debugString(DwDebug &stream)const;
    // implementation of delay delete
    void deleteLater();

    // return true if this object is instance of given class by rtti
    bool isA(const DwRtti *) const;
    // return true if this object is instance of given class by rtti
    bool isA(const DwRtti &) const;
    // return true if this object is instance of given class by string
    bool isA(const DwString &) const;
    // return true if this object is instance of given class by fourcc
    bool isA(const DwFourCC &) const;

    // dumping all of current alive rtti objects information
    static void dumpObjects();
    // dumping specified class of object
    static DwVector<DwRttiObject *> dumpObjectsOf(DwRtti *);
    // will call in app constructor
    static void initRttiObjects();
    // will call in app destructor
    static void unInitRttiObjects();
};// end of class DwRttiObject

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline T* dwsafe_cast(const DwRttiObject *obj)
{
    if(obj && obj->isA(T::RTTI()))
    {
        return (T*)(obj);
    }
    return NULL;
}