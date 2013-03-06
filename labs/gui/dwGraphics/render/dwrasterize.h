#pragma once

class DwRasterize
{
public:
	DwRasterize();
	~DwRasterize();

	const CompositionMode& compositionMode() const;
	void setCompositionMode(const CompositionMode& compositionMode);


private:
	CompositionMode  m_compositionMode;
};