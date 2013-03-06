#pragma once
#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class  DW_GUI_EXPORT DwRichChar
{
public:
    DwRichChar();
    explicit DwRichChar(wchar_t c);
    explicit DwRichChar(char c);

    wchar_t character() const;

protected:
    wchar_t m_c;
};// end of DwRichChar