#pragma once

#ifdef WIN32
typedef HWND WinId;
#else
typedef int WinId;
#endif