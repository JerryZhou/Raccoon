TEMPLATE = app

include(../../include/common.pro)

CONFIG += console

CONFIG(debug, debug|release) {
    LIBS += dwcored.lib
}

CONFIG(release, debug|release) {
    LIBS += dwcore.lib
}

# Input
HEADERS += $$files(core_test_*.h)

SOURCES = main.cpp
