# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ../..

CLIENT		= client
SERVER		= server

LIBFILENAME     = $(call mklibfilename,RegistryPlugin,$(VERSION))
SONAME		= $(call mksoname,RegistryPlugin,$(SOVERSION)) 
LIBNAME		= $(call mklibname,RegistryPlugin)

TARGETS		= $(CLIENT) $(SERVER) $(call mklibtargets,$(LIBFILENAME),$(SONAME),$(LIBNAME))

SLICE_OBJS	= Pricing.o

COBJS		= $(SLICE_OBJS) \
		  Client.o

SOBJS		= $(SLICE_OBJS) \
		  PricingI.o \
		  Server.o

POBJS		= RegistryPlugin.o

OBJS		= $(COBJS) \
		  $(SOBJS) \
		  $(POBJS)

include $(top_srcdir)/make/Make.rules

CPPFLAGS	:= -I. $(CPPFLAGS)
LINKWITH	:= $(BZIP2_RPATH_LINK) -lIce -lIceUtil

$(CLIENT): $(COBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(COBJS) -lIceGrid -lGlacier2 $(LINKWITH)

$(SERVER): $(SOBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(SOBJS) $(LINKWITH)

$(LIBFILENAME): $(POBJS)
	rm -f $@
	$(call mkshlib,$@,$(SONAME),$(POBJS),-lIceGrid -lGlacier2 $(LINKWITH))

$(SONAME): $(LIBFILENAME)
	rm -f $@
	ln -s $(LIBFILENAME) $@

$(LIBNAME): $(SONAME)
	rm -f $@
	ln -s $(SONAME) $@

clean::
	-rm -rf db/node/* db/registry/*
