#include "stable.h"
#include "dwbmpfile.h"


class DwBmpFilePrivate : public DwSharedData
{
	typedef enum 
	{
		DIB_1BPP,			//   2 color image, palette-based
		DIB_2BPP,			//   4 color image, palttte-based
		DIB_4BPP,			//  16 color image, palette-based
		DIB_4BPPRLE,		//  16 color image, palette-based, RLE compressed
		DIB_8BPP,			// 256 color image, palette-based 
		DIB_8BPPRLE,		// 256 color image, palette-based, RLE compressed

		DIB_16RGB555,		// 15 bit RGB color image, 5-5-5
		DIB_16RGB565,		// 16 bit RGB color image, 5-6-5, 1 bit unused
		DIB_24RGB888,		// 24 bit RGB color image, 8-8-8
		DIB_32RGB888,		// 32 bit RGB color image, 8-8-8, 8 bit unused

		DIB_32RGBA8888,		// 32 bit RGBA color image, 8-8-8-8

		DIB_16RGBbitfields,	// 16 bit RGB color image, non-standard bit masks, NT-only
		DIB_32RGBbitfields,	// 32 bit RGB color image, non-standard bit masks, NT-only

		DIB_JPEG,			// embedded JPEG image
		DIB_PNG				// embedded PNG image
	}  DIBFormat;

	typedef enum 
	{
		DIB_BMI_NEEDFREE   = 1,
		DIB_BMI_READONLY   = 2,
		DIB_BITS_NEEDFREE  = 4,
		DIB_BITS_READONLY  = 8	
	};

public:
	DwBmpFilePrivate();
	~DwBmpFilePrivate();

	bool LoadFile(const DwString &file);
	bool AttachDIB(BITMAPINFO * pDIB, uint8_t * pBits, int flags);
	void ReleaseDIB(void);			// release memory

	DIBFormat	 m_nImageFormat;    // pixel array format
	int			 m_Flags;		 	// DIB_BMI_NEEDFREE, ....
	BITMAPINFO   m_bmi;			// Bitmap Information Header + mask + color table
	uint8_t    * m_pBits;			// pixel array

	RGBTRIPLE  *    m_pRGBTRIPLE;		// OS/2 DIB color table within m_pBMI
	RGBQUAD   *    m_pRGBQUAD;		// V3,4,5 DIB color table within m_pBMI
	int			    m_nClrUsed;		// No of color in color table
	int			    m_nClrImpt;		// Real color used
	unsigned int  * m_pBitFields;		// 16, 32-bpp masks within m_pBMI

	int			 m_nWidth;			// image pixel width
	int			 m_nHeight;			// image pixel height, positive
	int			 m_nPlanes;			// plane count
	int			 m_nBitCount;		// bit per plane
	int			 m_nColorDepth;		// bit count * plane count
	int			 m_nImageSize;		// pixel array size

	// Precalculated values
	int			 m_nBPS;			// byte per scan line, per plane
	BYTE *       m_pOrigin;			// point to logical origin
	int			 m_nDelta;			// delta to next scan line
};

DwBmpFilePrivate::DwBmpFilePrivate()
{
	ZeroMemory(&m_bmi,sizeof(BITMAPINFO));
	m_pBits        = NULL;

	m_nWidth       = 0;
	m_nHeight      = 0;
	m_nPlanes      = 1;
	m_nBitCount    = 1;
	m_nColorDepth  = 1;
	m_nImageSize   = 0;
	m_nImageFormat = DIB_1BPP;

	m_pRGBTRIPLE   = NULL;
	m_pRGBQUAD	   = NULL;
}

DwBmpFilePrivate::~DwBmpFilePrivate()
{
	ReleaseDIB();
}

bool DwBmpFilePrivate::LoadFile(const DwString &filename)
{
	DW_ASSERT(!filename.isEmpty());
   
	if ( filename.isEmpty())
		return false;

	HANDLE handle = CreateFile((LPCWSTR)filename.constString(), GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if ( handle == INVALID_HANDLE_VALUE )
		return false;

	BITMAPFILEHEADER bmFH;

	DWORD  dwRead = 0;
	ReadFile(handle, & bmFH, sizeof(bmFH), & dwRead, NULL);


	if ( (bmFH.bfType == 0x4D42) && (bmFH.bfSize<=GetFileSize(handle, NULL)) )
	{
		BITMAPINFO * pDIB = (BITMAPINFO *) new BYTE[bmFH.bfSize];

		if ( pDIB )
		{
			ReadFile(handle, pDIB, bmFH.bfSize, & dwRead, NULL);
			CloseHandle(handle);
			m_bmi = *pDIB;

			return true;
		}

		//return AttachDIB(pDIB, NULL, DIB_BMI_NEEDFREE);
	}
	CloseHandle(handle);
	return false;
}

/*bool DwBmpFilePrivate::AttachDIB(BITMAPINFO * pDIB, BYTE * pBits, int flags)
{
    DW_ASSERT(pDIB);

	if ( IsBadReadPtr(pDIB, sizeof(BITMAPCOREHEADER)) )
		return false;

	//ReleaseDIB();

	m_pBMI  = pDIB;
	m_Flags = flags;

	unsigned int  size = * (unsigned int  *) pDIB; // always unsigned int  size, key to information header

	int compression;
	// gather information from bitmap information header structures
	switch ( size )
	{
	case sizeof(BITMAPCOREHEADER):
		{
			BITMAPCOREHEADER * pHeader = (BITMAPCOREHEADER *) pDIB;

			m_nWidth    = pHeader->bcWidth;
			m_nHeight   = pHeader->bcHeight;
			m_nPlanes   = pHeader->bcPlanes;
			m_nBitCount = pHeader->bcBitCount;
			m_nImageSize= 0;
			compression = BI_RGB;

			if ( m_nBitCount <= 8 )
			{
				m_nClrUsed   = 1 << m_nBitCount;
				m_nClrImpt   = m_nClrUsed;
				m_pRGBTRIPLE = (RGBTRIPLE *) ((BYTE *) m_pBMI + size);

				m_pBits      = (BYTE *) & m_pRGBTRIPLE[m_nClrUsed];
			}
			else
				m_pBits      = (BYTE *) m_pBMI + size;
			break;
		}

	case sizeof(BITMAPINFOHEADER):
	case sizeof(BITMAPV4HEADER):
	case sizeof(BITMAPV5HEADER):
		{
			BITMAPINFOHEADER * pHeader = & m_pBMI->bmiHeader;

			m_nWidth    = pHeader->biWidth;
			m_nHeight   = pHeader->biHeight;
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
					m_pBitFields = (unsigned int  *) ((BYTE *)pDIB+size);
					m_pRGBQUAD = (RGBQUAD *) ((BYTE *)pDIB+size + 3*sizeof(unsigned int ));
				}
				else
					m_pRGBQUAD = (RGBQUAD *) ((BYTE *)pDIB+size);

				m_pBits = (BYTE *) & m_pRGBQUAD[m_nClrUsed];
			}
			else
			{
				if ( compression==BI_BITFIELDS )
				{
					m_pBitFields = (unsigned int  *) ((BYTE *)pDIB+size);
					m_pBits      = (BYTE *) m_pBMI + size + 3 * sizeof(unsigned int );
				}
				else
					m_pBits      = (BYTE *) m_pBMI + size;
			}
			break;
		}

	default:
		return false;
	}

	if ( pBits )
		m_pBits = pBits;

	// precalculate information DIB parameters
	m_nColorDepth = m_nPlanes * m_nBitCount;
	m_nBPS		  = (m_nWidth * m_nBitCount + 31) / 32 * 4;

	if (m_nHeight < 0 ) // top-down bitmap
	{
		m_nHeight = - m_nHeight;	// change to positive
		m_nDelta  = m_nBPS;			// forward
		m_pOrigin = m_pBits;		// scan0 .. scanN-1
	}
	else
	{
		m_nDelta  = - m_nBPS;		// backward
		m_pOrigin = m_pBits + (m_nHeight-1) * m_nBPS * m_nPlanes;	// scanN-1..scan0
	}

	if ( m_nImageSize==0 )
		m_nImageSize = m_nBPS * m_nPlanes * m_nHeight;

	// convert compression mode to image format
	switch ( m_nBitCount )
	{
	case 0:
		if ( compression==BI_JPEG )
			m_nImageFormat = DIB_JPEG;
		else if ( compression==BI_PNG )
			m_nImageFormat = DIB_PNG;
		else
			return false;

	case 1:
		m_nImageFormat = DIB_1BPP;
		break;

	case 2:
		m_nImageFormat = DIB_2BPP;
		break;

	case 4:
		if ( compression==BI_RLE4 )
			m_nImageFormat = DIB_4BPPRLE;
		else
			m_nImageFormat = DIB_4BPP;
		break;

	case 8:
		if ( compression==BI_RLE8 )
			m_nImageFormat = DIB_8BPPRLE;
		else
			m_nImageFormat = DIB_8BPP;
		break;

	case 16:
		if ( compression==BI_BITFIELDS )
			m_nImageFormat = DIB_16RGBbitfields;
		else
			m_nImageFormat = DIB_16RGB555; // see below
		break;

	case 24:
		m_nImageFormat = DIB_24RGB888;
		break;

	case 32:
		if ( compression == BI_BITFIELDS )
			m_nImageFormat = DIB_32RGBbitfields;
		else
			m_nImageFormat = DIB_32RGB888; // see below
		break;

	default:
		return false;
	}

	// try to understand bit fields
	if ( compression==BI_BITFIELDS )
	{
		unsigned int  red   = m_pBitFields[0];
		unsigned int  green = m_pBitFields[1];
		unsigned int  blue  = m_pBitFields[2];

		if ((blue==0x001F) && (green==0x03E0) && (red==0x7C00) )
			m_nImageFormat = DIB_16RGB555;
		else if ( (blue==0x001F) && (green==0x07E0) && (red==0xF800) )
			m_nImageFormat = DIB_16RGB565;
		else if ( (blue==0x00FF) && (green==0xFF00) && (red==0xFF0000) )
			m_nImageFormat = DIB_32RGB888;
	}
	return true;
}
*/

void DwBmpFilePrivate::ReleaseDIB()
{
	if ( m_Flags & DIB_BITS_NEEDFREE )
	{
		delete [] m_pBits;
		m_Flags &= ~ DIB_BITS_NEEDFREE;
		m_pBits = NULL;
	}
}



DwBmpFile::DwBmpFile()
{
	d = new DwBmpFilePrivate;
}

DwBmpFile::~DwBmpFile()
{

}

bool DwBmpFile::loadFile(const DwString & file) 
{
   return d->LoadFile(file);
}

int DwBmpFile::width() const
{
	return d->m_nWidth;
}

int DwBmpFile::height() const
{
	return d->m_nHeight;
}

int DwBmpFile::depth() const
{
	return d->m_nColorDepth;
}
 
BITMAPINFO DwBmpFile::bmi() const
{
	return d->m_bmi;
}

uint8_t *DwBmpFile::bits() const
{
	return d->m_pBits;
}

void DwBmpFile::clear()
{
	d->ReleaseDIB();
}

