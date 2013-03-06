#pragma once

class DwWindow
{
protected:
	virtual void OnDraw(HDC hDC);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void GetWndClassEx(WNDCLASSEX & wc);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HRESULT CommonMDIChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
		HMENU hMenu, int nWindowMenu);
	virtual LRESULT OnQueryNewPalette(void);
	virtual LRESULT OnPaletteChanged(HWND hWnd, WPARAM wParam);

public:
	DwWindow(void);
	virtual ~DwWindow(void);
	virtual bool CreateEx(DWORD dwExStyle, LPCTSTR lpszClass, LPCTSTR lpszName, DWORD dwStyle, 
		int x, int y, int nWidth, int nHeight, HWND hParent, HMENU hMenu, HINSTANCE hInst);

	bool RegisterClass(LPCTSTR lpszClass, HINSTANCE hInst);
	virtual WPARAM MessageLoop(void);
	BOOL ShowWindow(int nCmdShow) const;
	BOOL UpdateWindow(void) const;

	HWND  m_hWnd;
	HPALETTE m_hPalette;
	int		 m_nUpdateCount;
	bool    m_bMainWindow;
};


int MyMessageBox(HWND hWnd, const TCHAR * text, const TCHAR * caption, DWORD style, int iconid);
