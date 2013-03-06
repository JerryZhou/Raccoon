#pragma once
#include "dwpaintdevice.h"

class DwBitmap;
class DwCanvasPrivate;
class DW_GRAPHICS_EXPORT DwCanvas:public DwPaintDevice
{
public:
	DwCanvas();
	~DwCanvas();

	virtual HDC getDC()const;
	virtual void releaseDC();

	HDC createDC(int width, int height);
	const DwBitmap* bitmap() const;

	void  clear();
private:
	DwExplicitlySharedDataPointer<DwCanvasPrivate> d;
};