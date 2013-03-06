#pragma once
#include "dweventid.h"
#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class DwGraphicsView;
class DwGraphicsItem;

// NB! DWEVENT AND SUBCLASS MUST SUPPORT THE COPY AND ASSIGN CONSTUCTOR
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwEvent : public DwRttiObject
{
    DW_DECLARE_EVENTID(DwEvent);
public:
    DwEvent();
    virtual ~DwEvent();

    inline void setAccepted(bool accepted);
    inline bool isAccepted() const;

    inline void accept();
    inline void ignore();

    inline void setUp(bool up);
    inline bool isUp() const;

    inline void setBacktrace(bool need);
    inline bool isBacktrace() const;

    inline void setTraced(bool traced);
    inline bool isTraced() const;

    inline void setMarked(bool mark);
    inline bool isMarked() const;

    bool isA(const DwEventId *) const;

protected:
    uint16_t m_accept : 1;
    uint16_t m_dir : 1;
    uint16_t m_backtrace : 1;
    uint16_t m_traced : 1;
    uint16_t m_reserved : 12;
private:
};// end of DwEvent

//------------------------------------------------------------------------------
/**
*/
inline void DwEvent::setAccepted(bool accepted)
{
    m_accept = accepted;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwEvent::isAccepted() const
{
    return m_accept;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwEvent::accept()
{
    m_accept = true;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwEvent::ignore()
{
    m_accept = false;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwEvent::setUp(bool up)
{
    m_dir = !up;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwEvent::isUp() const
{
    return !m_dir;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwEvent::setBacktrace(bool need)
{
    m_backtrace = need;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwEvent::isBacktrace() const
{
    return m_backtrace;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwEvent::setTraced(bool traced)
{
    m_traced = traced;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwEvent::isTraced() const
{
    return m_traced;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwEvent::setMarked(bool mark)
{
    m_reserved = mark;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwEvent::isMarked() const
{
    return !!(m_reserved);
}