/*
 *  Copyright 2012 The Luvit Authors. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include "luv.h"
#include "uv.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "uv-private/ev.h"

#include "luv_fs.h"
#include "luv_dns.h"
#include "luv_handle.h"
#include "luv_udp.h"
#include "luv_fs_watcher.h"
#include "luv_timer.h"
#include "luv_process.h"
#include "luv_stream.h"
#include "luv_tcp.h"
#include "luv_pipe.h"
#include "luv_tty.h"
#include "luv_misc.h"

#include "los.h"
#include "luv_dns.h"
#include "luv_debug.h"
#include "luv_portability.h"
#include "lconstants.h"
#include "lhttp_parser.h"
#include "lenv.h"

static const luaL_reg luv_f[] = {

  /* Handle functions */
  {"close", luv_close},
  {"ref", luv_ref},
  {"unref", luv_unref},
  {"setHandler", luv_set_handler},

  /* UDP functions */
  {"newUdp", luv_new_udp},
  {"udpBind", luv_udp_bind},
  {"udpBind6", luv_udp_bind6},
  {"udpSetMembership", luv_udp_set_membership},
  {"udpGetsockname", luv_udp_getsockname},
  {"udpSend", luv_udp_send},
  {"udpSend6", luv_udp_send6},
  {"udpRecvStart", luv_udp_recv_start},
  {"udpRecvStop", luv_udp_recv_stop},
  {"udpSetBroadcast", luv_udp_set_broadcast},
  {"udpSetTTL", luv_udp_set_ttl},
  {"udpSetMulticastTTL", luv_udp_set_multicast_ttl},
  {"udpSetMulticastLoopback", luv_udp_set_multicast_loopback},

  /* FS Watcher functions */
  {"newFsWatcher", luv_new_fs_watcher},

  /* Timer functions */
  {"newTimer", luv_new_timer},
  {"timerStart", luv_timer_start},
  {"timerStop", luv_timer_stop},
  {"timerAgain", luv_timer_again},
  {"timerSetRepeat", luv_timer_set_repeat},
  {"timerGetRepeat", luv_timer_get_repeat},
  {"timerGetActive", luv_timer_get_active},

  /* Process functions */
  {"spawn", luv_spawn},
  {"processKill", luv_process_kill},
  {"getpid", luv_getpid},
  
  /* Stream functions */
  {"shutdown", luv_shutdown},
  {"listen", luv_listen},
  {"accept", luv_accept},
  {"readStart", luv_read_start},
  {"readStart2", luv_read_start2},
  {"readStop", luv_read_stop},
  {"readStopNoRef", luv_read_stop_noref},
  {"writeQueueSize", luv_write_queue_size},
  {"write", luv_write},
  {"write2", luv_write2},

  /* TCP functions */
  {"newTcp", luv_new_tcp},
  {"tcpBind", luv_tcp_bind},
  {"tcpBind6", luv_tcp_bind6},
  {"tcpNodelay", luv_tcp_nodelay},
  {"tcpGetsockname", luv_tcp_getsockname},
  {"tcpGetpeername", luv_tcp_getpeername},
  {"tcpConnect", luv_tcp_connect},
  {"tcpConnect6", luv_tcp_connect6},

  /* Pipe functions */
  {"newPipe", luv_new_pipe},
  {"pipeOpen", luv_pipe_open},
  {"pipeBind", luv_pipe_bind},
  {"pipeConnect", luv_pipe_connect},

  /* TTY functions */
  {"newTty", luv_new_tty},
  {"ttySetMode", luv_tty_set_mode},
  {"ttyResetMode", luv_tty_reset_mode},
  {"ttyGetWinsize", luv_tty_get_winsize},

  /* DNS functions */
  {"dnsQueryA", luv_dns_queryA},
  {"dnsQueryAaaa", luv_dns_queryAaaa},
  {"dnsQueryCname", luv_dns_queryCname},
  {"dnsQueryMx", luv_dns_queryMx},
  {"dnsQueryNs", luv_dns_queryNs},
  {"dnsQueryTxt", luv_dns_queryTxt},
  {"dnsQuerySrv", luv_dns_querySrv},
  {"dnsGetHostByAddr", luv_dns_getHostByAddr},
  {"dnsGetAddrInfo", luv_dns_getAddrInfo},
  {"dnsIsIp", luv_dns_isIp},
  {"dnsIsIpV4", luv_dns_isIpV4},
  {"dnsIsIpV6", luv_dns_isIpV6},

  /* FS functions */
  {"fsOpen", luv_fs_open},
  {"fsClose", luv_fs_close},
  {"fsRead", luv_fs_read},
  {"fsWrite", luv_fs_write},
  {"fsUnlink", luv_fs_unlink},
  {"fsMkdir", luv_fs_mkdir},
  {"fsRmdir", luv_fs_rmdir},
  {"fsReaddir", luv_fs_readdir},
  {"fsStat", luv_fs_stat},
  {"fsFstat", luv_fs_fstat},
  {"fsRename", luv_fs_rename},
  {"fsFsync", luv_fs_fsync},
  {"fsFdatasync", luv_fs_fdatasync},
  {"fsFtruncate", luv_fs_ftruncate},
  {"fsSendfile", luv_fs_sendfile},
  {"fsChmod", luv_fs_chmod},
  {"fsUtime", luv_fs_utime},
  {"fsFutime", luv_fs_futime},
  {"fsLstat", luv_fs_lstat},
  {"fsLink", luv_fs_link},
  {"fsSymlink", luv_fs_symlink},
  {"fsReadlink", luv_fs_readlink},
  {"fsFchmod", luv_fs_fchmod},
  {"fsChown", luv_fs_chown},
  {"fsFchown", luv_fs_fchown},

  /* Misc functions */
  {"run", luv_run},
  {"run_once", luv_run_once},
  {"printActiveHandles", luv_print_active_handles},
  {"printAllHandles", luv_print_all_handles},
  {"updateTime", luv_update_time},
  {"now", luv_now},
  {"hrtime", luv_hrtime},
  {"getFreeMemory", luv_get_free_memory},
  {"getTotalMemory", luv_get_total_memory},
  {"loadavg", luv_loadavg},
  {"uptime", luv_uptime},
  {"cpuInfo", luv_cpu_info},
  {"interfaceAddresses", luv_interface_addresses},
  {"execpath", luv_execpath},
  {"getProcessTitle", luv_get_process_title},
  {"setProcessTitle", luv_set_process_title},
  {"handleType", luv_handle_type},
  {"activateSignalHandler", luv_activate_signal_handler},
  {NULL, NULL}
};

/* When the lhandle is freed, do some helpful sanity checks */
static int luv_handle_gc(lua_State* L) {
  luv_handle_t* lhandle = (luv_handle_t*)lua_touserdata(L, 1);
/*  printf("__gc %s lhandle=%p handle=%p\n", lhandle->type, lhandle, lhandle->handle);*/
  /* If the handle is still there, they forgot to close */
  if (lhandle->handle) {
    fprintf(stderr, "WARNING: forgot to close %s lhandle=%p handle=%p\n", lhandle->type, lhandle, lhandle->handle);
    uv_close(lhandle->handle, luv_on_close);
  }
  return 0;
}

int luaopen_uv_native (lua_State* L) {
  /* metatable for handle userdata types */
  /* It is it's own __index table to save space */
  luaL_newmetatable(L, "luv_handle");
  lua_pushcfunction(L, luv_handle_gc);
  lua_setfield(L, -2, "__gc");
  lua_pop(L, 1);

  /* Create a new exports table with functions and constants */
  lua_newtable (L);

  luaL_register(L, NULL, luv_f);
  lua_pushnumber(L, UV_VERSION_MAJOR);
  lua_setfield(L, -2, "VERSION_MAJOR");
  lua_pushnumber(L, UV_VERSION_MINOR);
  lua_setfield(L, -2, "VERSION_MINOR");

  return 1;
}

static int luvit_exit(lua_State* L) {
    int exit_code = luaL_checkint(L, 1);
    exit(exit_code);
    return 0;
}

static int luvit_print_stderr(lua_State* L) {
    const char* line = luaL_checkstring(L, 1);
    fprintf(stderr, "%s", line);
    return 0;
}

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof((a)) / sizeof((a)[0]))
#endif

static char getbuf[PATH_MAX + 1];

static int luvit_getcwd(lua_State* L) {
    uv_err_t rc;

    rc = uv_cwd(getbuf, ARRAY_SIZE(getbuf) - 1);
    if (rc.code != UV_OK) {
        return luaL_error(L, "luvit_getcwd: %s\n", strerror(errno));
    }

    getbuf[ARRAY_SIZE(getbuf) - 1] = '\0';
    lua_pushstring(L, getbuf);
    return 1;
}

int luvit_init_p(lua_State *L, uv_loop_t* loop)
{
    int index, rc;
    ares_channel channel;
    struct ares_options options;
    int argc = 0;
    char *argv[] = {""};

    memset(&options, 0, sizeof(options));

    rc = ares_library_init(ARES_LIB_INIT_ALL);
    //assert(rc == ARES_SUCCESS);

    /* Pull up the preload table */
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_remove(L, -2);

    /* Register debug */
    lua_pushcfunction(L, luaopen_debugger);
    lua_setfield(L, -2, "_debug");
    /* Register os */
    lua_pushcfunction(L, luaopen_os_binding);
    lua_setfield(L, -2, "os_binding");
    /* Register http_parser */
    lua_pushcfunction(L, luaopen_http_parser);
    lua_setfield(L, -2, "http_parser");
    /* Register uv */
    lua_pushcfunction(L, luaopen_uv_native);
    lua_setfield(L, -2, "uv_native");
    /* Register env */
    lua_pushcfunction(L, luaopen_env);
    lua_setfield(L, -2, "env");
    /* Register constants */
    lua_pushcfunction(L, luaopen_constants);
    lua_setfield(L, -2, "constants");

    /* We're done with preload, put it away */
    lua_pop(L, 1);

    /* Get argv */
    lua_createtable (L, argc, 0);
    for (index = 0; index < argc; index++) {
        lua_pushstring (L, argv[index]);
        lua_rawseti(L, -2, index);
    }
    lua_setglobal(L, "argv");

    lua_pushcfunction(L, luvit_exit);
    lua_setglobal(L, "exitProcess");

    lua_pushcfunction(L, luvit_print_stderr);
    lua_setglobal(L, "printStderr");

    lua_pushcfunction(L, luvit_getcwd);
    lua_setglobal(L, "getcwd");

    lua_pushcfunction(L, luv_run);
    lua_setglobal(L, "run");

    lua_pushstring(L, "1.0.0");
    lua_setglobal(L, "VERSION");

    lua_pushstring(L, "1.0.0");
    lua_setglobal(L, "UV_VERSION");

    lua_pushstring(L, "HTTP/1.0");
    lua_setglobal(L, "HTTP_VERSION");

    /* Hold a reference to the main thread in the registry */
    rc = lua_pushthread(L);
    assert(rc == 1);
    lua_setfield(L, LUA_REGISTRYINDEX, "main_thread");

    /* Store the loop within the registry */
    luv_set_loop(L, loop);

    /* Store the ARES Channel */
    uv_ares_init_options(luv_get_loop(L), &channel, &options, 0);
    luv_set_ares_channel(L, channel);

    return 0;
}

#ifdef WIN32
__declspec(dllexport)
#endif
int lua_open_libvit(lua_State *L){
    uv_loop_t *loop;

    loop = uv_default_loop();
    if (luvit_init_p(L, loop)) {
        fprintf(stderr, "luvit_init has failed\n");
        return 1;
    }

    luaL_dostring(L, "dwAddRequireSearchUrl(\"script:luvit\")\
                     require('luvitinit')");

    return 0;
}

