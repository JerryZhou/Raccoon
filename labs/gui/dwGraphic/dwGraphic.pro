DEFINES += _CRT_SECURE_NO_WARNINGS _CRT_NON_CONFORMING_SWPRINTFS

CONFIG -= qt
##CONFIG += windows

INCLUDEPATH += . ../../../framework/include
INCLUDEPATH += ../../../eastl/include

DESTDIR = ../../../bin/$(ConfigurationName)
OBJECTS_DIR = ../../../build/$(ProjectName)/$(ConfigurationName)
QMAKE_LIBDIR += ../../../bin/$(ConfigurationName)
LIBS = dwcore.lib

CharacterSet = 1
win32 {
	DEFINES += UNICODE
}

# Input
PRECOMPILED_HEADER = stable.h

HEADERS += gdi/dwbitmap.h\
		   gdi/dwcanvas.h\
		   gdi/dwmath.h\
		   gdi/dwrender.h\
		    gdi/dwinrender.h\
		   gdi/dwtype.h\
		   gdi/dwtexture.h\
		   gdi/dwbmpfile.h\
		   gdi/dwcolorfilter.h\
		   gdi/dwdrawmode.h\
		   testwnd/dwtestwin.h\
		 
SOURCES += dwGraphic.cpp \
           gdi/dwbitmap.cpp\
		   gdi/dwcanvas.cpp\
		   gdi/dwmath.cpp\
		   gdi/dwrender.cpp\
		   gdi/dwinrender.cpp\
		   gdi/dwtexture.cpp\
		   gdi/dwbmpfile.cpp\
		   gdi/dwcolorfilter.cpp\
		   gdi/dwdrawmode.cpp\
		   testwnd/dwtestwin.cpp\
		   