TEMPLATE = app

include(../../include/common.pro)

LIBS += gdiplus.lib

CONFIG(debug, debug|release) {
    LIBS += dwcored.lib
	LIBS += dwgraphicsd.lib
    LIBS += dwguid.lib
	LIBS += dwluabindd.lib
}

CONFIG(release, debug|release) {
    LIBS += dwcore.lib
	LIBS += dwgraphics.lib
    LIBS += dwgui.lib
	LIBS += dwluabind.lib
}

SOURCES += main.cpp
SOURCES += $$files(bin*.cpp)
SOURCES += $$files(export/*.cpp)
SOURCES += $$files(delegate/*.cpp)

HEADERS += $$files(bin/*.h)
HEADERS += $$files(export/*.h)
HEADERS += $$files(delegate/*.h)
HEADERS += resource.h

HEADERS += test_luabind.rc