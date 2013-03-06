TEMPLATE = app
TARGET = 01simplewindow
DEPENDPATH += .

include(../../include/common.pro)

LIBS += gdiplus.lib

CONFIG(debug, debug|release) {
    LIBS += dwcored.lib\
			dwgraphicsd.lib\
			dwguid.lib
}

CONFIG(release, debug|release) {
    LIBS += dwcore.lib\
			dwgraphics.lib\
			dwgui.lib
}

# Input
HEADERS += resource.rc

SOURCES += main.cpp
