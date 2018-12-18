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
    _warmup(false)
{
    Demo::StringDouble stringDoubleVal = { "hello", 3.14 };
    _structSeq = Demo::StringDoubleSeq(Demo::StringDoubleSeqSize, stringDoubleVal);

    Demo::Fixed fixedVal = { 0, 0, 0.0 };
    _fixedSeq = Demo::FixedSeq(Demo::FixedSeqSize, fixedVal);
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
ThroughputI::sendByteSeq(const std::pair<const Ice::Byte*, const Ice::Byte*>&, const Ice::Current&)
{
}

void
ThroughputI::recvByteSeq_async(const Demo::AMD_Throughput_recvByteSeqPtr& cb, const Ice::Current&)
{
    if(_warmup)
    {
        std::pair<const Ice::Byte*, const Ice::Byte*> ret;
        Demo::ByteSeq warmupBytesBuf(1);
        ret.first = &warmupBytesBuf[0];
        ret.second = ret.first + warmupBytesBuf.size();
        cb->ice_response(ret);
    }
    else
    {
        std::pair<const Ice::Byte*, const Ice::Byte*> ret;
        ret.first = &_byteSeq[0];
        ret.second = ret.first + _byteSeq.size();
        cb->ice_response(ret);
    }
}

void
ThroughputI::echoByteSeq_async(const Demo::AMD_Throughput_echoByteSeqPtr& cb,
                         const std::pair<const Ice::Byte*, const Ice::Byte*>& seq, const Ice::Current&)
{
    cb->ice_response(seq);
}

void
ThroughputI::sendStringSeq(const std::vector<Util::string_view>&, const Ice::Current&)
{
}

void
ThroughputI::recvStringSeq_async(const Demo::AMD_Throughput_recvStringSeqPtr& cb,
                                 const Ice::Current&)
{
    if(_warmup)
    {
        cb->ice_response(std::vector<Util::string_view>(1));
    }
    else
    {
        cb->ice_response(_stringViewSeq);
    }
}

void
ThroughputI::echoStringSeq_async(const Demo::AMD_Throughput_echoStringSeqPtr& cb,
                                 const std::vector<Util::string_view>& seq, const Ice::Current&)
{
    cb->ice_response(seq);
}

void
ThroughputI::sendStructSeq(const Demo::StringDoubleSeq&, const Ice::Current&)
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
ThroughputI::echoStructSeq(const Demo::StringDoubleSeq& seq, const Ice::Current&)
{
    return seq;
}

void
ThroughputI::sendFixedSeq(const Demo::FixedSeq&, const Ice::Current&)
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
ThroughputI::echoFixedSeq(const Demo::FixedSeq& seq, const Ice::Current&)
{
    return seq;
}

void
ThroughputI::shutdown(const Ice::Current& c)
{
    c.adapter->getCommunicator()->shutdown();
}
