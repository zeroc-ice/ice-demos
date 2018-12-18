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

    ThroughputI();

    virtual bool needsWarmup(const Ice::Current&) override;
    virtual void startWarmup(const Ice::Current&) override;
    virtual void endWarmup(const Ice::Current&) override;

    virtual void sendByteSeq(std::pair<const Ice::Byte*, const Ice::Byte*>, const Ice::Current&) override;
    virtual RecvByteSeqMarshaledResult recvByteSeq(const Ice::Current&) override;
    virtual EchoByteSeqMarshaledResult echoByteSeq(std::pair<const Ice::Byte*, const Ice::Byte*>, const Ice::Current&) override;

    virtual void sendStringSeq(std::vector<Util::string_view>, const Ice::Current&) override;
    virtual RecvStringSeqMarshaledResult recvStringSeq(const Ice::Current&) override;
    virtual EchoStringSeqMarshaledResult echoStringSeq(std::vector<Util::string_view>, const Ice::Current&) override;

    virtual void sendStructSeq(Demo::StringDoubleSeq, const Ice::Current&) override;
    virtual Demo::StringDoubleSeq recvStructSeq(const Ice::Current&) override;
    virtual Demo::StringDoubleSeq echoStructSeq(Demo::StringDoubleSeq, const Ice::Current&) override;

    virtual void sendFixedSeq(Demo::FixedSeq, const Ice::Current&) override;
    virtual Demo::FixedSeq recvFixedSeq(const Ice::Current&) override;
    virtual Demo::FixedSeq echoFixedSeq(Demo::FixedSeq, const Ice::Current&) override;

    virtual void shutdown(const Ice::Current&) override;

private:

    Demo::ByteSeq _byteSeq;
    Demo::StringSeq _stringSeq;
    std::vector<Util::string_view> _stringViewSeq;
    Demo::StringDoubleSeq _structSeq;
    Demo::FixedSeq _fixedSeq;

    bool _warmup;
};

#endif
