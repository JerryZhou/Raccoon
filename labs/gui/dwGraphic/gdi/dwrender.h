#pragma once

class DwRenderPrivate;
class DwRender
{

public:
	DwRender();
	~DwRender();
	bool drawCanvas(int nwidth,int nHeight);
	BOOL drawTexture(HGTEXTURE htexture,int x, int y, SIZE sz);
	void  canvasToWindow(unsigned int hdc);

private:
	DwExplicitlySharedDataPointer<DwRenderPrivate> d;

};