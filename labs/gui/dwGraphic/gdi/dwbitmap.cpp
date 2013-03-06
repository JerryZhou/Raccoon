#include "stable.h"
#include "dwbitmap.h"
#include "dwbmpfile.h"

class DwBitmapPrivate : public DwSharedData
{
public:
	DwBitmapPrivate();
	~DwBitmapPrivate(){};
	bool CreateBitmap(const DwString &file);
	bool CreateBitmap(const int nWidth, const int nHeight);
	void clear();
	DwBitmapPrivate *clone();

	DwBmpFile m_bmpFile;
	HBITMAP m_hBitmap;
	LPBYTE	m_lpByte;
	int   m_nWidth;
	int   m_nHeight;
};

DwBitmapPrivate::DwBitmapPrivate()
:m_hBitmap(NULL)
,m_lpByte(NULL)
,m_nHeight(0)
,m_nWidth(0)
{
	

}

void DwBitmapPrivate::clear()
{	
	m_hBitmap = NULL;
    m_lpByte = NULL;//是否有清空？
	m_nWidth  = 0;
	m_nHeight = 0;
}

bool DwBitmapPrivate::CreateBitmap(const DwString &file)
{
	DW_ASSERT(!file.isEmpty());
	if(!m_bmpFile.loadFile(file))
	{
		return false;
	}

	BITMAPINFO bmi = m_bmpFile.bmi();
	BYTE *bits = m_bmpFile.bits();
	m_hBitmap = ::CreateDIBSection(NULL,&bmi,DIB_RGB_COLORS,(void **)&m_lpByte,NULL,0);

	if (m_hBitmap)
	{
		return true;
	}
	return false;
}

bool  DwBitmapPrivate::CreateBitmap(const int nWidth, const int nHeight)
{
	m_nWidth  = nWidth;
	m_nHeight = nHeight;


	BITMAPINFO bmi = {0};
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = nWidth;
	bmi.bmiHeader.biHeight = nHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biSizeImage = nWidth * nHeight * 4;

	m_hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (VOID**)&m_lpByte, NULL, 0);
	if (m_hBitmap)
	{
		return true;
	}
	return false;
}

DwBitmapPrivate * DwBitmapPrivate::clone()
{
	//待实现
	return NULL;
}

DwBitmap::DwBitmap()
{
	d = new DwBitmapPrivate;
   
}

DwBitmap::~DwBitmap()
{

}

bool DwBitmap::createBitmap(const DwString &file)
{
	return d->CreateBitmap(file);
}

bool DwBitmap::createBitmap(int nWidth, int nHeight)
{
	return d->CreateBitmap(nWidth,nHeight);
}

unsigned int  DwBitmap::handle() const
{
	return (unsigned int)d->m_hBitmap;
}

int DwBitmap::width() const
{
	return d->m_nWidth;
}

int DwBitmap::height() const
{
	return d->m_nHeight;
}

const uint8_t* DwBitmap::bits()  const
{
	return static_cast<uint8_t*>(d->m_lpByte);

}

void DwBitmap::clear()
{
	d->clear();
}
