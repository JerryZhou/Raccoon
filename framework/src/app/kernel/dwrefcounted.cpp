#include "stable.h"
#include "dwrefcounted.h"

DW_IMPLEMENT_CLASS(DwRefCounted, 'RCED', DwRttiObject)

//------------------------------------------------------------------------------
/**
*/
DwRefCounted::~DwRefCounted()
{
    DW_ASSERT(0 == this->m_refCount);
}