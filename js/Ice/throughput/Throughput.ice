// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

module Demo
{

sequence<byte> ByteSeq;
const int ByteSeqSize = 50000;

sequence<string> StringSeq;
const int StringSeqSize = 5000;

struct StringDouble
{
    string s;
    double d;
};
sequence<StringDouble> StringDoubleSeq;
const int StringDoubleSeqSize = 5000;

struct Fixed
{
    int i;
    int j;
    double d;
};
sequence<Fixed> FixedSeq;
const int FixedSeqSize = 5000;

interface Throughput
{
    bool needsWarmup();
    void startWarmup();
    void endWarmup();

    void sendByteSeq(ByteSeq seq);
    ByteSeq recvByteSeq();
    ByteSeq echoByteSeq(ByteSeq seq);

    void sendStringSeq(StringSeq seq);
    StringSeq recvStringSeq();
    StringSeq echoStringSeq(StringSeq seq);

    void sendStructSeq(StringDoubleSeq seq);
    StringDoubleSeq recvStructSeq();
    StringDoubleSeq echoStructSeq(StringDoubleSeq seq);

    void sendFixedSeq(FixedSeq seq);
    FixedSeq recvFixedSeq();
    FixedSeq echoFixedSeq(FixedSeq seq);

    void shutdown();
};

};
