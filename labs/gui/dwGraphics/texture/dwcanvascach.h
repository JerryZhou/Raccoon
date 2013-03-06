#pragma once
#include "dwpaintdevice.h"
class DwCanvasCachPrivate;

class DwCanvasCach
{
private:
	DwCanvasCach();
	~DwCanvasCach();

public:
	static DwCanvasCach& getInstance();

	HDC  createCanvasDC(HWND hwnd) const;
	HDC  hwndTocanvasDC(HWND hwnd) const;
	//const DwBitmap* canvasbitmap() const;

	void  clear(); 
private:
	DwExplicitlySharedDataPointer<DwCanvasCachPrivate> d;
};