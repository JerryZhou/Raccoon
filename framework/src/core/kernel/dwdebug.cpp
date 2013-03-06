#include "stable.h"

#include <iostream>
#include "dwdebug.h"

DwDebug::DwDebug( DwMessageType t )
    : m_type(t)
{
}

DwDebug::~DwDebug()
{
    std::wcout << L"Debug: " << m_stream.str() << std::endl;
    m_stream << L"\n";
    ::OutputDebugString(m_stream.str().c_str());
}
