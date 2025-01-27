// Copyright (c) ZeroC, Inc.

#include "ThroughputI.h"
#include <Ice/Ice.h>

Demo::Throughput::EchoByteSeqMarshaledResult
ThroughputI::echoByteSeq(std::pair<const std::byte*, const std::byte*> seq, const Ice::Current& current)
{
    return {seq, current};
}

void
ThroughputI::shutdown(const Ice::Current& current)
{
    current.adapter->getCommunicator()->shutdown();
}
