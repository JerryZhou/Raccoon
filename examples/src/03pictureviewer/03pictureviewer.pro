TEMPLATE = app
TARGET = 03pictureviewer
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
HEADERS += picViewer.h\
			resource.rc

SOURCES += picViewer.cpp
SOURCES += main.cpp
