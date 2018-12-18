# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

TEMPLATE = app

TARGET = client
DESTDIR = .

CONFIG += qt warn_on debug
CONFIG -= app_bundle

QT = core gui widgets

SLICES = Hello.ice

HEADERS = Client.h

SOURCES = Client.cpp

DEFINES += ICE_CPP11_MAPPING

INCLUDEPATH += .

macx {
  ICE_HOME =? /usr/local
}
unix:!macx {
  ICE_HOME =? /usr
}

equals (ICE_HOME, "/usr") {
} else:equals(ICE_HOME, "/usr/local") {
} else:exists($(ICE_HOME)/include ) {
   INCLUDEPATH += $(ICE_HOME)/include
} else:exists($(ICE_HOME)/cpp/include ) {
   INCLUDEPATH += $(ICE_HOME)/cpp/include $(ICE_HOME)/cpp/include/generated
}

exists( $(ICE_HOME)/bin/slice2cpp ) {
   SLICE2CPP = $(ICE_HOME)/bin/slice2cpp
} else:exists( $(ICE_HOME)/cpp/bin/slice2cpp ) {
   SLICE2CPP = $(ICE_HOME)/cpp/bin/slice2cpp
} else {
   error(Cannot find a valid slice2cpp compiler)
}

equals (ICE_HOME, "/usr") {
} else:equals( ICE_HOME, "/usr/local") {
} else:exists( $(ICE_HOME)/lib64/libIce++11.* ) {
   LIBS += -L$(ICE_HOME)/lib64
} else:exists( $(ICE_HOME)/lib/libIce++11.* ) {
   LIBS += -L$(ICE_HOME)/lib
} else:exists( $(ICE_HOME)/lib/x86_64-linux-gnu/libIce++11.* ) {
   LIBS += -L$(ICE_HOME)/lib/x86_64-linux-gnu
} else:exists( $(ICE_HOME)/cpp/lib64/libIce++11.* ) {
   LIBS += -L$(ICE_HOME)/cpp/lib64
} else:exists( $(ICE_HOME)/cpp/lib/libIce++11.* ) {
   LIBS += -L$(ICE_HOME)/cpp/lib
} else:exists( $(ICE_HOME)/cpp/lib/x86_64-linux-gnu ) {
   LIBS += -L$(ICE_HOME)/cpp/lib/x86_64-linux-gnu
} else {
   error(Cannot find a valid ICE library directory)
}

LIBS += -lIce++11

#
# Qmake extra compilers expect a single output file per input file.
# The slice2h fake compiler worksaround this limitation it just add
# the generated header to HEADERS variable and adds a dependency
# to the slice2cpp compiler that generates both the header and source
# files.
#
slice2h.output = ${QMAKE_FILE_BASE}.h
slice2h.variable_out = HEADERS
slice2h.commands = touch ${QMAKE_FILE_BASE}.h
slice2h.depends = ${QMAKE_FILE_BASE}.cpp
slice2h.input = SLICES

slice2cpp.output  = ${QMAKE_FILE_BASE}.cpp
slice2cpp.variable_out = SOURCES HEADERS
slice2cpp.commands = $${SLICE2CPP} ${QMAKE_FILE_NAME} -I$(ICE_HOME)/slice
slice2cpp.input = SLICES

QMAKE_EXTRA_COMPILERS += slice2h slice2cpp
