TEMPLATE = app
TARGET = 02simplewidgets
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
HEADERS += simplewidgets.h\
			resource.rc

SOURCES += simplewidgets.cpp
SOURCES += main.cpp
