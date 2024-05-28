//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef THROUGHPUT_I_H
#define THROUGHPUT_I_H

#include "Throughput.h"

class ThroughputI final : public Demo::Throughput
{
public:
    EchoByteSeqMarshaledResult echoByteSeq(std::pair<const std::byte*, const std::byte*>, const Ice::Current&) final;

    void shutdown(const Ice::Current&) final;
};

#endif
