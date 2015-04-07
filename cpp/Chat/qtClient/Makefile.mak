# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Chat Demo is licensed to you under the terms described
# in the CHAT_DEMO_LICENSE file included in this distribution.
#
# **********************************************************************

top_srcdir	= ..\..

CLIENT		= $(top_srcdir)\bin\QtChatDemo.exe

TARGETS		= Makefile.qmake $(CLIENT)

SLICE_SRCS	= $(top_srcdir)\slice\Chat.ice \
		  $(top_srcdir)\slice\ChatSession.ice

SRCS	= Chat.cpp \
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

!include $(top_srcdir)/config/Make.rules.mak

CPPFLAGS		= $(CPPFLAGS) -I. $(ICE_FLAGS)

SLICE2CPPFLAGS  = -I$(top_srcdir)\slice $(SLICE2CPPFLAGS)

Makefile.qmake: qt.pro Chat.cpp ChatSession.cpp
	set OPTIMIZE=$(OPTIMIZE)
	set ICE_HOME="$(ice_cpp_dir)"
	set ICE_x64suffix=$(x64suffix)
	$(QMAKE) -o Makefile.qmake qt.pro

$(CLIENT): Makefile.qmake $(SRCS) $(HEADERS)
	nmake /f Makefile.qmake

install:: all
	copy  $(CLIENT) $(install_bindir)

clean::
	$(QMAKE) -o Makefile.qmake qt.pro
	nmake -f Makefile.qmake clean
	del Makefile.qmake
	del Makefile.qmake.Debug
	del Makefile.qmake.Release
	del Chat.cpp Chat.h
	del ChatSession.cpp ChatSession.h
