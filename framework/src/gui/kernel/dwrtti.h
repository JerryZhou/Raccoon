#pragma once
#include "dwgui/dwwidgetenum.h"
#include "dwgui/dwfactory.h"
#include "dwgui/dwguiconfig.h"
#include "dwcore/dwstring.h"

//------------------------------------------------------------------------------
class DwRttiObject;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwRtti
{
public:
    /// define a creation callback function prototype
    typedef DwRttiObject* (*pfnCreator)();

    DwRtti(const char* className, DwFourCC fcc, pfnCreator creatorFunc, const DwRtti* parentClass, int instSize);
    DwRtti(const char* className, pfnCreator creatorFunc, const DwRtti* parentClass, int instSize);

    bool operator==(const DwRtti& rhs) const;
    bool operator!=(const DwRtti& rhs) const;

    inline DwString name() const;
    inline const char* cname() const;
    inline DwFourCC fourCC() const;
    inline const DwRtti* parent() const;
    inline int instanceSize() const;

    DwRttiObject* create() const;

    bool isDerivedFrom(const DwRtti& other) const;
    bool isDerivedFrom(const DwRtti* other) const;
    bool isDerivedFrom(const DwString& otherClassName) const;
    bool isDerivedFrom(const DwFourCC& otherClassFourCC) const;

    void* allocInstanceMemory();
    void freeInstanceMemory(void* ptr);

private:
    /// constructor method, called from the various constructors
    void construct(const char* className, DwFourCC fcc, pfnCreator creatorFunc, const DwRtti* parentClass, int instSize);

    const char* m_className;
    const DwRtti* m_parent;
    DwFourCC m_fourCC;
    pfnCreator m_creator;
    int m_instanceSize;
};// end of DwRtti

//------------------------------------------------------------------------------
/**
*/
inline 
DwString DwRtti::name() const
{
    return DwString(m_className);
}

//------------------------------------------------------------------------------
/**
*/
inline 
const char* DwRtti::cname() const
{
    return m_className;
}

//------------------------------------------------------------------------------
/**
*/
inline 
DwFourCC DwRtti::fourCC() const
{
    return m_fourCC;
}

//------------------------------------------------------------------------------
/**
*/
inline 
const DwRtti* DwRtti::parent() const
{
    return m_parent;
}

//------------------------------------------------------------------------------
/**
*/
inline int DwRtti::instanceSize() const
{
    return m_instanceSize;
}
#include "dwgui/dwrttimacro.h"
