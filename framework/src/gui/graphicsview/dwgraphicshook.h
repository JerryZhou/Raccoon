#pragma once
#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsHook : public DwRttiObject
{
    DW_DECLARE_CLASS(DwGraphicsHook);
public:
    DwGraphicsHook(DwGraphicsHook *next = NULL);
    virtual ~DwGraphicsHook();

    void append(DwGraphicsHook *next);
    void setNext(DwGraphicsHook *next);
    DwGraphicsHook* last() const;
    DwGraphicsHook* next() const;

protected:
    DwGraphicsHook *m_next;
};// end of DwGraphicsHook