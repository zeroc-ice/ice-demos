# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Ice Protobuf is licensed to you under the terms
# described in the ICE_PROTOBUF_LICENSE file included in this
# distribution.
#
# **********************************************************************

#
# If protobuf is not installed in a standard location where the
# compiler can find it, set PROTOBUF_HOME to the protobuf installation
# directory.
#
PROTOBUF_HOME   ?= /usr/local

top_srcdir	= ..

CLIENT		= client
SERVER		= server

TARGETS		= $(CLIENT) $(SERVER)

SLICE_OBJS	= Hello.o

OBJS		= Person.pb.o

COBJS		= Client.o

SOBJS		= HelloI.o \
		  Server.o

SRCS		= $(SLICE_OBJS:.o=.cpp) \
		  $(OBJS:.o=.cpp) \
		  $(COBJS:.o=.cpp) \
		  $(SOBJS:.o=.cpp)

PROTO_SRCS	= Person.proto

include $(top_srcdir)/Make/Make.rules

CPPFLAGS	:= $(ICE_FLAGS) -I$(PROTOBUF_HOME)/include -I. $(CPPFLAGS)
LIBS		:= $(ICE_LIB_DIR) $(LIBS) -L$(PROTOBUF_HOME)/lib -lprotobuf

$(CLIENT): $(SLICE_OBJS) $(OBJS) $(COBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ $(SLICE_OBJS) $(OBJS) $(COBJS) $(LIBS)

$(SERVER): $(SLICE_OBJS) $(OBJS) $(SOBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ $(SLICE_OBJS) $(OBJS) $(SOBJS) $(LIBS)

%..pb.h %.pb.cpp: %.proto
	rm -f $(*F).pb.h $(*F).pb.cpp
	$(PROTOBUF_HOME)/bin/protoc --cpp_out=. $(*F).proto
	mv $(*F).pb.cc $(*F).pb.cpp

clean::
	rm -f $(addsuffix .pb.cpp, $(basename $(notdir $(PROTO_SRCS))))
	rm -f $(addsuffix .pb.h, $(basename $(notdir $(PROTO_SRCS))))
