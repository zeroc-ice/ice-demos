# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Chat Demo is licensed to you under the terms described
# in the CHAT_DEMO_LICENSE file included in this distribution.
#
# **********************************************************************

top_srcdir     	= ../..

UNAME		:= $(shell uname)

ifeq ($(UNAME),Darwin)
APPSUFFIX	= .app
endif

CLIENT		= QtChatDemo$(APPSUFFIX)

TARGETS		= Makefile.qmake $(CLIENT)

SLICE_SRCS	= Chat.ice \
		  ChatSession.ice

SRCS		= Chat.cpp \
		  ChatSession.cpp \
		  Util.cpp \
		  Chat.cpp \
		  ChatSession.cpp \
		  LoginView.cpp \
		  ChatView.cpp \
		  MainView.cpp \
		  Coordinator.cpp

HEADERS		= Chat.h \
		  ChatSession.h \
		  ChatView.h \
		  Coordinator.h \
		  LoginView.h \
		  MainView.h \
		  Util.h

include $(top_srcdir)/make/Make.rules

ifeq ($(QT_HOME),)

#
# Qt 4 in rhel6 uses -qt4 suffix
#
ifeq ($(shell test -f /usr/bin/qmake-qt4 && echo 0),0)
QMAKE = /usr/bin/qmake-qt4
endif

else

#
# If QT_HOME is set use qmake from QT_HOME bin sub
# directory
#
ifeq ($(shell test -f $(QT_HOME)/bin/qmake && echo 0),0)
QMAKE = $(QT_HOME)/bin/qmake
MACDEPLOYQT = $(QT_HOME)/bin/macdeployqt
endif

ifeq ($(UNAME),Darwin)

ifeq ($(shell test -f $(QT_HOME)/5.0.0/clang_64/bin/qmake && echo 0),0)
QMAKE = $(QT_HOME)/5.0.0/clang_64/bin/qmake
MACDEPLOYQT = $(QT_HOME)/5.0.0/clang_64/bin/macdeployqt
endif

ifeq ($(shell test -f $(QT_HOME)/5.0.1/clang_64/bin/qmake && echo 0),0)
QMAKE = $(QT_HOME)/5.0.1/clang_64/bin/qmake
MACDEPLOYQT = $(QT_HOME)/5.0.1/clang_64/bin/macdeployqt
endif

endif

endif

#
# If qmake was not found in previous locations, try to use 'qmake'
# from PATH.
#
ifeq ($(QMAKE),)
QMAKE = qmake
endif

ifeq ($(MACDEPLOYQT),)
MACDEPLOYQT = macdeployqt
endif

CPPFLAGS	:= -I. $(CPPFLAGS)
SLICE2CPPFLAGS  := -I. $(SLICE2CPPFLAGS)

Makefile.qmake: qt.pro
	OPTIMIZE=$(OPTIMIZE) ICE_HOME=$(ice_cpp_dir) ICE_libsubdir=$(libsubdir) $(QMAKE) -o Makefile.qmake qt.pro

$(CLIENT): Makefile.qmake $(SRCS) $(HEADERS)
	ICE_HOME=$(ice_cpp_dir) $(MAKE) -f Makefile.qmake
	@if test $(UNAME) = "Darwin" ; \
	then \
	    $(MACDEPLOYQT) QtChatDemo.app; \
	    for component in IceUtil Ice IceSSL Glacier2 ; \
	    do \
		cp -v $(ice_cpp_dir)/lib/lib$$component.$(SOVERSION).dylib  \
		QtChatDemo.app/Contents/Frameworks/; \
	    done; \
	fi

clean::
	$(QMAKE) -o Makefile.qmake qt.pro
	make -f Makefile.qmake clean
	@if test $(UNAME) = "Darwin" ; \
	then \
	    rm -rf $(CLIENT); \
	fi
	rm -rf Makefile.qmake
