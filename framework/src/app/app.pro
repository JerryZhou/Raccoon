TEMPLATE = lib
TARGET = dwapp
DEPENDPATH += .
DEFINES += BUILD_DW_APP

include(../../include/common.pro)

INCLUDEPATH += . ../../../depends/dw3rd/wtl-80/include

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
HEADERS += dwapp.h
HEADERS += $$files(debug/*.h)
HEADERS += $$files(debug/handlers/*.h)
HEADERS += $$files(generated/*.h)
HEADERS += $$files(io/*.h)
HEADERS += $$files(io/tinyxml/*.h)
HEADERS += $$files(kernel/*.h)
HEADERS += $$files(messaging/*.h)
HEADERS += $$files(net/*.h)
HEADERS += $$files(net/http/*.h)
HEADERS += $$files(net/http/debug/*.h)
HEADERS += $$files(net/http/html/*.h)
HEADERS += $$files(net/http/svg/*.h)
HEADERS += $$files(thread/*.h)
HEADERS += $$files(system/*.h)
HEADERS += $$files(util/*.h)
HEADERS += $$files(resource/*.h)

SOURCES += dwapp.cpp
SOURCES += $$files(debug/*.cpp)
SOURCES += $$files(debug/handlers/*.cpp)
SOURCES += $$files(generated/*.cpp)
SOURCES += $$files(io/*.cpp)
SOURCES += $$files(io/tinyxml/*.cpp)
SOURCES += $$files(kernel/*.cpp)
SOURCES += $$files(messaging/*.cpp)
SOURCES += $$files(net/*.cpp)
SOURCES += $$files(net/http/*.cpp)
SOURCES += $$files(net/http/debug/*.cpp)
SOURCES += $$files(net/http/html/*.cpp)
SOURCES += $$files(net/http/svg/*.cpp)
SOURCES += $$files(thread/*.cpp)
SOURCES += $$files(system/*.cpp)
SOURCES += $$files(util/*.cpp)
SOURCES += $$files(resource/*.cpp)
