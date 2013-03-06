CONFIG -= qt

DEFINES += _CRT_SECURE_NO_WARNINGS
DEFINES += _CRT_NON_CONFORMING_SWPRINTFS

INCLUDEPATH += . ../../include

PRECOMPILED_HEADER = stable.h

DESTDIR = ../../../bin/$(ConfigurationName)
OBJECTS_DIR = ../../../tmp/$(ProjectName)/$(ConfigurationName)
QMAKE_LIBDIR += "../../lib"

QMAKE_LFLAGS_DEBUG += /IMPLIB:../../lib/$(ProjectName)d.lib
QMAKE_LFLAGS_RELEASE += /IMPLIB:../../lib/$(ProjectName).lib

CharacterSet = 1
win32 {
	DEFINES += UNICODE
}

DEFINES += BUILD_DW_FRAMEWORK_STATIC_BUILD
CONFIG += staticlib
CONFIG(debug, debug|release){
    OBJECTS_DIR = ../../../tmp/$(ProjectName)/debug_static
}
CONFIG(release, release){
    OBJECTS_DIR = ../../../tmp/$(ProjectName)/release_static
}
DESTDIR = "../../lib"
CONFIG(debug, debug|release) {
     win32: TARGET = $$join(TARGET,,,d)
}

QMAKE_CXXFLAGS += /Fd$(IntDir)
QMAKE_CXXFLAGS_RELEASE += /Zi
QMAKE_CXXFLAGS += /WX
QMAKE_CXXFLAGS += /MP
QMAKE_CXXFLAGS += /GS
QMAKE_CXXFLAGS += /GR-
QMAKE_LFLAGS_RELEASE += /DEBUG
QMAKE_LFLAGS_RELEASE += /OPT:REF /OPT:ICF
QMAKE_LFLAGS += /MACHINE:X86
QMAKE_LFLAGS_DEBUG += /debugtype:cv,fixup
QMAKE_CXXFLAGS_EXCEPTIONS_ON =
QMAKE_CXXFLAGS_STL_ON   = 
QMAKE_CXXFLAGS_RTTI_ON += /GR-

# for yy setup
QMAKE_CXXFLAGS_RELEASE += /O1