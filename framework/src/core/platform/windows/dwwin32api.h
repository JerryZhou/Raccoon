#pragma once

#include <dwcore/dwstring.h>

namespace DwWinAPI
{
	DW_CORE_EXPORT DwString dwGetModuleFilePath(HMODULE h);
	DW_CORE_EXPORT DwString dwGetModuleDir(HMODULE h);

	DW_CORE_EXPORT bool dwSetCurrentDir(const DwString &dir);

	DW_CORE_EXPORT bool dwDwmEnableBlurBehindWindow(HWND hWnd, HRGN hRgn);
}