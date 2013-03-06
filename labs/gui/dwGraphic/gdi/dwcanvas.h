#pragma once

class DwBitmap;
class DwCanvasPrivate;
class DwCanvas
{
public:
	DwCanvas();
	~DwCanvas();

	bool create(int nWidth, int nHeight);
	void destroy();

	void filterMode(LPFILTERMODE pFilter) const;
	void setFilterMode(FILTERMODE filter);

	void detachBitmap(DwBitmap *pBitmap) const;
	void * memHDc();
	void resetData(); 
private:
	DwExplicitlySharedDataPointer<DwCanvasPrivate> d;
};