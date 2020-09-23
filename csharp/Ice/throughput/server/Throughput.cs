// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Collections.Generic;
using ZeroC.Ice;

namespace Demo
{
    public sealed class Throughput : IThroughput
    {
        private readonly byte[] _byteSeq;
        private readonly Fixed[] _fixedSeq;
        private bool _needsWarmup = true;
        private readonly string[] _stringSeq;
        private readonly StringDouble[] _structSeq;
        private bool _warmup;
        private readonly byte[] _warmupByteSeq = new byte[1];
        private readonly string[] _warmupStringSeq = new string[] { "hello" };
        private readonly Fixed[] _warmupFixedSeq = new Fixed[1];
        private readonly StringDouble[] _warmupStructSeq = new StringDouble[] { new StringDouble("hello", 3.14) };

        public Throughput()
        {
            _byteSeq = new byte[Constants.ByteSeqSize];
            _stringSeq = new string[Constants.StringSeqSize];
            for (int i = 0; i < Constants.StringSeqSize; ++i)
            {
                _stringSeq[i] = "hello";
            }
            _structSeq = new StringDouble[Constants.StringDoubleSeqSize];
            for (int i = 0; i < Constants.StringDoubleSeqSize; ++i)
            {
                _structSeq[i] = new StringDouble("hello", 3.14);
            }
            _fixedSeq = new Fixed[Constants.FixedSeqSize];
            for (int i = 0; i < Constants.FixedSeqSize; ++i)
            {
                _fixedSeq[i].I = 0;
                _fixedSeq[i].J = 0;
                _fixedSeq[i].D = 0;
            }
        }

        public bool NeedsWarmup(Current current)
        {
            _warmup = false;
            return _needsWarmup;
        }

        public void StartWarmup(Current current) => _warmup = true;

        public void EndWarmup(Current current)
        {
            _warmup = false;
            _needsWarmup = false;
        }

        public void SendByteSeq(byte[] seq, Current current)
        {
        }

        public void SendOnewayByteSeq(byte[] seq, Current current)
        {
        }

        public ReadOnlyMemory<byte> RecvByteSeq(Current current) => _warmup ? _warmupByteSeq : _byteSeq;

        public ReadOnlyMemory<byte> EchoByteSeq(byte[] seq, Current current) => seq;

        public void SendStringSeq(string[] seq, Current current)
        {
        }

        public void SendOnewayStringSeq(string[] seq, Current current)
        {
        }

        public IEnumerable<string> RecvStringSeq(Current current) => _warmup ? _warmupStringSeq : _stringSeq;

        public IEnumerable<string> EchoStringSeq(string[] seq, Current current) => seq;

        public void SendStructSeq(StringDouble[] seq, Current current)
        {
        }

        public void SendOnewayStructSeq(StringDouble[] seq, Current current)
        {
        }

        public IEnumerable<StringDouble> RecvStructSeq(Current current) => _warmup ? _warmupStructSeq : _structSeq;

        public IEnumerable<StringDouble> EchoStructSeq(StringDouble[] seq, Current current) => seq;

        public void SendFixedSeq(Fixed[] seq, Current current)
        {
        }

        public void SendOnewayFixedSeq(Fixed[] seq, Current current)
        {
        }

        public IEnumerable<Fixed> RecvFixedSeq(Current current) => _warmup ? _warmupFixedSeq : _fixedSeq;
        public IEnumerable<Fixed> EchoFixedSeq(Fixed[] seq, Current current) => seq;
        public void Shutdown(Current current) => current.Adapter.Communicator.DisposeAsync();
    }

}
