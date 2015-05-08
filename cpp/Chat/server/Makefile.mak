# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ..

SERVER		= $(top_srcdir)\bin\chatserver.exe

TARGETS		= $(SERVER)

OBJS		= Chat.obj \
		  ChatSession.obj \
		  PollingChat.obj \
		  ChatUtils.obj \
		  ChatSessionI.obj \
		  ChatSessionManagerI.obj \
		  PollingChatSessionI.obj \
		  PollingChatSessionFactoryI.obj \
		  ChatRoom.obj \
		  ChatServer.obj

SRCS		= $(OBJS:.obj=.cpp)

SLICE_SRCS	= $(top_srcdir)/slice/Chat.ice \
		  $(top_srcdir)/slice/ChatSession.ice \
		  $(top_srcdir)/slice/PollingChat.ice

!include $(top_srcdir)/config/Make.rules.mak

SLICE2CPPFLAGS  = -I$(top_srcdir)\slice $(SLICE2CPPFLAGS)
CPPFLAGS	= -I. $(ICE_CPPFLAGS) $(CPPFLAGS) -DWIN32_LEAN_AND_MEAN
LDFLAGS		= $(ICE_LDFLAGS) $(LDFLAGS)

!if "$(GENERATE_PDB)" == "yes"
SPDBFLAGS        = /pdb:$(SERVER:.exe=.pdb)
!endif

$(SERVER): $(OBJS)
	$(LINK) $(LD_EXEFLAGS) $(SPDBFLAGS) $(SETARGV) $(OBJS) $(PREOUT)$@ $(PRELIBS)glacier2$(LIBSUFFIX).lib $(ICE_LIBS)
	@if exist $@.manifest echo ^ ^ ^ Embedding manifest using $(MT) && \
	    $(MT) -nologo -manifest "$@.manifest" -outputresource:"$@";#1 && del /q "$@.manifest"

install::
	copy $(SERVER) $(install_bindir)

clean::
	-del /q Chat.cpp Chat.h ChatSession.cpp ChatSession.h PollingChat.cpp PollingChat.h
	-del $(SERVER:.exe=.*)

!include .depend
