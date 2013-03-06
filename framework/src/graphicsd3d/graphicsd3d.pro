TEMPLATE = app
TARGET = dwgraphicsd3d
DEPENDPATH += .
DEFINES += BUILD_DW_GRAPHICS

include(../../include/common.pro)

LIBS += gdiplus.lib
LIBS += dxerr.lib
LIBS += dxguid.lib
LIBS += d3dx9d.lib
LIBS += d3d9.lib
LIBS += winmm.lib
LIBS += comctl32.lib

CONFIG(debug, debug|release) {
    LIBS += dwcored.lib
}

CONFIG(release, debug|release) {
    LIBS += dwcore.lib
}

# Input
HEADERS += dwgraphics.h
HEADERS += $$files(dw*.h)

SOURCES += dwgraphics.cpp
SOURCES += main.cpp
SOURCES += $$files(dw*.cpp)
