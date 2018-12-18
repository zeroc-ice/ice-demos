// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef THROUGHPUT_I_H
#define THROUGHPUT_I_H

#include <Throughput.h>

class ThroughputI : public Demo::Throughput
{
public:

    virtual EchoByteSeqMarshaledResult echoByteSeq(std::pair<const Ice::Byte*, const Ice::Byte*>, const Ice::Current&) override;

    virtual void shutdown(const Ice::Current&) override;
};

#endif
