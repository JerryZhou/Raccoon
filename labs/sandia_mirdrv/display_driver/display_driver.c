/******************************Module*Header*******************************\
*
*                           *******************
*                           * GDI SAMPLE CODE *
*                           *******************
*
* Module Name: enable.c
*
* This module contains the functions that enable and disable the
* driver, the pdev, and the surface.
*
* Copyright (c) 1992-1998 Microsoft Corporation
\**************************************************************************/
#define DBG 1

#include "driver.h"

#define RESOLUTION_X      800
#define RESOLUTION_Y      600

// The driver function table with all function index/address pairs

static DRVFN gadrvfn[] =
{
	{   INDEX_DrvEnablePDEV,            (PFN) DrvEnablePDEV         },
	{   INDEX_DrvCompletePDEV,          (PFN) DrvCompletePDEV       },
	{   INDEX_DrvDisablePDEV,           (PFN) DrvDisablePDEV        },
	{   INDEX_DrvEnableSurface,         (PFN) DrvEnableSurface      },
	{   INDEX_DrvDisableSurface,        (PFN) DrvDisableSurface     },
	{   INDEX_DrvResetPDEV,             (PFN) DrvResetPDEV          },
	{   INDEX_DrvAssertMode,            (PFN) DrvAssertMode         },
	{   INDEX_DrvNotify,                (PFN) DrvNotify             },
	{   INDEX_DrvCreateDeviceBitmap,    (PFN) DrvCreateDeviceBitmap },
	{   INDEX_DrvDeleteDeviceBitmap,    (PFN) DrvDeleteDeviceBitmap },
	{   INDEX_DrvTextOut,               (PFN) DrvTextOut            },
	{   INDEX_DrvBitBlt,                (PFN) DrvBitBlt             },
	{   INDEX_DrvCopyBits,              (PFN) DrvCopyBits           },
	{   INDEX_DrvStrokePath,            (PFN) DrvStrokePath         },
	{   INDEX_DrvLineTo,                (PFN) DrvLineTo             },
	{   INDEX_DrvFillPath,              (PFN) DrvFillPath           },
	{   INDEX_DrvStrokeAndFillPath,     (PFN) DrvStrokeAndFillPath  },
	{   INDEX_DrvStretchBlt,            (PFN) DrvStretchBlt         },
	{   INDEX_DrvSetPointerShape,       (PFN) DrvSetPointerShape    },
	{   INDEX_DrvMovePointer,           (PFN) DrvMovePointer        },
	{   INDEX_DrvGetModes,              (PFN) DrvGetModes           },
	{   INDEX_DrvAlphaBlend,            (PFN) DrvAlphaBlend         },
	{   INDEX_DrvTransparentBlt,        (PFN) DrvTransparentBlt     },
	{   INDEX_DrvGradientFill,          (PFN) DrvGradientFill       },
	{   INDEX_DrvPlgBlt,                (PFN) DrvPlgBlt             },
	{   INDEX_DrvStretchBltROP,         (PFN) DrvStretchBltROP      },
#if (NTDDI_VERSION >= NTDDI_VISTA)
	{   INDEX_DrvRenderHint,            (PFN) DrvRenderHint         },
#endif
	{   INDEX_DrvEscape,                (PFN) DrvEscape             }
};

//
// always hook these routines to ensure the mirrored driver
// is called for our surfaces
//

#define flGlobalHooks   HOOK_BITBLT|HOOK_TEXTOUT|HOOK_COPYBITS|HOOK_STROKEPATH|HOOK_LINETO|HOOK_FILLPATH|HOOK_STROKEANDFILLPATH|HOOK_STRETCHBLT|HOOK_ALPHABLEND|HOOK_TRANSPARENTBLT|HOOK_GRADIENTFILL|HOOK_PLGBLT|HOOK_STRETCHBLTROP

// Define the functions you want to hook for 8/16/24/32 pel formats

#define HOOKS_BMF8BPP 0

#define HOOKS_BMF16BPP 0

#define HOOKS_BMF24BPP 0

#define HOOKS_BMF32BPP 0

#define SYSTM_LOGFONT {16,7,0,0,700,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,VARIABLE_PITCH | FF_DONTCARE,L"System"}
#define HELVE_LOGFONT {12,9,0,0,400,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,VARIABLE_PITCH | FF_DONTCARE,L"MS Sans Serif"}
#define COURI_LOGFONT {12,9,0,0,400,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,FIXED_PITCH | FF_DONTCARE, L"Courier"}

// This is the basic devinfo for a default driver.  This is used as a base and customized based
// on information passed back from the miniport driver.

const DEVINFO gDevInfoFrameBuffer = {
	( GCAPS_OPAQUERECT
	| GCAPS_LAYERED
	), /* Graphics capabilities         */
	SYSTM_LOGFONT,    /* Default font description */
	HELVE_LOGFONT,    /* ANSI variable font description   */
	COURI_LOGFONT,    /* ANSI fixed font description          */
	0,                /* Count of device fonts          */
	0,                /* Preferred DIB format          */
	8,                /* Width of color dither          */
	8,                /* Height of color dither   */
	0                 /* Default palette to use for this device */
};

// This is default palette from Win 3.1

#define NUMPALCOLORS 256
#define NUMPALRESERVED 20

ULONG palColors[NUMPALCOLORS][4] =
{
	{ 0,   0,   0,   0  },  // 0
	{ 0x80,0,   0,   0  },  // 1
	{ 0,   0x80,0,   0  },  // 2
	{ 0x80,0x80,0,   0  },  // 3
	{ 0,   0,   0x80,0  },  // 4
	{ 0x80,0,   0x80,0  },  // 5
	{ 0,   0x80,0x80,0  },  // 6
	{ 0xC0,0xC0,0xC0,0  },  // 7

	{ 192, 220, 192, 0  },  // 8
	{ 166, 202, 240, 0  },  // 9
	{ 255, 251, 240, 0  },  // 10
	{ 160, 160, 164, 0  },  // 11

	{ 0x80,0x80,0x80,0  },  // 12
	{ 0xFF,0,   0,   0  },  // 13
	{ 0,   0xFF,0,   0  },  // 14
	{ 0xFF,0xFF,0,   0  },  // 15
	{ 0,   0,   0xFF,0  },  // 16
	{ 0xFF,0,   0xFF,0  },  // 17
	{ 0,   0xFF,0xFF,0  },  // 18
	{ 0xFF,0xFF,0xFF,0  }   // 19
};

VOID DbgPrint(PCHAR dbgMsg, ...)
{
	va_list ap;

	va_start(ap, dbgMsg);
	EngDebugPrint("[abcdebug]", dbgMsg, ap);
	va_end(ap);
}

/******************************Public*Routine******************************\
* bInitPDEV
*
* Determine the mode we should be in based on the DEVMODE passed in.
* For mirrored devices we don't bother querying the miniport.
*
\**************************************************************************/

BOOL bInitPDEV(
			   PPDEV ppdev,
			   DEVMODEW *pDevMode,
			   GDIINFO *pGdiInfo,
			   DEVINFO *pDevInfo)
{
	ULONG red, green, blue;
	INT i;

	//
	// Fill in the GDIINFO data structure with the information returned from
	// the kernel driver.
	//

	ppdev->ulMode = 0;
	ppdev->cxScreen = pDevMode->dmPelsWidth;
	ppdev->cyScreen = pDevMode->dmPelsHeight;
	ppdev->ulBitCount = pDevMode->dmBitsPerPel;
	ppdev->lDeltaScreen = 0;

	ppdev->flRed = 0x00FF0000;
	ppdev->flGreen = 0x000FF00;
	ppdev->flBlue = 0x00000FF;

	pGdiInfo->ulVersion    = GDI_DRIVER_VERSION;
	pGdiInfo->ulTechnology = DT_RASDISPLAY;
	pGdiInfo->ulHorzSize   = 0;
	pGdiInfo->ulVertSize   = 0;

	pGdiInfo->ulHorzRes        = ppdev->cxScreen;
	pGdiInfo->ulVertRes        = ppdev->cyScreen;
	pGdiInfo->ulPanningHorzRes = 0;
	pGdiInfo->ulPanningVertRes = 0;
	pGdiInfo->cBitsPixel       = 8;
	pGdiInfo->cPlanes          = 1; 
	pGdiInfo->ulVRefresh       = 1;       // not used
	pGdiInfo->ulBltAlignment   = 1;     // We don't have accelerated screen-
	//   to-screen blts, and any
	//   window alignment is okay

	pGdiInfo->ulLogPixelsX = pDevMode->dmLogPixels;
	pGdiInfo->ulLogPixelsY = pDevMode->dmLogPixels;

	pGdiInfo->flTextCaps = TC_RA_ABLE;

	pGdiInfo->flRaster = 0;           // flRaster is reserved by DDI

	pGdiInfo->ulDACRed   = 8;
	pGdiInfo->ulDACGreen = 8;
	pGdiInfo->ulDACBlue  = 8;

	pGdiInfo->ulAspectX    = 0x24;    // One-to-one aspect ratio
	pGdiInfo->ulAspectY    = 0x24;
	pGdiInfo->ulAspectXY   = 0x33;

	pGdiInfo->xStyleStep   = 1;       // A style unit is 3 pels
	pGdiInfo->yStyleStep   = 1;
	pGdiInfo->denStyleStep = 3;

	pGdiInfo->ptlPhysOffset.x = 0;
	pGdiInfo->ptlPhysOffset.y = 0;
	pGdiInfo->szlPhysSize.cx  = 0;
	pGdiInfo->szlPhysSize.cy  = 0;

	// RGB and CMY color info.

	pGdiInfo->ciDevice.Red.x = 6700;
	pGdiInfo->ciDevice.Red.y = 3300;
	pGdiInfo->ciDevice.Red.Y = 0;
	pGdiInfo->ciDevice.Green.x = 2100;
	pGdiInfo->ciDevice.Green.y = 7100;
	pGdiInfo->ciDevice.Green.Y = 0;
	pGdiInfo->ciDevice.Blue.x = 1400;
	pGdiInfo->ciDevice.Blue.y = 800;
	pGdiInfo->ciDevice.Blue.Y = 0;
	pGdiInfo->ciDevice.AlignmentWhite.x = 3127;
	pGdiInfo->ciDevice.AlignmentWhite.y = 3290;
	pGdiInfo->ciDevice.AlignmentWhite.Y = 0;

	pGdiInfo->ciDevice.RedGamma = 20000;
	pGdiInfo->ciDevice.GreenGamma = 20000;
	pGdiInfo->ciDevice.BlueGamma = 20000;

	pGdiInfo->ciDevice.Cyan.x = 0;
	pGdiInfo->ciDevice.Cyan.y = 0;
	pGdiInfo->ciDevice.Cyan.Y = 0;
	pGdiInfo->ciDevice.Magenta.x = 0;
	pGdiInfo->ciDevice.Magenta.y = 0;
	pGdiInfo->ciDevice.Magenta.Y = 0;
	pGdiInfo->ciDevice.Yellow.x = 0;
	pGdiInfo->ciDevice.Yellow.y = 0;
	pGdiInfo->ciDevice.Yellow.Y = 0;

	// No dye correction for raster displays.

	pGdiInfo->ciDevice.MagentaInCyanDye = 0;
	pGdiInfo->ciDevice.YellowInCyanDye = 0;
	pGdiInfo->ciDevice.CyanInMagentaDye = 0;
	pGdiInfo->ciDevice.YellowInMagentaDye = 0;
	pGdiInfo->ciDevice.CyanInYellowDye = 0;
	pGdiInfo->ciDevice.MagentaInYellowDye = 0;

	pGdiInfo->ulDevicePelsDPI = 0;   // For printers only
	pGdiInfo->ulPrimaryOrder = PRIMARY_ORDER_CBA;

	// Note: this should be modified later to take into account the size
	// of the display and the resolution.

	pGdiInfo->ulHTPatternSize = HT_PATSIZE_4x4_M;

	pGdiInfo->flHTFlags = HT_FLAG_ADDITIVE_PRIMS;

	// Fill in the basic devinfo structure

	*pDevInfo = gDevInfoFrameBuffer;

	// Fill in the rest of the devinfo and GdiInfo structures.
#if (NTDDI_VERSION >= NTDDI_VISTA)
	pDevInfo->flGraphicsCaps2 |= GCAPS2_INCLUDEAPIBITMAPS | GCAPS2_EXCLUDELAYERED;
#endif

	if (ppdev->ulBitCount == 8)
	{
		// It is Palette Managed.

		pGdiInfo->ulNumColors = 20;
		pGdiInfo->ulNumPalReg = 1 << ppdev->ulBitCount;

		pDevInfo->flGraphicsCaps |= (GCAPS_PALMANAGED | GCAPS_COLOR_DITHER );

		pGdiInfo->ulHTOutputFormat = HT_FORMAT_8BPP;
		pDevInfo->iDitherFormat = BMF_8BPP;

		// Assuming palette is orthogonal - all colors are same size.

		ppdev->cPaletteShift   = 8 - pGdiInfo->ulDACRed;
	}
	else
	{
		pGdiInfo->ulNumColors = (ULONG) (-1);
		pGdiInfo->ulNumPalReg = 0;

		if (ppdev->ulBitCount == 16)
		{
			pGdiInfo->ulHTOutputFormat = HT_FORMAT_16BPP;
			pDevInfo->iDitherFormat = BMF_16BPP;
		}
		else if (ppdev->ulBitCount == 24)
		{
			pGdiInfo->ulHTOutputFormat = HT_FORMAT_24BPP;
			pDevInfo->iDitherFormat = BMF_24BPP;
		}
		else
		{
			pGdiInfo->ulHTOutputFormat = HT_FORMAT_32BPP;
			pDevInfo->iDitherFormat = BMF_32BPP;
		}
	}


	pDevInfo->flGraphicsCaps |= (GCAPS_WINDINGFILL | GCAPS_GEOMETRICWIDE);

	// create remaining palette entries, simple loop to create uniformly
	// distributed color values.

	red = 0, green = 0, blue = 0;

	for (i = NUMPALRESERVED; i < NUMPALCOLORS; i++) {
		palColors[i][0] = red;
		palColors[i][1] = green;
		palColors[i][2] = blue;
		palColors[i][3] = 0;

		if (!(red += 32))
			if (!(green += 32))
				blue += 64;
	}

	if (ppdev->ulBitCount == 8)
	{
		pDevInfo->hpalDefault = ppdev->hpalDefault =

			EngCreatePalette(PAL_INDEXED,
			NUMPALCOLORS,     // cColors
			(ULONG*)&palColors[0],       // pulColors
			0,
			0,
			0);         // flRed, flGreen, flBlue [not used]
	}
	else
	{
		pDevInfo->hpalDefault = ppdev->hpalDefault =
			EngCreatePalette(PAL_BITFIELDS, 0,NULL,
			ppdev->flRed,ppdev->flBlue,ppdev->flGreen);
	}



	return(TRUE);
}

/******************************Public*Routine******************************\
* DrvEnableDriver
*
* Enables the driver by retrieving the drivers function table and version.
*
\**************************************************************************/

BOOL DrvEnableDriver(
					 ULONG iEngineVersion,
					 ULONG cj,
					 PDRVENABLEDATA pded)
{
	// Engine Version is passed down so future drivers can support previous
	// engine versions.  A next generation driver can support both the old
	// and new engine conventions if told what version of engine it is
	// working with.  For the first version the driver does nothing with it.

	iEngineVersion;

	DbgPrint("DrvEnableDriver:\n");

	// Fill in as much as we can.

	if (cj >= sizeof(DRVENABLEDATA))
		pded->pdrvfn = gadrvfn;

	if (cj >= (sizeof(ULONG) * 2))
		pded->c = sizeof(gadrvfn) / sizeof(DRVFN);

	// DDI version this driver was targeted for is passed back to engine.
	// Future graphic's engine may break calls down to old driver format.

	if (cj >= sizeof(ULONG))
		// DDI_DRIVER_VERSION is now out-dated. See winddi.h
		// DDI_DRIVER_VERSION_NT4 is equivalent to the old DDI_DRIVER_VERSION
		pded->iDriverVersion = DDI_DRIVER_VERSION_NT4;

	return(TRUE);
}

/******************************Public*Routine******************************\
* DrvEnablePDEV
*
* DDI function, Enables the Physical Device.
*
* Return Value: device handle to pdev.
*
\**************************************************************************/

DHPDEV
DrvEnablePDEV(
			  __in     DEVMODEW   *pDevmode,                // Pointer to DEVMODE
			  __in_opt PWSTR       pwszLogAddress,          // Logical address
			  __in     ULONG       cPatterns,               // number of patterns
			  __in_opt HSURF      *ahsurfPatterns,          // return standard patterns
			  __in     ULONG       cjGdiInfo,               // Length of memory pointed to by pGdiInfo
			  __out_bcount(cjGdiInfo)  ULONG    *pGdiInfo,  // Pointer to GdiInfo structure
			  __in     ULONG       cjDevInfo,               // Length of following PDEVINFO structure
			  __out_bcount(cjDevInfo)  DEVINFO  *pDevInfo,  // physical device information structure
			  __in_opt HDEV        hdev,                    // HDEV, used for callbacks
			  __in_opt PWSTR       pwszDeviceName,          // DeviceName - not used
			  __in     HANDLE      hDriver                  // Handle to base driver
			  )
{
	GDIINFO GdiInfo;
	DEVINFO DevInfo;
	PPDEV   ppdev = (PPDEV) NULL;

	DbgPrint("DrvEnablePDEV:\n");

	UNREFERENCED_PARAMETER(pwszLogAddress);
	UNREFERENCED_PARAMETER(cPatterns);
	UNREFERENCED_PARAMETER(ahsurfPatterns);
	UNREFERENCED_PARAMETER(hdev);
	UNREFERENCED_PARAMETER(pwszDeviceName);

	// Allocate a physical device structure.

	ppdev = (PPDEV) EngAllocMem(FL_ZERO_MEMORY, sizeof(PDEV), ALLOC_TAG);

	if (ppdev == (PPDEV) NULL)
	{
		DbgPrint("DISP DrvEnablePDEV failed EngAllocMem\n");
		return((DHPDEV) 0);
	}

	// Save the screen handle in the PDEV.

	ppdev->hDriver = hDriver;

	// Get the current screen mode information.  Set up device caps and devinfo.

	if (!bInitPDEV(ppdev, pDevmode, &GdiInfo, &DevInfo))
	{
		DbgPrint("DISP DrvEnablePDEV failed\n");
		goto error_free;
	}

	// Copy the devinfo into the engine buffer.

	// memcpy(pDevInfo, &DevInfo, min(sizeof(DEVINFO), cjDevInfo));
	if (sizeof(DEVINFO) > cjDevInfo)
	{
		DbgPrint("DISP DrvEnablePDEV failed: insufficient pDevInfo memory\n");
		goto error_free;
	}
	RtlCopyMemory(pDevInfo, &DevInfo, sizeof(DEVINFO));

	// Set the pdevCaps with GdiInfo we have prepared to the list of caps for this
	// pdev.

	//memcpy(pGdiInfo, &GdiInfo, min(cjGdiInfo, sizeof(GDIINFO)));
	if (sizeof(GDIINFO) > cjGdiInfo)
	{
		DbgPrint("DISP DrvEnablePDEV failed: insufficient pDevInfo memory\n");
		goto error_free;
	}
	RtlCopyMemory(pGdiInfo, &GdiInfo, sizeof(GDIINFO));

	ppdev->pVideoMemory = EngMapFile(L"\\??\\c:\\video.dat", RESOLUTION_X*RESOLUTION_Y*4, &ppdev->pMappedFile);

	return((DHPDEV) ppdev);

	// Error case for failure.
error_free:
	EngFreeMem(ppdev);
	return((DHPDEV) 0);
}

/******************************Public*Routine******************************\
* DrvCompletePDEV
*
* Store the HPDEV, the engines handle for this PDEV, in the DHPDEV.
*
\**************************************************************************/

VOID DrvCompletePDEV(
					 DHPDEV dhpdev,
					 HDEV  hdev)
{
	DbgPrint("DrvCompletePDEV:\n");
	((PPDEV) dhpdev)->hdevEng = hdev;
}

/******************************Public*Routine******************************\
* DrvDisablePDEV
*
* Release the resources allocated in DrvEnablePDEV.  If a surface has been
* enabled DrvDisableSurface will have already been called.
*
\**************************************************************************/

VOID DrvDisablePDEV(
					DHPDEV dhpdev)
{
	PPDEV ppdev = (PPDEV) dhpdev;

	DbgPrint("DrvDisablePDEV:\n");

	if (ppdev->pMappedFile)
	{
		EngUnmapFile(ppdev->pMappedFile);
		ppdev->pMappedFile = 0;
	}

	EngDeletePalette(ppdev->hpalDefault);

	EngFreeMem(dhpdev);
}

/******************************Public*Routine******************************\
* DrvEnableSurface
*
* Enable the surface for the device.  Hook the calls this driver supports.
*
* Return: Handle to the surface if successful, 0 for failure.
*
\**************************************************************************/

HSURF DrvEnableSurface(
					   DHPDEV dhpdev)
{
	PPDEV ppdev;
	HSURF hsurf;
	SIZEL sizl;
	ULONG ulBitmapType;
	FLONG flHooks;
	ULONG mirrorsize;
	MIRRSURF *mirrsurf;
	DHSURF dhsurf;

	// Create engine bitmap around frame buffer.

	DbgPrint("DrvEnableSurface:\n");

	ppdev = (PPDEV) dhpdev;

	ppdev->ptlOrg.x = 0;
	ppdev->ptlOrg.y = 0;

	sizl.cx = ppdev->cxScreen;
	sizl.cy = ppdev->cyScreen;

	if (ppdev->ulBitCount == 16)
	{
		ulBitmapType = BMF_16BPP;
		flHooks = HOOKS_BMF16BPP;
	}
	else if (ppdev->ulBitCount == 24)
	{
		ulBitmapType = BMF_24BPP;
		flHooks = HOOKS_BMF24BPP;
	}
	else
	{
		ulBitmapType = BMF_32BPP;
		flHooks = HOOKS_BMF32BPP;
	}

	flHooks |= flGlobalHooks;

	mirrorsize = (ULONG)(sizeof(MIRRSURF) + 
		ppdev->lDeltaScreen * sizl.cy);

	mirrsurf = (MIRRSURF *) EngAllocMem(FL_ZERO_MEMORY,
		mirrorsize,
		0x4D495252);
	if (!mirrsurf) {
		DbgPrint("DISP DrvEnableSurface failed EngAllocMem\n");
		return(FALSE);
	}


	dhsurf = (DHSURF) mirrsurf;

	hsurf = EngCreateDeviceSurface(dhsurf,
		sizl,
		ulBitmapType);

	if (hsurf == (HSURF) 0)
	{
		DbgPrint("DISP DrvEnableSurface failed EngCreateBitmap\n");
		return(FALSE);
	}

	//if (!EngAssociateSurface(hsurf, ppdev->hdevEng, flHooks))
	//{
	//	DbgPrint("DISP DrvEnableSurface failed EngAssociateSurface\n");
	//	EngDeleteSurface(hsurf);
	//	return(FALSE);
	//}

	EngModifySurface(hsurf, ppdev->hdevEng, flHooks, 0, dhsurf, ppdev->pVideoMemory, RESOLUTION_X * 4, NULL);

	ppdev->hsurfEng = (HSURF) hsurf;
	ppdev->pvTmpBuffer = (PVOID) dhsurf;

	mirrsurf->cx = ppdev->cxScreen;
	mirrsurf->cy = ppdev->cyScreen;
	mirrsurf->lDelta = ppdev->lDeltaScreen;
	mirrsurf->ulBitCount = ppdev->ulBitCount;
	mirrsurf->bIsScreen = TRUE;

	return(hsurf);
}

/******************************Public*Routine******************************\
* DrvNotify
*
* Receives notification on where the mirror driver is positioned.
* Also gets notified before drawing happens 
*
\**************************************************************************/

VOID DrvNotify(
			   SURFOBJ *pso,
			   ULONG iType,
			   PVOID pvData)
{
	UNREFERENCED_PARAMETER(pso);
	UNREFERENCED_PARAMETER(pvData);

	switch(iType)
	{
	case DN_DEVICE_ORIGIN:
		DbgPrint("DrvNotify: DN_DEVICE_ORIGIN (%d,%d)\n", ((POINTL*)pvData)->x, ((POINTL*)pvData)->y);
		break;
	case DN_DRAWING_BEGIN:
		DbgPrint("DrvNotify: DN_DRAWING_BEGIN\n");
		break;
	}
}

/******************************Public*Routine******************************\
* DrvDisableSurface
*
* Free resources allocated by DrvEnableSurface.  Release the surface.
*
\**************************************************************************/

VOID DrvDisableSurface(
					   DHPDEV dhpdev)
{
	PPDEV ppdev = (PPDEV) dhpdev;

	DbgPrint("DrvDisableSurface:\n");

	EngDeleteSurface( ppdev->hsurfEng );

	// deallocate MIRRSURF structure.

	EngFreeMem( ppdev->pvTmpBuffer );
}

BOOL DrvResetPDEV(
				  DHPDEV  dhpdevOld,
				  DHPDEV  dhpdevNew)
{
	UNREFERENCED_PARAMETER(dhpdevOld);
	UNREFERENCED_PARAMETER(dhpdevNew);

	DbgPrint("Mirror Driver DrvCopyBits: \n\n");

	return FALSE;
}

/******************************Public*Routine******************************\
* DrvCopyBits
*
\**************************************************************************/

BOOL DrvCopyBits(
				 OUT SURFOBJ *psoDst,
				 IN SURFOBJ *psoSrc,
				 IN CLIPOBJ *pco,
				 IN XLATEOBJ *pxlo,
				 IN RECTL *prclDst,
				 IN POINTL *pptlSrc
				 )
{
	INT cnt1 = 0, cnt2 = 0;

	UNREFERENCED_PARAMETER(pco);
	UNREFERENCED_PARAMETER(pxlo);
	UNREFERENCED_PARAMETER(prclDst);
	UNREFERENCED_PARAMETER(pptlSrc);

	DbgPrint("Mirror Driver DrvCopyBits: \n");

	if (psoSrc)
	{
		if (psoSrc->dhsurf)
		{
			MIRRSURF *mirrsurf = (MIRRSURF *)psoSrc->dhsurf;

			if (mirrsurf->bIsScreen) 
			{
				DbgPrint("From Mirror Screen ");
			}
			else
			{
				DbgPrint("From Mirror DFB ");
			}
			cnt1 ++;
		}
		else
		{
			DbgPrint("From DIB ");
		}
	}

	if (psoDst)
	{
		if (psoDst->dhsurf)
		{
			MIRRSURF *mirrsurf = (MIRRSURF *)psoDst->dhsurf;

			if (mirrsurf->bIsScreen) 
			{
				DbgPrint("to MirrorScreen ");
			}
			else
			{
				DbgPrint("to Mirror DFB ");
			}
			cnt2 ++;
		}
		else
		{
			DbgPrint("to DIB ");
		}
	}

	if (cnt1 && cnt2)
	{
		DbgPrint(" [Send Request Over Wire]\n");
	}
	else if (cnt1)
	{
		DbgPrint(" [Read Cached Bits, Or Pull Bits]\n");
	}
	else if (cnt2) 
	{
		DbgPrint(" [Push Bits/Compress]\n");
	}
	else
	{
		DbgPrint(" [What Are We Doing Here?]\n");
	}

	return TRUE;
}

/******************************Public*Routine******************************\
* DrvBitBlt
*
\**************************************************************************/

BOOL DrvBitBlt(
			   IN SURFOBJ *psoDst,
			   IN SURFOBJ *psoSrc,
			   IN SURFOBJ *psoMask,
			   IN CLIPOBJ *pco,
			   IN XLATEOBJ *pxlo,
			   IN RECTL *prclDst,
			   IN POINTL *pptlSrc,
			   IN POINTL *pptlMask,
			   IN BRUSHOBJ *pbo,
			   IN POINTL *pptlBrush,
			   IN ROP4 rop4
			   )
{
	INT cnt1 = 0, cnt2 = 0;

	UNREFERENCED_PARAMETER(psoMask);
	UNREFERENCED_PARAMETER(pco);
	UNREFERENCED_PARAMETER(pxlo);
	UNREFERENCED_PARAMETER(prclDst);
	UNREFERENCED_PARAMETER(pptlSrc);
	UNREFERENCED_PARAMETER(pptlMask);
	UNREFERENCED_PARAMETER(pbo);
	UNREFERENCED_PARAMETER(pptlBrush);
	UNREFERENCED_PARAMETER(rop4);

	DbgPrint("Mirror Driver DrvBitBlt (Mask=%08x, rop=%08x:\n", psoMask, rop4);

	if (psoSrc)
	{
		if (psoSrc->dhsurf)
		{
			MIRRSURF *mirrsurf = (MIRRSURF *)psoSrc->dhsurf;

			if (mirrsurf->bIsScreen) 
			{
				DbgPrint("From Mirror Screen ");
			}
			else
			{
				DbgPrint("From Mirror DFB ");
			}
			cnt1 ++;
		}
		else
		{
			DbgPrint("From DIB ");
		}
	}

	if (psoDst)
	{
		if (psoDst->dhsurf)
		{
			MIRRSURF *mirrsurf = (MIRRSURF *)psoDst->dhsurf;

			if (mirrsurf->bIsScreen) 
			{
				DbgPrint("to MirrorScreen ");
			}
			else
			{
				DbgPrint("to Mirror DFB ");
			}
			cnt2 ++;
		}
		else
		{
			DbgPrint("to DIB ");
		}
	}

	if (cnt1 && cnt2)
	{
		DbgPrint(" [Send Request Over Wire]\n");
	}
	else if (cnt1)
	{
		DbgPrint(" [Read Cached Bits, Or Pull Bits]\n");
	}
	else if (cnt2) 
	{
		DbgPrint(" [Push Bits/Compress]\n");
	}
	else
	{
		DbgPrint(" [What Are We Doing Here?]\n");
	}

	return TRUE;
}

BOOL DrvTextOut(
				IN SURFOBJ *psoDst,
				IN STROBJ *pstro,
				IN FONTOBJ *pfo,
				IN CLIPOBJ *pco,
				IN RECTL *prclExtra,
				IN RECTL *prclOpaque,
				IN BRUSHOBJ *pboFore,
				IN BRUSHOBJ *pboOpaque,
				IN POINTL *pptlOrg,
				IN MIX mix
				)
{
	UNREFERENCED_PARAMETER(psoDst);
	UNREFERENCED_PARAMETER(pstro);
	UNREFERENCED_PARAMETER(pfo);
	UNREFERENCED_PARAMETER(pco);
	UNREFERENCED_PARAMETER(prclExtra);
	UNREFERENCED_PARAMETER(prclOpaque);
	UNREFERENCED_PARAMETER(pboFore);
	UNREFERENCED_PARAMETER(pboOpaque);
	UNREFERENCED_PARAMETER(pptlOrg);
	UNREFERENCED_PARAMETER(mix);

	DbgPrint("Mirror Driver DrvTextOut: pwstr=%08x\n",
		pstro ? pstro->pwszOrg : (WCHAR*)-1);

	return TRUE;
}

BOOL
DrvStrokePath(SURFOBJ*   pso,
			  PATHOBJ*   ppo,
			  CLIPOBJ*   pco,
			  XFORMOBJ*  pxo,
			  BRUSHOBJ*  pbo,
			  POINTL*    pptlBrush,
			  LINEATTRS* pLineAttrs,
			  MIX        mix)
{
	UNREFERENCED_PARAMETER(pso);
	UNREFERENCED_PARAMETER(ppo);
	UNREFERENCED_PARAMETER(pco);
	UNREFERENCED_PARAMETER(pxo);
	UNREFERENCED_PARAMETER(pbo);
	UNREFERENCED_PARAMETER(pptlBrush);
	UNREFERENCED_PARAMETER(pLineAttrs);
	UNREFERENCED_PARAMETER(mix);

	DbgPrint("Mirror Driver DrvStrokePath:\n");

	return TRUE;
}

BOOL DrvLineTo(
			   SURFOBJ   *pso,
			   CLIPOBJ   *pco,
			   BRUSHOBJ  *pbo,
			   LONG       x1,
			   LONG       y1,
			   LONG       x2,
			   LONG       y2,
			   RECTL     *prclBounds,
			   MIX        mix)
{
	UNREFERENCED_PARAMETER(pso);
	UNREFERENCED_PARAMETER(pco);
	UNREFERENCED_PARAMETER(pbo);
	UNREFERENCED_PARAMETER(x1);
	UNREFERENCED_PARAMETER(y1);
	UNREFERENCED_PARAMETER(x2);
	UNREFERENCED_PARAMETER(y2);
	UNREFERENCED_PARAMETER(prclBounds);
	UNREFERENCED_PARAMETER(mix);

	DbgPrint("Mirror Driver DrvLineTo: \n");
	return TRUE;
}



BOOL DrvFillPath(
				 SURFOBJ  *pso,
				 PATHOBJ  *ppo,
				 CLIPOBJ  *pco,
				 BRUSHOBJ *pbo,
				 PPOINTL   pptlBrushOrg,
				 MIX       mix,
				 FLONG     flOptions)
{
	UNREFERENCED_PARAMETER(pso);
	UNREFERENCED_PARAMETER(ppo);
	UNREFERENCED_PARAMETER(pco);
	UNREFERENCED_PARAMETER(pbo);
	UNREFERENCED_PARAMETER(pptlBrushOrg);
	UNREFERENCED_PARAMETER(mix);
	UNREFERENCED_PARAMETER(flOptions);

	DbgPrint("Mirror Driver DrvFillPath: \n");
	return TRUE;   
}

BOOL DrvStrokeAndFillPath(
						  SURFOBJ*   pso,
						  PATHOBJ*   ppo,
						  CLIPOBJ*   pco,
						  XFORMOBJ*  pxo,
						  BRUSHOBJ*  pboStroke,
						  LINEATTRS* plineattrs,
						  BRUSHOBJ*  pboFill,
						  POINTL*    pptlBrushOrg,
						  MIX        mixFill,
						  FLONG      flOptions)
{
	UNREFERENCED_PARAMETER(pso);
	UNREFERENCED_PARAMETER(ppo);
	UNREFERENCED_PARAMETER(pco);
	UNREFERENCED_PARAMETER(pxo);
	UNREFERENCED_PARAMETER(pboStroke);
	UNREFERENCED_PARAMETER(plineattrs);
	UNREFERENCED_PARAMETER(pboFill);
	UNREFERENCED_PARAMETER(pptlBrushOrg);
	UNREFERENCED_PARAMETER(mixFill);
	UNREFERENCED_PARAMETER(flOptions);

	DbgPrint("Mirror Driver DrvStrokeAndFillPath: \n");
	return TRUE;
}

BOOL DrvTransparentBlt(
					   SURFOBJ*    psoDst,
					   SURFOBJ*    psoSrc,
					   CLIPOBJ*    pco,
					   XLATEOBJ*   pxlo,
					   RECTL*      prclDst,
					   RECTL*      prclSrc,
					   ULONG       iTransColor,
					   ULONG       ulReserved)
{
	UNREFERENCED_PARAMETER(psoDst);
	UNREFERENCED_PARAMETER(psoSrc);
	UNREFERENCED_PARAMETER(pco);
	UNREFERENCED_PARAMETER(pxlo);
	UNREFERENCED_PARAMETER(prclDst);
	UNREFERENCED_PARAMETER(prclSrc);
	UNREFERENCED_PARAMETER(iTransColor);
	UNREFERENCED_PARAMETER(ulReserved);

	DbgPrint("Mirror Driver DrvTransparentBlt: \n");
	return TRUE;
}


BOOL DrvAlphaBlend(
				   SURFOBJ*            psoDst,
				   SURFOBJ*            psoSrc,
				   CLIPOBJ*            pco,
				   XLATEOBJ*           pxlo,
				   RECTL*              prclDst,
				   RECTL*              prclSrc,
				   BLENDOBJ*           pBlendObj)
{
	UNREFERENCED_PARAMETER(psoDst);
	UNREFERENCED_PARAMETER(psoSrc);
	UNREFERENCED_PARAMETER(pco);
	UNREFERENCED_PARAMETER(pxlo);
	UNREFERENCED_PARAMETER(prclDst);
	UNREFERENCED_PARAMETER(prclSrc);
	UNREFERENCED_PARAMETER(pBlendObj);

	DbgPrint("Mirror Driver DrvAlphaBlend: \n");



	return TRUE;
}

BOOL DrvGradientFill(
					 SURFOBJ*            pso,
					 CLIPOBJ*            pco,
					 XLATEOBJ*           pxlo,
					 TRIVERTEX*          pVertex,
					 ULONG               nVertex,
					 PVOID               pMesh,
					 ULONG               nMesh,
					 RECTL*              prclExtents,
					 POINTL*             pptlDitherOrg,
					 ULONG               ulMode)
{
	UNREFERENCED_PARAMETER(pso);
	UNREFERENCED_PARAMETER(pco);
	UNREFERENCED_PARAMETER(pxlo);
	UNREFERENCED_PARAMETER(pVertex);
	UNREFERENCED_PARAMETER(nVertex);
	UNREFERENCED_PARAMETER(pMesh);
	UNREFERENCED_PARAMETER(nMesh);
	UNREFERENCED_PARAMETER(prclExtents);
	UNREFERENCED_PARAMETER(pptlDitherOrg);
	UNREFERENCED_PARAMETER(ulMode);

	DbgPrint("Mirror Driver DrvGradientFill: \n");
	return TRUE;
}

BOOL DrvStretchBlt(
				   SURFOBJ*            psoDst,
				   SURFOBJ*            psoSrc,
				   SURFOBJ*            psoMsk,
				   CLIPOBJ*            pco,
				   XLATEOBJ*           pxlo,
				   COLORADJUSTMENT*    pca,
				   POINTL*             pptlHTOrg,
				   RECTL*              prclDst,
				   RECTL*              prclSrc,
				   POINTL*             pptlMsk,
				   ULONG               iMode)
{
	/*
	UNREFERENCED_PARAMETER(psoDst);
	UNREFERENCED_PARAMETER(psoSrc);
	UNREFERENCED_PARAMETER(psoMsk);
	UNREFERENCED_PARAMETER(pco);
	UNREFERENCED_PARAMETER(pxlo);
	UNREFERENCED_PARAMETER(pca);
	UNREFERENCED_PARAMETER(pptlHTOrg);
	UNREFERENCED_PARAMETER(prclDst);
	UNREFERENCED_PARAMETER(prclSrc);
	UNREFERENCED_PARAMETER(pptlMsk);
	UNREFERENCED_PARAMETER(iMode);
	*/

	DbgPrint("Mirror Driver DrvStretchBlt: \n");

	return EngStretchBlt(psoDst, psoSrc, psoMsk, pco, pxlo,
		pca, pptlHTOrg, prclDst, prclSrc, pptlMsk, iMode);
}

ULONG DrvSetPointerShape(
						 SURFOBJ  *pso,
						 SURFOBJ  *psoMask,
						 SURFOBJ  *psoColor,
						 XLATEOBJ *pxlo,
						 LONG     xHot,
						 LONG     yHot,
						 LONG     x,
						 LONG     y,
						 RECTL    *prcl,
						 FLONG    fl)
{
	UNREFERENCED_PARAMETER(pso);
	UNREFERENCED_PARAMETER(psoMask);
	UNREFERENCED_PARAMETER(psoColor);
	UNREFERENCED_PARAMETER(pxlo);
	UNREFERENCED_PARAMETER(xHot);
	UNREFERENCED_PARAMETER(yHot);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(prcl);
	UNREFERENCED_PARAMETER(fl);

	DbgPrint("Mirror Driver DrvSetPointerShape: \n");

	return SPS_ACCEPT_NOEXCLUDE;
}

VOID DrvMovePointer(
					SURFOBJ  *pso,
					LONG     x,
					LONG     y,
					RECTL    *prcl)
{
	DbgPrint("Mirror Driver DrvMovePointer: \n");

	if(pso)
	{
		EngMovePointer(pso, x, y, prcl);
	}

}

ULONG DrvGetModes(
				  HANDLE    hDriver,
				  ULONG     cjSize,
				  DEVMODEW  *pdm)
{
	ULONG ulBytesWritten = 0;
	ULONG ulBytesNeeded = sizeof(DEVMODEW);
	ULONG ulReturnValue;

	UNREFERENCED_PARAMETER(hDriver);
	UNREFERENCED_PARAMETER(cjSize);

	DbgPrint("Mirror Driver DrvGetModes: \n");

	if(pdm == NULL)
	{
		ulReturnValue =  ulBytesNeeded;
	}
	else
	{
		ulBytesWritten = sizeof(DEVMODEW);

		memset(pdm, 0, sizeof(DEVMODEW));
		memcpy(pdm->dmDeviceName, DLL_NAME, sizeof(DLL_NAME));

		pdm->dmSpecVersion   = DM_SPECVERSION;
		pdm->dmDriverVersion = DM_SPECVERSION;

		pdm->dmSize             = sizeof(DEVMODEW);
		pdm->dmBitsPerPel       = 32;
		pdm->dmPelsWidth        = RESOLUTION_X;
		pdm->dmPelsHeight       = RESOLUTION_Y;
		pdm->dmDisplayFrequency = 75;
		pdm->dmFields           = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFLAGS | DM_DISPLAYFREQUENCY;

		ulReturnValue = ulBytesWritten;
	}

	return ulReturnValue;
}

BOOL DrvStretchBltROP(
					  SURFOBJ         *psoTrg,
					  SURFOBJ         *psoSrc,
					  SURFOBJ         *psoMask,
					  CLIPOBJ         *pco,
					  XLATEOBJ        *pxlo,
					  COLORADJUSTMENT *pca,
					  POINTL          *pptlBrushOrg,
					  RECTL           *prclTrg,
					  RECTL           *prclSrc,
					  POINTL          *pptlMask,
					  ULONG            iMode,
					  BRUSHOBJ        *pbo,
					  ROP4            rop4)
{
	UNREFERENCED_PARAMETER(psoTrg);
	UNREFERENCED_PARAMETER(psoSrc);
	UNREFERENCED_PARAMETER(psoMask);
	UNREFERENCED_PARAMETER(pco);
	UNREFERENCED_PARAMETER(pxlo);
	UNREFERENCED_PARAMETER(pca);
	UNREFERENCED_PARAMETER(pptlBrushOrg);
	UNREFERENCED_PARAMETER(prclTrg);
	UNREFERENCED_PARAMETER(prclSrc);
	UNREFERENCED_PARAMETER(pptlMask);
	UNREFERENCED_PARAMETER(iMode);
	UNREFERENCED_PARAMETER(pbo);
	UNREFERENCED_PARAMETER(rop4);

	DbgPrint("Mirror Driver DrvStretchBltROP: \n");
	return TRUE; 
}

BOOL DrvPlgBlt(
			   SURFOBJ         *psoTrg,
			   SURFOBJ         *psoSrc,
			   SURFOBJ         *psoMsk,
			   CLIPOBJ         *pco,
			   XLATEOBJ        *pxlo,
			   COLORADJUSTMENT *pca,
			   POINTL          *pptlBrushOrg,
			   POINTFIX        *pptfx,
			   RECTL           *prcl,
			   POINTL          *pptl,
			   ULONG            iMode)
{
	UNREFERENCED_PARAMETER(psoTrg);
	UNREFERENCED_PARAMETER(psoSrc);
	UNREFERENCED_PARAMETER(psoMsk);
	UNREFERENCED_PARAMETER(pco);
	UNREFERENCED_PARAMETER(pxlo);
	UNREFERENCED_PARAMETER(pca);
	UNREFERENCED_PARAMETER(pptlBrushOrg);
	UNREFERENCED_PARAMETER(pptfx);
	UNREFERENCED_PARAMETER(prcl);
	UNREFERENCED_PARAMETER(pptl);
	UNREFERENCED_PARAMETER(iMode);

	DbgPrint("Mirror Driver DrvPlgBlt: \n");
	return TRUE;
}

HBITMAP DrvCreateDeviceBitmap(
							  IN DHPDEV dhpdev,
							  IN SIZEL sizl,
							  IN ULONG iFormat
							  )
{
	MIRRSURF *mirrsurf;
	ULONG mirrorsize;
	DHSURF dhsurf;
	ULONG stride;
	HSURF hsurf;

	PPDEV ppdev = (PPDEV) dhpdev;

	DbgPrint("CreateDeviceBitmap:\n");

	if (iFormat == BMF_1BPP || iFormat == BMF_4BPP)
	{
		return NULL;
	}

	// DWORD align each stride
	stride = (sizl.cx*(iFormat/8)+3);
	stride -= stride % 4;

	mirrorsize = (int)(sizeof(MIRRSURF) + stride * sizl.cy);

	mirrsurf = (MIRRSURF *) EngAllocMem(FL_ZERO_MEMORY,
		mirrorsize,
		0x4D495252);
	if (!mirrsurf) {
		DbgPrint("DISP DrvCreateDeviceBitmap failed EngAllocMem\n");
		return(FALSE);
	}

	dhsurf = (DHSURF) mirrsurf;

	hsurf = (HSURF) EngCreateDeviceBitmap(dhsurf,
		sizl,
		iFormat);

	if (hsurf == (HSURF) 0)
	{
		DbgPrint("DISP DrvCreateDeviceBitmap failed EngCreateBitmap\n");
		return(FALSE);
	}

	if (!EngAssociateSurface(hsurf, 
		ppdev->hdevEng, 
		flGlobalHooks))
	{
		DbgPrint("DISP DrvCreateDeviceBitmap failed EngAssociateSurface\n");
		EngDeleteSurface(hsurf);
		return(FALSE);
	}

	mirrsurf->cx = sizl.cx;
	mirrsurf->cy = sizl.cy;
	mirrsurf->lDelta = stride;
	mirrsurf->ulBitCount = iFormat;
	mirrsurf->bIsScreen = FALSE;

	return((HBITMAP)hsurf);
}

VOID DrvDeleteDeviceBitmap(
						   IN DHSURF dhsurf
						   )
{
	MIRRSURF *mirrsurf;

	DbgPrint("DeleteDeviceBitmap:\n");

	mirrsurf = (MIRRSURF *) dhsurf;

	EngFreeMem((PVOID) mirrsurf);
}

#if (NTDDI_VERSION >= NTDDI_VISTA)
LONG
DrvRenderHint(DHPDEV dhpdev,
			  ULONG  NotifyCode,
			  SIZE_T Length,
			  PVOID  Data)
{
	PPDEV ppdev = (PPDEV) dhpdev;
	PDRH_APIBITMAPDATA pData = (PDRH_APIBITMAPDATA)Data;

	UNREFERENCED_PARAMETER(ppdev);

	if (NotifyCode == DRH_APIBITMAP && Length && Data)
	{
		DbgPrint("DrvRenderHint(API Render: %08x, %lx)\n", pData->pso, pData->b);
	}

	return TRUE;
}
#endif

/******************************Public*Routine******************************\
* DrvAssertMode
*
* Enable/Disable the given device.
*
\**************************************************************************/

DrvAssertMode(DHPDEV  dhpdev,
			  BOOL    bEnable)
{
	PPDEV ppdev = (PPDEV) dhpdev;

	UNREFERENCED_PARAMETER(bEnable);
	UNREFERENCED_PARAMETER(ppdev);

	DbgPrint("DrvAssertMode(%lx, %lx)\n", dhpdev, bEnable);

	return TRUE;

}// DrvAssertMode()

/******************************Public*Routine******************************\
* DrvEscape
*
* We only handle WNDOBJ_SETUP escape. 
*
\**************************************************************************/

typedef struct _WndObjENUMRECTS
{
	ULONG c;
	RECTL arcl[100];
} WNDOBJENUMRECTS;

VOID
vDumpWndObjRgn(WNDOBJ *pwo)
{
	ULONG ulRet;

	ulRet = WNDOBJ_cEnumStart(pwo, CT_RECTANGLES, CD_RIGHTDOWN, 100);

	if (ulRet != 0xFFFFFFFF)
	{
		BOOL bMore;
		ULONG i;
		WNDOBJENUMRECTS enumRects;

		do
		{
			bMore = WNDOBJ_bEnum(pwo, sizeof(enumRects), &enumRects.c);

			for (i = 0; i < enumRects.c; i++)
			{
				DbgPrint("\nWNDOBJ_rect:[%d,%d][%d,%d]",
					enumRects.arcl[i].left,
					enumRects.arcl[i].top,
					enumRects.arcl[i].right,
					enumRects.arcl[i].bottom);

			}
		} while (bMore);
	}
}

VOID
WndObjCallback(WNDOBJ *pwo,
			   FLONG fl)
{
#if (NTDDI_VERSION < NTDDI_VISTA)
	UNREFERENCED_PARAMETER(pwo);
#endif

	if (fl & (WOC_RGN_CLIENT_DELTA |
		WOC_RGN_CLIENT |
		WOC_RGN_SURFACE_DELTA |
		WOC_RGN_SURFACE |
		WOC_CHANGED |
		WOC_DELETE |
		WOC_DRAWN |
		WOC_SPRITE_OVERLAP |
		WOC_SPRITE_NO_OVERLAP
#if (NTDDI_VERSION >= NTDDI_VISTA)
		| WOC_RGN_SPRITE
#endif 
		))
	{
		DbgPrint((0,"WndObjCallback: "));

		if (fl & WOC_RGN_CLIENT_DELTA) 
			DbgPrint("WOC_RGN_CLIENT_DELTA ");
		if (fl & WOC_RGN_CLIENT) 
			DbgPrint("WOC_RGN_CLIENT ");
		if (fl & WOC_RGN_SURFACE_DELTA) 
			DbgPrint("WOC_RGN_SURFACE_DELTA ");
		if (fl & WOC_RGN_SURFACE) 
			DbgPrint("WOC_RGN_SURFACE ");
		if (fl & WOC_CHANGED) 
			DbgPrint("WOC_CHANGED ");
		if (fl & WOC_DELETE) 
			DbgPrint("WOC_DELETE ");
		if (fl & WOC_DRAWN) 
			DbgPrint("WOC_DRAWN ");
		if (fl & WOC_SPRITE_OVERLAP) 
			DbgPrint("WOC_SPRITE_OVERLAP ");
		if (fl & WOC_SPRITE_NO_OVERLAP)
			DbgPrint("WOC_SPRITE_NO_OVERLAP ");
#if (NTDDI_VERSION >= NTDDI_VISTA)
		if (fl & WOC_RGN_SPRITE)
		{
			DbgPrint("WOC_RGN_SPRITE ");
			vDumpWndObjRgn(pwo);
		}
#endif
		DbgPrint("\n");
	}
}

ULONG
DrvEscape(SURFOBJ *pso,
		  ULONG iEsc,
		  ULONG cjIn,
		  PVOID pvIn,
		  ULONG cjOut,
		  PVOID pvOut)
{
	ULONG ulRet = 0;

	UNREFERENCED_PARAMETER(cjIn);
	UNREFERENCED_PARAMETER(pvIn);
	UNREFERENCED_PARAMETER(cjOut);
	UNREFERENCED_PARAMETER(pvOut);

	if (pso->dhsurf)
	{

		if (iEsc == WNDOBJ_SETUP)
		{
			WNDOBJ *pwo = NULL;

			DbgPrint("Attempt to create WndObj\n");

			pwo = EngCreateWnd(pso,
				*(HWND*)pvIn,
				WndObjCallback,
				WO_DRAW_NOTIFY |
				WO_RGN_CLIENT |
				WO_RGN_CLIENT_DELTA |
				WO_RGN_WINDOW |
				WO_SPRITE_NOTIFY
#if (NTDDI_VERSION >= NTDDI_VISTA)
				| WO_RGN_SPRITE
#endif
				,
				0);
			if (pwo != NULL)
			{
				DbgPrint("WndObj creat success\n");
				ulRet = 1;
			}
		}
	}

	return ulRet;
}
