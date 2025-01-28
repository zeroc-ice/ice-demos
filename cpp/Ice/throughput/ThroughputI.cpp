// Copyright (c) ZeroC, Inc.

#include <Ice/Ice.h>
#include <ThroughputI.h>

ThroughputI::ThroughputI()
    : _byteSeq(Demo::ByteSeqSize),
      _stringSeq(Demo::StringSeqSize, "hello"),
      _structSeq(Demo::StringDoubleSeqSize, {"hello", 3.14}),
      _fixedSeq(Demo::FixedSeqSize, {0, 0, 0.0})
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
ThroughputI::sendByteSeq(std::pair<const std::byte*, const std::byte*>, const Ice::Current&)
{
}

Demo::Throughput::RecvByteSeqMarshaledResult
ThroughputI::recvByteSeq(const Ice::Current& current)
{
    if (_warmup)
    {
        Demo::ByteSeq warmupBytesBuf(1);
        const std::pair<const std::byte*, const std::byte*> ret =
            std::make_pair(warmupBytesBuf.data(), warmupBytesBuf.data() + warmupBytesBuf.size());
        return {ret, current};
    }
    else
    {
        const std::pair<const std::byte*, const std::byte*> ret =
            std::make_pair(_byteSeq.data(), _byteSeq.data() + _byteSeq.size());
        return {ret, current};
    }
}

Demo::Throughput::EchoByteSeqMarshaledResult
ThroughputI::echoByteSeq(std::pair<const std::byte*, const std::byte*> seq, const Ice::Current& current)
{
    return {seq, current};
}

void
ThroughputI::sendStringSeq(std::vector<std::string>, const Ice::Current&)
{
}

Demo::Throughput::RecvStringSeqMarshaledResult
ThroughputI::recvStringSeq(const Ice::Current& current)
{
    if (_warmup)
    {
        return {std::vector<std::string>(1), current};
    }
    else
    {
        return {_stringSeq, current};
    }
}

Demo::Throughput::EchoStringSeqMarshaledResult
ThroughputI::echoStringSeq(std::vector<std::string> seq, const Ice::Current& current)
{
    return {seq, current};
}

void
ThroughputI::sendStructSeq(Demo::StringDoubleSeq, const Ice::Current&)
{
}

Demo::StringDoubleSeq
ThroughputI::recvStructSeq(const Ice::Current&)
{
    if (_warmup)
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
    if (_warmup)
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
