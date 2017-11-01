TEMPLATE = app

TARGET = client
DESTDIR = .

CONFIG += qt warn_on

QT = core gui widgets

HEADERS = Client.h Hello.h

SOURCES = Client.cpp Hello.cpp

DEFINES += ICE_CPP11_MAPPING

OPTIMIZE = $$(OPTIMIZE)

INCLUDEPATH += . $$(ICE_HOME)/include $$(ICE_HOME)/include/generated

QMAKE_LIBDIR  += $$(ICE_libdir)

LIBS += -lIce++11
