//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef THROUGHPUT_I_H
#define THROUGHPUT_I_H

#include "Throughput.h"

class ThroughputI final : public Demo::Throughput
{
public:
    ThroughputI();

    bool needsWarmup(const Ice::Current&) final;
    void startWarmup(const Ice::Current&) final;
    void endWarmup(const Ice::Current&) final;

    void sendByteSeq(std::pair<const std::byte*, const std::byte*>, const Ice::Current&) final;
    RecvByteSeqMarshaledResult recvByteSeq(const Ice::Current&) final;
    EchoByteSeqMarshaledResult echoByteSeq(std::pair<const std::byte*, const std::byte*>, const Ice::Current&) final;

    void sendStringSeq(std::vector<std::string>, const Ice::Current&) final;
    RecvStringSeqMarshaledResult recvStringSeq(const Ice::Current&) final;
    EchoStringSeqMarshaledResult echoStringSeq(std::vector<std::string>, const Ice::Current&) final;

    void sendStructSeq(Demo::StringDoubleSeq, const Ice::Current&) final;
    Demo::StringDoubleSeq recvStructSeq(const Ice::Current&) final;
    Demo::StringDoubleSeq echoStructSeq(Demo::StringDoubleSeq, const Ice::Current&) final;

    void sendFixedSeq(Demo::FixedSeq, const Ice::Current&) final;
    Demo::FixedSeq recvFixedSeq(const Ice::Current&) final;
    Demo::FixedSeq echoFixedSeq(Demo::FixedSeq, const Ice::Current&) final;

    void shutdown(const Ice::Current&) final;

private:
    Demo::ByteSeq _byteSeq;
    Demo::StringSeq _stringSeq;
    Demo::StringDoubleSeq _structSeq;
    Demo::FixedSeq _fixedSeq;

    bool _warmup{false};
};

#endif
