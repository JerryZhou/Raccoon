TEMPLATE = lib
TARGET = pluginlualibuv
DEPENDPATH += .

INCLUDEPATH += ./libuv/include
INCLUDEPATH += ./libuv/include/uv-private
INCLUDEPATH += ./libuv/src
INCLUDEPATH += ./libuv/src/ares
INCLUDEPATH += ./libuv/src/ares/config_win32
INCLUDEPATH += ./luvit
INCLUDEPATH += ./httppraser

DEFINES += WIN32 _CRT_SECURE_NO_DEPRECATE
DEFINES += _CRT_NONSTDC_NO_DEPRECATE
DEFINES += HAVE_CONFIG_H
DEFINES += _WIN32_WINNT=0x0600
DEFINES += EIO_STACKSIZE=262144 _GNU_SOURCE

include(../../include/common.pro)

LIBS += gdiplus.lib

CONFIG(debug, debug|release) {
    LIBS += dwcored.lib
    LIBS += dwguid.lib
    LIBS += dwgraphicsd.lib
	LIBS += dwluabindd.lib
}

CONFIG(release, debug|release) {
    LIBS += dwcore.lib
    LIBS += dwgui.lib
    LIBS += dwgraphics.lib
	LIBS += dwluabind.lib
}

LIBS += ws2_32.lib
LIBS += psapi.lib
LIBS += iphlpapi.lib

# Input
HEADERS += $$files(bind/export/*.h)
HEADERS += $$files(libuv/include/*.h)
HEADERS += $$files(libuv/include/uv-private/*.h)
HEADERS += $$files(libuv/src/*.h)
HEADERS += $$files(libuv/src/win/*.h)
HEADERS += $$files(libuv/src/ares/*.h)
HEADERS += $$files(libuv/src/ares/config_win32/*.h)
HEADERS += $$files(luvit/*.h)
HEADERS += $$files(httppraser/*.h)

SOURCES += $$files(bind/export/*.cpp)
SOURCES += $$files(libuv/include/*.c)
SOURCES += $$files(libuv/include/uv-private/*.c)
SOURCES += $$files(libuv/src/*.c)
SOURCES += $$files(libuv/src/win/*.c)
SOURCES += $$files(libuv/src/ares/*.c)
SOURCES += $$files(libuv/src/ares/config_win32/*.c)
SOURCES += $$files(luvit/*.c)
SOURCES += $$files(httppraser/*.c)
