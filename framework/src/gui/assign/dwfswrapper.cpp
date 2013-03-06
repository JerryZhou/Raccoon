#include "stable.h"
#include "dwfswrapper.h"
#include "graphicsview/dwdummys.h"

//------------------------------------------------------------------------------
/**
*/
bool DwFsWrapper::isDeviceName( const DwString& str )
{
    if (str.length() == 1)
    {
        wchar_t c = str[0];
        if (((c >= L'A') && (c <= L'Z')) ||
                ((c >= L'a') && (c <= L'z')))
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwFsWrapper::homeDirectory()
{
    TCHAR wideBuffer[DW_GUI_MAX_PATH] = { 0 };
    DWORD res = ::GetModuleFileNameW(NULL, (LPWSTR)wideBuffer, sizeof(wideBuffer) / 2);
    DW_ASSERT(0 != res);
    DW_UNUSED(res);

    DwString pathToExe = DwString::fromWCharArray(wideBuffer);
    dwConvertBackslashes(pathToExe);

    DwString pathToDir = dwExtractLastDirName(pathToExe);

    if ( _wcsnicmp(pathToDir.constString(), L"Win32", dwMin(pathToDir.length(), 5)) == 0
            || _wcsnicmp(pathToDir.constString(), L"Debug", dwMin(pathToDir.length(), 5)) == 0
            || _wcsnicmp(pathToDir.constString(), L"Release", dwMin(pathToDir.length(), 5)) == 0 )
    {
        // normal home:bin/win32 directory structure
        // home:bin/debug, home:bin/release
        // strip bin/win32
        DwString homePath = dwExtractDirName(pathToExe);
        homePath = dwExtractDirName(homePath);
        //homePath = extractDirName(homePath);
        dwTrimRight(homePath, L'/');
        return DwString("file:///") + homePath;
    }
    else
    {
        // not in normal home:bin/win32 directory structure,
        // use the exe's directory as home path
        DwString homePath = dwExtractDirName(pathToExe);
        dwTrimRight(homePath, L'/');
        return DwString("file:///") + homePath;
    }
}

//------------------------------------------------------------------------------
/**
*/
DwString DwFsWrapper::binDirectory()
{
    TCHAR wideBuffer[DW_GUI_MAX_PATH] = { 0 };
    DWORD res = GetModuleFileNameW(NULL, (LPWSTR)wideBuffer, sizeof(wideBuffer) / 2);
    DW_ASSERT(0 != res);
    DW_UNUSED(res);
    DwString result = DwString::fromWCharArray(wideBuffer);
    dwConvertBackslashes(result);
    result = dwExtractDirName(result);
    dwTrimRight(result, L'/');
    return DwString("file:///") + result;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwFsWrapper::tempDirectory()
{
    TCHAR wideBuffer[DW_GUI_MAX_PATH] = { 0 };
    GetTempPathW(sizeof(wideBuffer) / 2, (LPWSTR)wideBuffer);
    DwString result = DwString::fromWCharArray(wideBuffer);
    dwConvertBackslashes(result);
    dwTrimRight(result, L'/');
    return DwString("file:///") + result;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwFsWrapper::userDirectory()
{
    TCHAR wideBuffer[DW_GUI_MAX_PATH] = { 0 };
    HRESULT hr = SHGetFolderPathW(NULL,
                                  CSIDL_PERSONAL | CSIDL_FLAG_CREATE,
                                  NULL,
                                  0,
                                  (LPWSTR)wideBuffer);
    DW_ASSERT(SUCCEEDED(hr));
    DW_UNUSED(hr);
    DwString result = DwString::fromWCharArray(wideBuffer);
    dwConvertBackslashes(result);
    return DwString("file:///") + result;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwFsWrapper::appDataDirectory()
{
    TCHAR wideBuffer[DW_GUI_MAX_PATH] = { 0 };
    HRESULT hr = SHGetFolderPathW(NULL,
                                  CSIDL_APPDATA | CSIDL_FLAG_CREATE,
                                  NULL,
                                  0,
                                  (LPWSTR)wideBuffer);
    DW_ASSERT(SUCCEEDED(hr));
    DW_UNUSED(hr);
    DwString result = DwString::fromWCharArray(wideBuffer);
    dwConvertBackslashes(result);
    return DwString("file:///") + result;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwFsWrapper::programsDirectory()
{
    TCHAR wideBuffer[DW_GUI_MAX_PATH] = { 0 };
    HRESULT hr = SHGetFolderPathW(NULL,
                                  CSIDL_PROGRAM_FILES,
                                  NULL,
                                  0,
                                  (LPWSTR)wideBuffer);
    DW_ASSERT(SUCCEEDED(hr));
    DW_UNUSED(hr);
    DwString result = DwString::fromWCharArray(wideBuffer);
    dwConvertBackslashes(result);
    return DwString("file:///") + result;
}

//------------------------------------------------------------------------------
/**
    TODO: USER CONFIG
*/
DwString DwFsWrapper::rootDirectory()
{
    return binDirectory();
}

//------------------------------------------------------------------------------
/**
*/
DwString DwFsWrapper::currentDirectory()
{
    TCHAR wideBuffer[DW_GUI_MAX_PATH] = { 0 };
    DWORD res = ::GetCurrentDirectory(sizeof(wideBuffer) / 2, wideBuffer);
    DW_ASSERT(0 != res);
    DW_UNUSED(res);
    DwString result = DwString::fromWCharArray(wideBuffer);
    dwConvertBackslashes(result);
    return DwString("file:///") + result;
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwString> DwFsWrapper::listFiles(const DwString& dirPath, const DwString& pattern)
{
    DW_ASSERT(!dirPath.isEmpty());
    DW_ASSERT(!pattern.isEmpty());

    DwString pathWithPattern = dirPath + "/" + pattern;
    DwVector<DwString> result;
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = ::FindFirstFile(pathWithPattern.constString(), &findFileData);
    if (INVALID_HANDLE_VALUE != hFind)
    {
        do
        {
            if (0 == (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                result.append(findFileData.cFileName);
            }
        }
        while (::FindNextFile(hFind, &findFileData) != 0);
        ::FindClose(hFind);
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwString> DwFsWrapper::listDirectories(const DwString& dirPath, const DwString& pattern)
{
    DW_ASSERT(!dirPath.isEmpty());
    DW_ASSERT(!pattern.isEmpty());

    DwString pathWithPattern = dirPath + "/" + pattern;
    DwVector<DwString> result;

    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = ::FindFirstFileW(pathWithPattern.constString(), &findFileData);
    if (INVALID_HANDLE_VALUE != hFind)
    {
        do
        {
            DwString fileName = findFileData.cFileName;
            if ((0 != (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) &&
                    (fileName != L"..") && (fileName != L"."))
            {
                result.append(fileName);
            }
        }
        while (::FindNextFileW(hFind, &findFileData) != 0);
        ::FindClose(hFind);
    }

    return result;
}