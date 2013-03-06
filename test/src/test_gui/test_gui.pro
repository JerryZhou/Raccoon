TEMPLATE = app

include(../../include/common.pro)
INCLUDEPATH += ../../../eastl/include

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
HEADERS += gtest_helper.h
HEADERS += $$files(gui_test_*.h)

SOURCES = main.cpp
