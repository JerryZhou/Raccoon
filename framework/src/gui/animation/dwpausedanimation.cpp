#include "stable.h"
#include "dwpausedanimation.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwPausedAnimation, 'PANN', DwAbstractAnimation);

//------------------------------------------------------------------------------
/**
*/
DwPausedAnimation::DwPausedAnimation(DwObject *parent/* = NULL*/)
    : DwAbstractAnimation(parent)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwPausedAnimation::DwPausedAnimation(int msecs, DwObject *parent/* = NULL*/)
    : DwAbstractAnimation(parent)
{
    setDuration(msecs);
}

//------------------------------------------------------------------------------
/**
*/
DwPausedAnimation::~DwPausedAnimation()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwPausedAnimation::updateCurrentTime(int)
{
    ;
}