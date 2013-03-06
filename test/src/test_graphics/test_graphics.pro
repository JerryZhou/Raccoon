TEMPLATE = app

include(../../include/common.pro)

CONFIG += console

LIBS += gdiplus.lib

CONFIG(debug, debug|release) {
    LIBS += dwcored.lib
    LIBS += dwgraphicsd.lib
    LIBS += dwguid.lib
}

CONFIG(release, debug|release) {
    LIBS += dwcore.lib
    LIBS += dwgraphics.lib
    LIBS += dwgui.lib
}

# Input
HEADERS += $$files(graphics_test_*.h)

SOURCES = main.cpp
