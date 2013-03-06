TEMPLATE = lib
TARGET = dwluabind
DEPENDPATH += .
DEFINES += BUILD_DW_LUABIND

include(../../include/common.pro)

CONFIG(debug, debug|release) {
    LIBS += dwcored.lib
    LIBS += dwguid.lib
    LIBS += dwgraphicsd.lib
}

CONFIG(release, debug|release) {
    LIBS += dwcore.lib
    LIBS += dwgui.lib
    LIBS += dwgraphics.lib
}

# Input
HEADERS += dwluabind.h
HEADERS += $$files(bind/*.h)
HEADERS += $$files(delegate/*.h)
HEADERS += $$files(delegate/eventdelegate/*.h)
HEADERS += $$files(delegate/itemdelegate/*.h)
HEADERS += $$files(container/*.h)
HEADERS += $$files(export/*.h)
HEADERS += $$files(generated/*.h)
HEADERS += $$files(lua/*.h)

SOURCES += dwluabind.cpp
SOURCES += $$files(bind/*.cpp)
SOURCES += $$files(delegate/*.cpp)
SOURCES += $$files(delegate/eventdelegate/*.cpp)
SOURCES += $$files(delegate/itemdelegate/*.cpp)
SOURCES += $$files(container/*.cpp)
SOURCES += $$files(export/*.cpp)
SOURCES += $$files(generated/*.cpp)
SOURCES += $$files(lua/*.c)

DEF_FILE += lua/lua5.1.def
