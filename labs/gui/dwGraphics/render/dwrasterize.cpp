#include "stable.h"
#include "dwrasterize.h"


DwRasterize::DwRasterize()
{

}

DwRasterize::~DwRasterize()
{

}

const CompositionMode& DwRasterize::compositionMode() const
{
	return m_compositionMode;
}

void DwRasterize::setCompositionMode(const CompositionMode& compositionMode)
{
	m_compositionMode = compositionMode;
}