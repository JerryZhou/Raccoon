#include "stable.h"
#include "dwdrawmode.h"

DwDrawMode::DwDrawMode()
{

}

DwDrawMode::~DwDrawMode()
{

}

DrawMode DwDrawMode::drawMode() const
{
	return m_drawMode;
}

void DwDrawMode::setDrawMode(DrawMode drawMode)
{
	m_drawMode = drawMode;
}