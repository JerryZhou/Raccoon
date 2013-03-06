#pragma once

//------------------------------------------------------------------------------
class DwClipBoardData
{
public:
    DwClipBoardData();
    virtual ~DwClipBoardData();

    HWND clipBoardViewer;
    HWND nextClipboardViewer;
    DwEventSignalDispatch signalDisptach;
    static LRESULT WindowProc(HWND hWnd, UINT uMsg, LPARAM lParam, WPARAM wParam);

    HRESULT Clear();
    HRESULT GetText(DwString &txt);
    HRESULT SetText(const DwString &txt);
};// end of DwClipBoardData
DW_GLOBAL_STATIC(DwClipBoardData, _dw_ClipBoardData);

//------------------------------------------------------------------------------
/**
*/
DwClipBoardData::DwClipBoardData()
{
    clipBoardViewer = ::CreateWindowEx(0,_T("STATIC"),NULL,0, 0,0,0,0, NULL,NULL,NULL,NULL);
    ::SetWindowLongPtr(clipBoardViewer, GWLP_WNDPROC, (LONG_PTR) DwClipBoardData::WindowProc);
    ::SetWindowLongPtr(clipBoardViewer, GWLP_USERDATA, (LONG_PTR) this);
    nextClipboardViewer = SetClipboardViewer(clipBoardViewer);
}

//------------------------------------------------------------------------------
/**
*/
DwClipBoardData::~DwClipBoardData()
{
    ChangeClipboardChain(clipBoardViewer, nextClipboardViewer);
}

//------------------------------------------------------------------------------
/**
*/
HRESULT DwClipBoardData::Clear()
{
    HWND owner = clipBoardViewer;
    if (!::OpenClipboard(owner))
        return !S_OK;

    BOOL result = ::EmptyClipboard();
    ::CloseClipboard();

    return result ? S_OK : !S_OK;
}

//------------------------------------------------------------------------------
/**
*/
LRESULT DwClipBoardData::WindowProc(HWND hWnd, UINT uMsg, LPARAM lParam, WPARAM wParam)
{
    DwClipBoardData * d = (DwClipBoardData *)::GetWindowLongPtr(hWnd, GWLP_USERDATA);

    bool propagate = false;

    if (uMsg == WM_CHANGECBCHAIN)
    {
        if ((HWND)wParam == d->nextClipboardViewer)
            d->nextClipboardViewer = (HWND)lParam;
        else
            propagate = true;
    }
    else if (uMsg == WM_DRAWCLIPBOARD)
    {
        //emitChanged(QClipboard::Clipboard);
        //if (!ownsClipboard() && d->iData)
        // clean up the clipboard object if we no longer own the clipboard
        //d->releaseIData();
        propagate = true;
    }

    if (propagate && d->nextClipboardViewer)
    {
        ::SendMessage(d->nextClipboardViewer, uMsg, wParam, lParam);
    }

    return ::DefWindowProc(hWnd, uMsg, lParam, wParam);
}

//------------------------------------------------------------------------------
/**
*/
HRESULT DwClipBoardData::GetText( DwString &clipText )
{
    HWND owner = clipBoardViewer;
    if (!::OpenClipboard(owner))
        return !S_OK;

    if (!::IsClipboardFormatAvailable(CF_TEXT) && !::IsClipboardFormatAvailable(CF_UNICODETEXT))
        return !S_OK;

    HGLOBAL clipData = ::GetClipboardData(CF_UNICODETEXT);

    if (clipData == 0)
    {
        clipData = ::GetClipboardData(CF_TEXT);
        if (clipData != 0)
        {
            const char* cstr = (const char*)::GlobalLock(clipData);
            if (cstr)
            {
                clipText = DwString::fromUtf8(cstr);
                ::GlobalUnlock(clipData);
            }
        }
    }
    else
    {
        LPCTSTR lpStr = (LPCTSTR)::GlobalLock(clipData);
        if (lpStr)
        {
            clipText = DwString::fromWCharArray(lpStr);
            ::GlobalUnlock(clipData);
        }
    }

    ::CloseClipboard();
    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT DwClipBoardData::SetText( const DwString &txt )
{
    HWND owner = clipBoardViewer;
    if (!::OpenClipboard(owner))
        return !S_OK;

    bool result = false;
    if (txt.isEmpty())
    {
        result = true;
        ::EmptyClipboard();
        ::CloseClipboard();
    }
    else
    {
        if (!txt.isEmpty())
        {
            ::EmptyClipboard();
            LPTSTR  lptstrCopy;
            HGLOBAL hglbCopy;
            int cch = txt.length();
            // Allocate a global memory object for the text.

            hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE,
                                     (cch + 1) * sizeof(TCHAR));
            if (hglbCopy == NULL)
            {
                ::CloseClipboard();
                result = false;
            }
            else
            {
                // Lock the handle and copy the text to the buffer.
                lptstrCopy = (LPTSTR)(::GlobalLock(hglbCopy));
                memcpy(lptstrCopy, txt.constString(),
                       cch * sizeof(TCHAR));
                lptstrCopy[cch] = (TCHAR) 0;    // null character
                ::GlobalUnlock(hglbCopy);

                result =  (::SetClipboardData(CF_UNICODETEXT, hglbCopy) != NULL);
                ::CloseClipboard();
                result = true;
            }
        }
    }
    return result ? S_OK : !S_OK;
}
