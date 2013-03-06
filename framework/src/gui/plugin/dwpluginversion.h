#pragma once
#include "dwgui/dwversion.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwPluginVersion : public DwVersion
{
public:
    DwPluginVersion();

    explicit DwPluginVersion(const DwVersion& other);

    bool isCompatibleWith(const DwPluginVersion &ver) const;
    bool isCompatibleWith(const DwVersion& ver) const;
protected:
private:
};// end of DwPluginVersion