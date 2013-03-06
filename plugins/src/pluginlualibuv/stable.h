#pragma once
#include "dwstable.h"
#include <winsock2.h>
#include <iptypes.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "dwcore.h"
#include "dwluabind/dwlua.h"
# define _SSIZE_T_
# define _SSIZE_T_DEFINED
extern "C"
{
#include "./libuv/include/uv.h"
#include "./libuv/src/uv-common.h"
#include "./libuv/include/uv-private/tree.h"
}

#define DW_PLUGIN_LUAUV_EXPORT __declspec( dllexport )
