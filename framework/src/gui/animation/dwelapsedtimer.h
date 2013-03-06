#pragma once
#include "dwnamespace.h"
#include "dwcore/dwtypes.h"

class DW_GUI_EXPORT DwElapsedTimer
{
public:
    static Dw::ClockType clockType();
    static bool isMonotonic();

    void start();
    int64_t restart();
    void invalidate();
    bool isValid() const;

    int64_t elapsed() const;
    bool hasExpired(int64_t timeout) const;

    int64_t msecsSinceReference() const;
    int64_t msecsTo(const DwElapsedTimer &other) const;
    int64_t secsTo(const DwElapsedTimer &other) const;

    inline bool operator==(const DwElapsedTimer &other) const;
    inline bool operator!=(const DwElapsedTimer &other) const;

    friend bool operator<(const DwElapsedTimer &v1, const DwElapsedTimer &v2);
private:
    int64_t t1;
    int64_t t2;
};// end of DwElapsedTimer

//------------------------------------------------------------------------------
/**
*/
inline bool DwElapsedTimer::operator==(const DwElapsedTimer &other) const
{
    return t1 == other.t1 && t2 == other.t2;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwElapsedTimer::operator!=(const DwElapsedTimer &other) const
{
    return !(*this == other);
}