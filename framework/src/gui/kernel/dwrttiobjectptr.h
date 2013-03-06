#pragma once
#include "dwgui/dwweakptr.h"

//------------------------------------------------------------------------------
class DwRttiObject;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwRRttiObjectPtr : public DwWeakPtr<DwRttiObject, DwRttiObject>
{
public:
    DwRRttiObjectPtr();
    DwRRttiObjectPtr(const DwRttiObject* ptr);
};// end of DwRRttiObjectPtr

//------------------------------------------------------------------------------
template<typename T>
class DwRttiObjectPtr : public DwRRttiObjectPtr
{
public:
    DwRttiObjectPtr();
    DwRttiObjectPtr(const T* ptr);

    inline T* operator->() const;

    inline T& operator*() const;

    inline operator T*() const;
};// end of DwRttiObjectPtr

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline DwRttiObjectPtr<T>::DwRttiObjectPtr()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline DwRttiObjectPtr<T>::DwRttiObjectPtr(const T* ptr) : DwRRttiObjectPtr(ptr)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline T* DwRttiObjectPtr<T>::operator->() const
{
    if (holderPtr)
    {
        return static_cast<T*>(holderPtr->ptr);
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline T& DwRttiObjectPtr<T>::operator*() const
{
    if (holderPtr)
    {
        return *(static_cast<T*>(holderPtr->ptr));
    }
    return *static_cast<T*>(NULL);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline DwRttiObjectPtr<T>::operator T*() const
{
    if (holderPtr)
    {
        return static_cast<T*>(holderPtr->ptr);
    }
    return NULL;
}