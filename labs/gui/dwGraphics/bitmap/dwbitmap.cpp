#include "stable.h"
#include "dwbitmap.h"

DwBitmapPrivate::DwBitmapPrivate()
:m_hBitmap(NULL)
,m_lpByte(NULL)
,m_height(0)
,m_width(0)
{
	
}

DwBitmapPrivate::~DwBitmapPrivate()
{

}
bool DwBitmapPrivate::createBitmapFromFile(const DwString &filePath)
{
	if (!loadFile(filePath))
	{
		return false;
	}

	void* pBype = NULL;
	m_hBitmap = ::CreateDIBSection(NULL, &m_bitmapInfo, DIB_RGB_COLORS, &pBype, NULL, 0);
	if (m_hBitmap)
	{
		return true;
	}

	return false;
}

bool DwBitmapPrivate::createBitmapFromSize(int width, int height)
{
	DW_ASSERT(width > 0 && height > 0);
	m_width  = width;
	m_height = height;

	BITMAPINFO bmi = {0};
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biSizeImage = width * height * 4;

	void *pByte = NULL;
	m_hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (VOID**)&pByte, NULL, 0);
	if (m_hBitmap)
	{

		return true;
	}
	return false;
}

bool DwBitmapPrivate::createBitmapFromBuffer(const char *buffer, int size)
{
	//TODO...
	return false;
}

bool DwBitmapPrivate::loadFile(const DwString &filePathPath)
{
	DW_ASSERT(!filePathPath.isEmpty());
	if (!m_imageData.loadFile(filePathPath))
	{
		return false;
	}

	m_lpByte = (LPBYTE)m_imageData.bits();
	m_bitmapInfo = m_imageData.bmi();
	m_height = m_imageData.height();
	m_width  = m_imageData.width();
	return true;
}

DwBitmapPrivate * DwBitmapPrivate::clone()
{
	//待实现
	return NULL;
}

void DwBitmapPrivate::reset()
{	
	m_hBitmap = NULL;
	m_lpByte = NULL;//是否有清空？
	m_width  = 0;
	m_height = 0;
}

DwBitmap::DwBitmap()
{
	d = new DwBitmapPrivate;
}

DwBitmap::~DwBitmap()
{

}

bool DwBitmap::createBitmapFromFile(const DwString &filePath)
{
	return d->createBitmapFromFile(filePath);
}

bool DwBitmap::createBitmapFromSize(int width, int height)
{
	return d->createBitmapFromSize(width, height);
}

int DwBitmap::width() const
{
	return d->m_width;
}

int DwBitmap::height() const
{
	return d->m_height;
}

const HBITMAP  DwBitmap::handle() const
{
	return d->m_hBitmap;
}

const uint8_t* DwBitmap::bits()  const
{
	return static_cast<uint8_t*>(d->m_lpByte);
}

const BITMAPINFO DwBitmap::bitmapinfo() const
{
	return d->m_bitmapInfo;
}

void DwBitmap::reset()
{
	d->reset();
}
