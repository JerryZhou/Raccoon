#pragma once

#include <sstream>

enum DwMessageType { DwDebugMsg, DwWarningMsg, DwCriticalMsg, DwFatalMsg, DwSystemMsg };

const uint16_t KSpace[] = L" ";

class DW_CORE_EXPORT DwDebug
{
public:
    DwDebug(DwMessageType t = DwDebugMsg);
    ~DwDebug();

    inline DwDebug &operator<<(bool t)
    {
        m_stream << (t ? L"true" : L"false") << KSpace;
        return *this;
    }
    inline DwDebug &operator<<(char t)
    {
        m_stream << t;
        return *this;
    }

    template<typename T>
    inline DwDebug &operator<<(const T &t)
    {
        m_stream << t << KSpace;
        return *this;
    }

    template<typename T>
    inline DwDebug &operator>>(T &t)
    {
        DW_ASSERT(false);
        DW_UNUSED(t);
        return *this;
    }

private:
    DwMessageType m_type;
    std::wstringstream m_stream;
};

#define dwDebug() DwDebug()