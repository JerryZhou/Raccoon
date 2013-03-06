#include "stable.h"
#include "dwtextformat.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwTextFormat, 'TFAT', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwTextFormat::DwTextFormat()
    : m_autoLineBreak (false)
    , m_lineSpacing(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwTextFormat::setAutoLineBreak(bool b)
{
    m_autoLineBreak = b;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextFormat::isAutoLineBreak() const
{
    return m_autoLineBreak;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextFormat::lineSpacing() const
{
    return m_lineSpacing;
}

//------------------------------------------------------------------------------
/**
*/
void DwTextFormat::setLineSpacing(int spacing)
{
    m_lineSpacing = spacing;
}