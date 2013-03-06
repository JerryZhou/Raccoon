#include "stable.h"
#include "bitmap/dwbitmap.h"
#include "dwimagedata.h"

//测试文件，后面将替换掉
class DwImageDataPrivate : public DwSharedData
{
	typedef enum 
	{
		DIB_BMI_NEEDFREE   = 1,
		DIB_BMI_READONLY   = 2,
		DIB_BITS_NEEDFREE  = 4,
		DIB_BITS_READONLY  = 8	
	};

public:
	DwImageDataPrivate();
	~DwImageDataPrivate();

	bool LoadFile(const DwString &filePath);
	bool AttachDIB(BITMAPINFO * pDIB, uint8_t * pBits, int flags);
	void ReleaseDIB(void);			// release memory
	
	int			 m_Flags;		 	// DIB_BMI_NEEDFREE, ....
	BITMAPINFO   m_bmi;		// Bitmap Information Header + mask + color table
	BITMAPINFO   *m_pbmi;
	uint8_t    * m_pBits;			// pixel array

	RGBTRIPLE  *    m_pRGBTRIPLE;		// OS/2 DIB color table within m_bmi
	RGBQUAD   *    m_pRGBQUAD;		// V3,4,5 DIB color table within m_bmi
	int			    m_nClrUsed;		// No of color in color table
	int			    m_nClrImpt;		// Real color used
	DWORD     * m_pBitFields;		// 16, 32-bpp masks within m_bmi

	int			 m_nBPS;
	int			 m_width;			// image pixel width
	int			 m_height;			// image pixel height, positive
	int			 m_nPlanes;			// plane count
	int			 m_nBitCount;		// bit per plane
	int			 m_nColorDepth;		// bit count * plane count
	int			 m_nImageSize;		// pixel array size
};

DwImageDataPrivate::DwImageDataPrivate()
{
	ZeroMemory(&m_bmi,sizeof(BITMAPINFO));
	m_pBits        = NULL;

	m_width       = 0;
	m_height      = 0;
	m_nPlanes      = 1;
	m_nBitCount    = 1;
	m_nColorDepth  = 1;
	m_nImageSize   = 0;
	m_pRGBTRIPLE   = NULL;
	m_pRGBQUAD	   = NULL;
}

DwImageDataPrivate::~DwImageDataPrivate()
{
	ReleaseDIB();
}

bool DwImageDataPrivate::LoadFile(const DwString &filename)
{
	DW_ASSERT(!filename.isEmpty());
	if ( filename.isEmpty())
		return false;

	HANDLE handle = CreateFile((LPCWSTR)filename.constString(), GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if ( handle == INVALID_HANDLE_VALUE )
		return false;

	BITMAPFILEHEADER bmFH = {0};
	DWORD  dwRead = 0;
	ReadFile(handle, & bmFH, sizeof(bmFH), & dwRead, NULL);
	if ((bmFH.bfType == 0x4D42) && (bmFH.bfSize<=GetFileSize(handle, NULL)))
	{
		BITMAPINFO * pDIB = (BITMAPINFO *) new BYTE[bmFH.bfSize];

		if (pDIB)
		{
			ReadFile(handle, pDIB, bmFH.bfSize, & dwRead, NULL);
			CloseHandle(handle);
			return AttachDIB(pDIB, NULL, DIB_BMI_NEEDFREE);
		}
	}
	CloseHandle(handle);
	return false;
}

bool DwImageDataPrivate::AttachDIB(BITMAPINFO * pDIB, BYTE * pBits, int flags)
{
	if ( IsBadReadPtr(pDIB, sizeof(BITMAPCOREHEADER)) )
		return false;

	ReleaseDIB();
	m_pbmi  = pDIB;
	m_Flags = flags;
	m_bmi   = *pDIB;
	DWORD size = * (DWORD *) pDIB; // always DWORD size, key to information header
	int compression;
	// gather information from bitmap information header structures
	switch ( size )
	{
	case sizeof(BITMAPCOREHEADER):
		{
			BITMAPCOREHEADER * pHeader = (BITMAPCOREHEADER *) pDIB;

			m_width    = pHeader->bcWidth;
			m_height   = pHeader->bcHeight;
			m_nPlanes   = pHeader->bcPlanes;
			m_nBitCount = pHeader->bcBitCount;
			m_nImageSize= 0;
			compression = BI_RGB;

			if ( m_nBitCount <= 8 )
			{
				m_nClrUsed   = 1 << m_nBitCount;
				m_nClrImpt   = m_nClrUsed;
				m_pRGBTRIPLE = (RGBTRIPLE *) ((BYTE *) m_pbmi + size);

				m_pBits      = (BYTE *) & m_pRGBTRIPLE[m_nClrUsed];
			}
			else
				m_pBits      = (BYTE *) m_pbmi + size;
			break;
		}

	case sizeof(BITMAPINFOHEADER):
	case sizeof(BITMAPV4HEADER):
	case sizeof(BITMAPV5HEADER):
		{
			BITMAPINFOHEADER * pHeader = & m_pbmi->bmiHeader;

			m_width    = pHeader->biWidth;
			m_height   = pHeader->biHeight;
			m_nPlanes   = pHeader->biPlanes;
			m_nBitCount = pHeader->biBitCount;
			m_nImageSize= pHeader->biSizeImage;
			compression = pHeader->biCompression;

			m_nClrUsed  = pHeader->biClrUsed;
			m_nClrImpt  = pHeader->biClrImportant;

			if ( m_nBitCount<=8 )
				if ( m_nClrUsed==0 )	// 0 means full color table
					m_nClrUsed = 1 << m_nBitCount;

			if ( m_nClrUsed )	// has a color table
			{
				if ( m_nClrImpt==0 )	// 0 means all important
					m_nClrImpt = m_nClrUsed;

				if ( compression==BI_BITFIELDS )
				{
					m_pBitFields = (DWORD *) ((BYTE *)pDIB+size);
					m_pRGBQUAD = (RGBQUAD *) ((BYTE *)pDIB+size + 3*sizeof(DWORD));
				}
				else
					m_pRGBQUAD = (RGBQUAD *) ((BYTE *)pDIB+size);

				m_pBits = (BYTE *) & m_pRGBQUAD[m_nClrUsed];
			}
			else
			{
				if ( compression==BI_BITFIELDS )
				{
					m_pBitFields = (DWORD *) ((BYTE *)pDIB+size);
					m_pBits      = (BYTE *) m_pbmi + size + 3 * sizeof(DWORD);
				}
				else
					m_pBits      = (BYTE *) m_pbmi + size;
			}
			break;
		}

	default:
		return false;
	}

	if ( pBits )
		m_pBits = pBits;

	return true;
}

void DwImageDataPrivate::ReleaseDIB()
{
	if (m_Flags & DIB_BITS_NEEDFREE )
	{
		delete [] m_pBits;
		m_Flags &= ~ DIB_BITS_NEEDFREE;
		m_pBits = NULL;
	}
}

DwImageData::DwImageData()
{
	d = new DwImageDataPrivate;
}

DwImageData::~DwImageData()
{

}

bool DwImageData::loadFile(const DwString & filePath) 
{
   return d->LoadFile(filePath);
}

int DwImageData::width() const
{
	return d->m_width;
}

int DwImageData::height() const
{
	return d->m_height;
}

int DwImageData::depth() const
{
	return d->m_nColorDepth;
}
 
const BITMAPINFO DwImageData::bmi() const
{
	return d->m_bmi;
}

uint8_t *DwImageData::bits() const
{
	return d->m_pBits;
}

void DwImageData::reset()
{
	d->ReleaseDIB();
}

