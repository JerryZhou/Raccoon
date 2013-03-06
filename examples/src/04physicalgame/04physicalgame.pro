TEMPLATE = app
TARGET = 04physicalgame

include(../../include/common.pro)

INCLUDEPATH += Box2D/include

LIBS += gdiplus.lib

CONFIG(debug, debug|release) {
    LIBS += dwcored.lib
    LIBS += dwgraphicsd.lib
    LIBS += dwguid.lib
    LIBS += Box2D/lib/Box2D.lib
}

CONFIG(release, debug|release) {
    LIBS += dwcore.lib
    LIBS += dwgraphics.lib
    LIBS += dwgui.lib
    LIBS += Box2D/lib/Box2D.lib
}

# Input
HEADERS += actor.h \
           actorItem.h \
           gameState.h \
           mainFrameWindow.h \
           menuStart.h \
           physicCollisionContactListener.h \
           physicControl.h \   

HEADERS += resource.rc	   

SOURCES = main.cpp \
          actorItem.cpp \
          mainFrameWindow.cpp \
          menuStart.cpp \
          physicCollisionContactListener.cpp \
          physicControl.cpp \