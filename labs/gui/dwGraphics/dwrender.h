#pragma once

class DwRenderPrivate;
class DwRender
{

public:
	DwRender();
	~DwRender();
	bool drawCanvas(int nwidth,int height);
	BOOL drawTextureToMemDC(HGTEXTURE htexture,POINT pt);
	BOOL canvasToWindow(uint32_t hdc,int width,int height);

private:
	DwExplicitlySharedDataPointer<DwRenderPrivate> d;

};