TEMPLATE = lib
TARGET = dwgui
DEPENDPATH += .
DEFINES += BUILD_DW_GUI

include(../../include/common.pro)

INCLUDEPATH += . ../../../depends/dw3rd/wtl-80/include

CONFIG(debug, debug|release) {
    LIBS += dwcored.lib
    LIBS += dwgraphicsd.lib
}

CONFIG(release, debug|release) {
    LIBS += dwcore.lib
    LIBS += dwgraphics.lib
}

LIBS += riched20.lib
LIBS += version.lib
LIBS += odbc32.lib
LIBS += odbccp32.lib
LIBS += IMM32.lib

# Input
HEADERS += dwgui.h
HEADERS += $$files(kernel/*.h)
HEADERS += $$files(graphicsview/*.h)
HEADERS += $$files(event/*.h)
HEADERS += $$files(layout/*.h)
HEADERS += $$files(widgets/*.h)
HEADERS += $$files(widgets/edit/*.h)
HEADERS += $$files(widgets/windowlessedit/*.h)
HEADERS += $$files(animation/*.h)
HEADERS += $$files(propertys/*.h)
HEADERS += $$files(assign/*.h)
HEADERS += $$files(debug/*.h)
HEADERS += $$files(debug/http/*.h)
HEADERS += $$files(debug/io/*.h)
HEADERS += $$files(debug/net/*.h)
HEADERS += $$files(debug/thread/*.h)
HEADERS += $$files(system/*.h)
HEADERS += $$files(plugin/*.h)
HEADERS += $$files(resource/*.h)
HEADERS += $$files(generated/*.h)
HEADERS += $$files(accessible/*.h)

SOURCES += dwgui.cpp
SOURCES += $$files(kernel/*.cpp)
SOURCES += $$files(graphicsview/*.cpp)
SOURCES += $$files(event/*.cpp)
SOURCES += $$files(layout/*.cpp)
SOURCES += $$files(widgets/*.cpp)
SOURCES += $$files(widgets/edit/*.cpp)
SOURCES += $$files(widgets/windowlessedit/*.cpp)
SOURCES += $$files(animation/*.cpp)
SOURCES += $$files(propertys/*.cpp)
SOURCES += $$files(assign/*.cpp)
SOURCES += $$files(debug/*cpp)
SOURCES += $$files(debug/http/*.cpp)
SOURCES += $$files(debug/io/*.cpp)
SOURCES += $$files(debug/net/*.cpp)
SOURCES += $$files(debug/thread/*.cpp)
SOURCES += $$files(system/*.cpp)
SOURCES += $$files(plugin/*.cpp)
SOURCES += $$files(resource/*.cpp)
SOURCES += $$files(generated/*.cpp)
SOURCES += $$files(accessible/*.cpp)