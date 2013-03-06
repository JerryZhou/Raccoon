#include "stable.h"

#include <dwmapi.h>

#include "tools/dwstring.h"
#include "tools/dwdir.h"
#include "dwwin32api.h"

namespace DwWinAPI
{
	DwString dwGetModuleFilePath(HMODULE h)
	{
		DwString path;
		WCHAR buf[MAX_PATH] = {0};
		DWORD dwSize = GetModuleFileName(h, buf, MAX_PATH - 1);
		if(GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			// auto ptr do not support array
			WCHAR *lp(new WCHAR[dwSize + 1]);
			GetModuleFileName(NULL, lp, dwSize);
			path = lp;
			DW_SAFE_DELETE_ARRAY(lp);
		}
		else
		{
			path = buf;
		}
		return path;
	}

	DwString dwGetModuleDir(HMODULE h)
	{
		DwString path = dwGetModuleFilePath(h);
		return DwDir::dirFromFilePath(path);
	}

	bool dwSetCurrentDir(const DwString &dir)
	{
		return SetCurrentDirectory(dir.constString()) ? true : false;
	}

	typedef HRESULT (WINAPI *DWMENABLEBLURBEHINDWINDOW)( HWND, const DWM_BLURBEHIND * );

	static FARPROC dwGetDllFunction(LPCWSTR dll, const char *fnName)
	{
		HMODULE h = LoadLibrary(dll);
		if(h)
		{
			return GetProcAddress(h, fnName);
		}

		return NULL;
	}

	bool dwDwmEnableBlurBehindWindow(HWND hWnd, HRGN hRgn)
	{
		static DWMENABLEBLURBEHINDWINDOW fn = NULL;
		if(!fn)
		{
			fn = (DWMENABLEBLURBEHINDWINDOW)dwGetDllFunction(L"dwmapi.dll", "DwmEnableBlurBehindWindow");
		}

		if(fn)
		{
			DWM_BLURBEHIND db;

			db.fEnable = hRgn ? TRUE : FALSE;
			db.hRgnBlur = hRgn;
			db.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
			db.fTransitionOnMaximized = FALSE;
			HRESULT hr = fn(hWnd, &db);

			return hr == S_OK;
		}

		return false;
	}
}
