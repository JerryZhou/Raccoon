TEMPLATE = lib
TARGET = dwgraphics
DEPENDPATH += .
DEFINES += BUILD_DW_GRAPHICS

include(../../include/common.pro)

LIBS += gdiplus.lib

CONFIG(debug, debug|release) {
    LIBS += dwcored.lib
}

CONFIG(release, debug|release) {
    LIBS += dwcore.lib
}

# Input
HEADERS += dwgraphics.h
HEADERS += $$files(kernel/*.h)
HEADERS += $$files(image/*.h)
HEADERS += $$files(painting/*.h)
HEADERS += $$files(math/*.h)

SOURCES += dwgraphics.cpp
SOURCES += $$files(kernel/*.cpp)
SOURCES += $$files(image/*.cpp)
SOURCES += $$files(painting/*.cpp)
SOURCES += $$files(math/*.cpp)
