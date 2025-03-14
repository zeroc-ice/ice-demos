// Copyright (c) ZeroC, Inc.

#pragma once

module Demo
{
    sequence<byte> ByteSeq;
    const int ByteSeqSize = 500000;

    sequence<string> StringSeq;
    const int StringSeqSize = 50000;

    struct StringDouble
    {
        string s;
        double d;
    }
    sequence<StringDouble> StringDoubleSeq;
    const int StringDoubleSeqSize = 50000;

    struct Fixed
    {
        int i;
        int j;
        double d;
    }
    sequence<Fixed> FixedSeq;
    const int FixedSeqSize = 50000;

    interface Throughput
    {
        bool needsWarmup();
        void startWarmup();
        void endWarmup();

        void sendByteSeq(["cpp:array"] ByteSeq seq);
        ["marshaled-result", "cpp:array"] ByteSeq recvByteSeq();
        ["marshaled-result", "cpp:array"] ByteSeq echoByteSeq(["cpp:array"] ByteSeq seq);

        void sendStringSeq(["cpp:view-type:std::vector<Util::string_view>"] StringSeq seq);
        ["marshaled-result", "cpp:view-type:std::vector<Util::string_view>"] StringSeq recvStringSeq();
        ["marshaled-result", "cpp:view-type:std::vector<Util::string_view>"] StringSeq echoStringSeq(
            ["cpp:view-type:std::vector<Util::string_view>"] StringSeq seq);

        void sendStructSeq(StringDoubleSeq seq);
        StringDoubleSeq recvStructSeq();
        StringDoubleSeq echoStructSeq(StringDoubleSeq seq);

        void sendFixedSeq(FixedSeq seq);
        FixedSeq recvFixedSeq();
        FixedSeq echoFixedSeq(FixedSeq seq);

        void shutdown();
    }
}
