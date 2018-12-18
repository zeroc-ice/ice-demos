// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <ThroughputI.h>

ThroughputI::ThroughputI() :
    _byteSeq(Demo::ByteSeqSize),
    _stringSeq(Demo::StringSeqSize, "hello"),
    _stringViewSeq(Demo::StringSeqSize, "hello"),
    _structSeq(Demo::StringDoubleSeqSize, { "hello", 3.14 }),
    _fixedSeq(Demo::FixedSeqSize, { 0, 0, 0.0 }),
    _warmup(false)
{
}

bool
ThroughputI::needsWarmup(const Ice::Current&)
{
    _warmup = false;
    return false;
}

void
ThroughputI::startWarmup(const Ice::Current&)
{
    _warmup = true;
}

void
ThroughputI::endWarmup(const Ice::Current&)
{
    _warmup = false;
}

void
ThroughputI::sendByteSeq(std::pair<const Ice::Byte*, const Ice::Byte*>, const Ice::Current&)
{
}

Demo::Throughput::RecvByteSeqMarshaledResult
ThroughputI::recvByteSeq(const Ice::Current& current)
{
    if(_warmup)
    {
        Demo::ByteSeq warmupBytesBuf(1);
        std::pair<const Ice::Byte*, const Ice::Byte*> ret =
            std::make_pair(warmupBytesBuf.data(), warmupBytesBuf.data() + warmupBytesBuf.size());
        return RecvByteSeqMarshaledResult(ret, current);
    }
    else
    {
        std::pair<const Ice::Byte*, const Ice::Byte*> ret =
            std::make_pair(_byteSeq.data(), _byteSeq.data() + _byteSeq.size());
        return RecvByteSeqMarshaledResult(ret, current);
    }
}

Demo::Throughput::EchoByteSeqMarshaledResult
ThroughputI::echoByteSeq(std::pair<const Ice::Byte*, const Ice::Byte*> seq, const Ice::Current& current)
{
    return EchoByteSeqMarshaledResult(seq, current);
}

void
ThroughputI::sendStringSeq(std::vector<Util::string_view>, const Ice::Current&)
{
}

Demo::Throughput::RecvStringSeqMarshaledResult
ThroughputI::recvStringSeq(const Ice::Current& current)
{
    if(_warmup)
    {
        return RecvStringSeqMarshaledResult(std::vector<Util::string_view>(1), current);
    }
    else
    {
        return RecvStringSeqMarshaledResult(_stringViewSeq, current);
    }
}

Demo::Throughput::EchoStringSeqMarshaledResult
ThroughputI::echoStringSeq(std::vector<Util::string_view> seq, const Ice::Current& current)
{
    return EchoStringSeqMarshaledResult(seq, current);
}

void
ThroughputI::sendStructSeq(Demo::StringDoubleSeq, const Ice::Current&)
{
}

Demo::StringDoubleSeq
ThroughputI::recvStructSeq(const Ice::Current&)
{
    if(_warmup)
    {
        return Demo::StringDoubleSeq(1);
    }
    else
    {
        return _structSeq;
    }
}

Demo::StringDoubleSeq
ThroughputI::echoStructSeq(Demo::StringDoubleSeq seq, const Ice::Current&)
{
    return seq;
}

void
ThroughputI::sendFixedSeq(Demo::FixedSeq, const Ice::Current&)
{
}

Demo::FixedSeq
ThroughputI::recvFixedSeq(const Ice::Current&)
{
    if(_warmup)
    {
        return Demo::FixedSeq(1);
    }
    else
    {
        return _fixedSeq;
    }
}

Demo::FixedSeq
ThroughputI::echoFixedSeq(Demo::FixedSeq seq, const Ice::Current&)
{
    return seq;
}

void
ThroughputI::shutdown(const Ice::Current& current)
{
    current.adapter->getCommunicator()->shutdown();
}
