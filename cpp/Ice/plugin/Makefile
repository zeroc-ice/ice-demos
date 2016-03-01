# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

top_srcdir	= ../..

CLIENT		= client
SERVER		= server

LOGGERFILENAME	= $(call mklibfilename,LoggerPlugin)
LOGGERSONAME	= $(call mksoname,LoggerPlugin,$(SOVERSION))

HELLOFILENAME	= $(call mklibfilename,HelloPlugin)
HELLOSONAME	= $(call mksoname,HelloPlugin,$(SOVERSION))

TARGETS		= $(CLIENT) $(SERVER) $(LOGGERFILENAME) $(LOGGERSONAME) $(HELLOFILENAME) $(HELLOSONAME)

SLICE_OBJS	= Hello.o

COBJS		= $(SLICE_OBJS) Client.o

SOBJS		= $(SLICE_OBJS) Server.o

LOBJS		= LoggerPluginI.o

HOBJS		= $(SLICE_OBJS) HelloPluginI.o

OBJS		= $(COBJS) \
		  $(SOBJS) \
		  $(LOBJS) \
		  $(HOBJS)

include $(top_srcdir)/make/Make.rules

CPPFLAGS	:= -I. $(CPPFLAGS)

$(CLIENT): $(COBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(COBJS) $(LIBS)

$(SERVER): $(SOBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) $(LDEXEFLAGS) -o $@ $(SOBJS) $(LIBS)

$(LOGGERFILENAME): $(LOBJS)
	rm -f $@
	$(call mkshlib,$@,$(LOGGERSONAME),$(LOBJS),$(LIBS))

$(LOGGERSONAME): $(LOGGERFILENAME)
	rm -f $@
	ln -s $(LOGGERFILENAME) $@

$(HELLOFILENAME): $(HOBJS)
	rm -f $@
	$(call mkshlib,$@,$(HELLOSONAME),$(HOBJS),$(LIBS))

$(HELLOSONAME): $(HELLOFILENAME)
	rm -f $@
	ln -s $(HELLOFILENAME) $@
