#pragma once
#include "dwbitmap.h"

class DwTexturePrivate;
class DwTexture
{

public:
	DwTexture();
	~DwTexture();

	HGTEXTURE createBitmap(const DwString &file);
	bool createBitmap(int nWidth, int nHeight);

	unsigned int handle() const;
	const uint8_t *bits() const;
	int     width() const;
	int     height() const;

	void filterMode(LPFILTERMODE lpFilter ) const;
    void setFilterMode(FILTERMODE filter);

	void drawMode(LPDRAWMODE lpDrawMode) const;
	void setDrawMode(DRAWMODE drawMode);
private:
	DwExplicitlySharedDataPointer<DwTexturePrivate> d;
};