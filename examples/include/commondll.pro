CONFIG -= qt

DEFINES += _CRT_SECURE_NO_WARNINGS
DEFINES += _CRT_NON_CONFORMING_SWPRINTFS

INCLUDEPATH += .
INCLUDEPATH += ../../../framework/include
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../depends/dw3rd/gtest-1.5.0/include

PRECOMPILED_HEADER = stable.h

DESTDIR = ../../../bin/$(ConfigurationName)
OBJECTS_DIR = ../../../tmp/$(ProjectName)/$(ConfigurationName)
QMAKE_LIBDIR += "../../../framework/lib"
QMAKE_LIBDIR += "../../../depends/dw3rd/gtest-1.5.0/lib"

CharacterSet = 1
win32 {
	DEFINES += UNICODE
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