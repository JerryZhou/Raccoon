#pragma once

class DwDrawMode
{
	DW_DISABLE_COPY(DwDrawMode);
public:
	DwDrawMode();
	~DwDrawMode();

	DrawMode drawMode() const;
	void setDrawMode(DrawMode drawMode);

private:
    DrawMode  m_drawMode;
};