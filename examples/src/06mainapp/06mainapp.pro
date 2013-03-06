TEMPLATE = app
TARGET = 06mainapp
DEPENDPATH += .

include(../../include/common.pro)

LIBS += gdiplus.lib

CONFIG(debug, debug|release) {
    LIBS += dwcored.lib\
			dwgraphicsd.lib\
			dwguid.lib\
			dwluabindd.lib\
			dwappd.lib
}

CONFIG(release, debug|release) {
    LIBS += dwcore.lib\
			dwgraphics.lib\
			dwgui.lib\
			dwluabind.lib\
			dwapp.lib
}

# Input
HEADERS += resource.rc

SOURCES += main.cpp
