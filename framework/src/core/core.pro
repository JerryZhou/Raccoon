TEMPLATE = lib
TARGET = dwcore
DEPENDPATH += .
DEFINES += BUILD_DW_CORE EA_DLL
DEFINES += DW_SAFE_PARSER

include(../../include/common.pro)

# Input
SOURCES += dwcore.cpp
SOURCES += $$files(io/*.cpp)
SOURCES += $$files(global/*.cpp)
SOURCES += $$files(kernel/*.cpp)
SOURCES += $$files(eastl/*.cpp)
SOURCES += $$files(tools/*.cpp)
SOURCES += $$files(parser/*.cpp)
SOURCES += $$files(zip/*.cpp)
SOURCES += $$files(3rdparty/*.c)
SOURCES += $$files(3rdparty/*.cpp)
SOURCES += $$files(3rdparty/zlib/*.c)
SOURCES += $$files(3rdparty/zlib/*.cpp)
SOURCES += $$files(3rdparty/zlib/minizip/*.c)
SOURCES += $$files(platform/windows/*.cpp)
SOURCES += $$files(codecs/*.cpp)

HEADERS += dwcore.h
HEADERS += $$files(io/*.h)
HEADERS += $$files(global/*.h)
HEADERS += $$files(kernel/*.h)
HEADERS += $$files(eastl/*.h)
HEADERS += $$files(tools/*.h)
HEADERS += $$files(parser/*.h)
HEADERS += $$files(zip/*.h)
HEADERS += $$files(3rdparty/*.h)
HEADERS += $$files(3rdparty/zlib/*.h)
HEADERS += $$files(3rdparty/zlib/minizip/*.h)
HEADERS += $$files(platform/windows/*.h)
HEADERS += $$files(codecs/*.h)
