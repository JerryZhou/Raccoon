TEMPLATE = lib
TARGET = dwgraphics
DEPENDPATH += .
DEFINES += BUILD_DW_GRAPHICS

## include(../../include/common.pro)
DEFINES += _CRT_SECURE_NO_WARNINGS _CRT_NON_CONFORMING_SWPRINTFS

CONFIG -= qt

INCLUDEPATH += . ../../../framework/include
INCLUDEPATH += ../../../eastl/include

PRECOMPILED_HEADER = stable.h

DESTDIR = ../../../bin/$(ConfigurationName)
OBJECTS_DIR = ../../../build/$(ProjectName)/$(ConfigurationName)
QMAKE_LIBDIR += "$(TargetDir)"

CharacterSet = 1
win32 {
	DEFINES += UNICODE
}
LIBS += dwcore.lib

# Input
HEADERS += dwpaintdevice.h\
		   ximage/dwimagedata.h\
		   texture/dwcanvas.h\
		   texture/dwcanvascach.h\
		   render/dwalpha.h\
		   render/dwrasterize.h\
		   render/dwtransform.h\
		   mode/dwdrawmode.h\
		   bitmap/dwbitmap.h\
		   bitmap/dwbrush.h\
		   include/dwtexture.h\
		   include/dwpainter.h\
		   include/dwtype.h\
		  
SOURCES += dwpaintdevice.cpp\	
           dwpainter.cpp\
		   ximage/dwimagedata.cpp\
		   texture/dwcanvas.cpp\
		   texture/dwtexture.cpp\
		   texture/dwcanvascach.cpp\
		   render/dwalpha.cpp\
		   render/dwrasterize.cpp\
		   render/dwtransform.cpp\
		   mode/dwdrawmode.cpp\
		   bitmap/dwbitmap.cpp\
		   bitmap/dwbrush.cpp\

