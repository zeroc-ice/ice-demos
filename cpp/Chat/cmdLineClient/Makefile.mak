# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Chat Demo is licensed to you under the terms described
# in the CHAT_DEMO_LICENSE file included in this distribution.
#
# **********************************************************************

top_srcdir	= ..\..

GL2CLIENT	= $(top_srcdir)\bin\chatgl2client.exe
POLLCLIENT	= $(top_srcdir)\bin\chatpollclient.exe

TARGETS		= $(GL2CLIENT) $(POLLCLIENT)

GL2CLIENT_OBJS	= Chat.obj \
		  ChatUtils.obj \
		  ChatSession.obj \
		  Client.obj

POLLCLIENT_OBJS = Chat.obj \
		  PollingChat.obj \
		  ChatUtils.obj \
		  PollingClient.obj

SRCS		= $(GL2CLIENT_OBJS:.obj=.cpp) \
		  $(POLLCLIENT_OBJS:.obj=.cpp)

!include $(top_srcdir)\config\Make.rules.mak

SLICE2CPPFLAGS  = -I$(top_srcdir)\slice $(SLICE2CPPFLAGS)
CPPFLAGS	= -I. $(ICE_CPPFLAGS) $(CPPFLAGS) -DWIN32_LEAN_AND_MEAN
LDFLAGS		= $(ICE_LDFLAGS) $(LDFLAGS)

!if "$(GENERATE_PDB)" == "yes"
GL2PDBFLAGS        = /pdb:$(GL2CLIENT:.exe=.pdb)
POLLPDBFLAGS       = /pdb:$(POLLCLIENT:.exe=.pdb)
!endif

$(GL2CLIENT): $(GL2CLIENT_OBJS)
	$(LINK) $(LD_EXEFLAGS) $(GL2PDBFLAGS) $(SETARGV) $(GL2CLIENT_OBJS) $(PREOUT)$@ $(PRELIBS)glacier2$(LIBSUFFIX).lib $(ICE_LIBS)
	@if exist $@.manifest echo ^ ^ ^ Embedding manifest using $(MT) && \
	    $(MT) -nologo -manifest $@.manifest -outputresource:$@;#1 && del /q $@.manifest

$(POLLCLIENT): $(POLLCLIENT_OBJS)
	$(LINK) $(LD_EXEFLAGS) $(POLLPDBFLAGS) $(SETARGV) $(POLLCLIENT_OBJS) $(PREOUT)$@ $(PRELIBS)$(ICE_LIBS)
	@if exist $@.manifest echo ^ ^ ^ Embedding manifest using $(MT) && \
	    $(MT) -nologo -manifest $@.manifest -outputresource:$@;#1 && del /q $@.manifest

install:: all
	copy  $(GL2CLIENT) $(install_bindir)
	copy $(POLLCLIENT) $(install_bindir)

clean::
	-del /q Chat.cpp Chat.h ChatSession.cpp ChatSession.h PollingChat.cpp PollingChat.h
	-del $(GL2CLIENT:.exe=.*)
	-del $(POLLCLIENT:.exe=.*)

!include .depend
