#pragma once

#include "dwglobalstatic.h"
#include "dwcore/dwmalloc.h"
#include "platform/windows/dwwin32api.h"

class DwApplicationData
{
public:
    DwApplicationData()
    {
        init();
    }

private:
    void init();

public:
    DwString m_applicationFilePath;
};

void DwApplicationData::init()
{
    m_applicationFilePath = DwWinAPI::dwGetModuleFilePath(NULL);
}
