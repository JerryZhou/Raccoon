#pragma once
#include "dwcore/dwtypes.h"
#include "dwgui/dwrtti.h"

// NB! DO NOT INHERIT
typedef DwRtti DwEventId;

#define DW_DECLARE_EVENTID(type) \
	DW_DECLARE_CLASS(type);\
    public:\
    static DwEventId* EventId() { return type::RTTI(); }\
    virtual DwEventId* eventId() const;\
    private:

#define DW_IMPLEMENT_ROOT_EVENTID(type) \
    DW_IMPLEMENT_CLASS(type, 0, DwRttiObject);\
    DwEventId* type::eventId() const { return type::RTTI(); }

#define DW_IMPLEMENT_EVENTID(type, basetype) \
    DW_IMPLEMENT_CLASS(type, 0, basetype); \
    DwEventId* type::eventId() const { return type::RTTI(); }

#define DW_IMPLEMENT_FOURCC_EVENTID(type, fourcc, basetype) \
    DW_IMPLEMENT_CLASS(type, fourcc, basetype); \
    DwEventId* type::eventId() const { return type::RTTI(); }