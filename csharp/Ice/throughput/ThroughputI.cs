// Copyright (c) ZeroC, Inc.

using Demo;

internal sealed class ThroughputI : ThroughputDisp_
{
    public ThroughputI()
    {
        _byteSeq = new byte[ByteSeqSize.value];
        _stringSeq = new string[StringSeqSize.value];
        for (int i = 0; i < StringSeqSize.value; ++i)
        {
            _stringSeq[i] = "hello";
        }
        _structSeq = new StringDouble[StringDoubleSeqSize.value];
        for (int i = 0; i < StringDoubleSeqSize.value; ++i)
        {
            _structSeq[i] = new StringDouble();
            _structSeq[i].s = "hello";
            _structSeq[i].d = 3.14;
        }
        _fixedSeq = new Fixed[FixedSeqSize.value];
        for (int i = 0; i < FixedSeqSize.value; ++i)
        {
            _fixedSeq[i].i = 0;
            _fixedSeq[i].j = 0;
            _fixedSeq[i].d = 0;
        }
    }

    public override bool needsWarmup(Ice.Current current)
    {
        _warmup = false;
        return _needsWarmup;
    }

    public override void startWarmup(Ice.Current current)
    {
        _warmup = true;
    }

    public override void endWarmup(Ice.Current current)
    {
        _warmup = false;
        _needsWarmup = false;
    }

    public override void sendByteSeq(byte[] seq, Ice.Current current)
    {
    }

    public override byte[] recvByteSeq(Ice.Current current)
    {
        if (_warmup)
        {
            return _warmupByteSeq;
        }
        else
        {
            return _byteSeq;
        }
    }

    public override byte[] echoByteSeq(byte[] seq, Ice.Current current)
    {
        return seq;
    }

    public override void sendStringSeq(string[] seq, Ice.Current current)
    {
    }

    public override string[] recvStringSeq(Ice.Current current)
    {
        if (_warmup)
        {
            return _warmupStringSeq;
        }
        else
        {
            return _stringSeq;
        }
    }

    public override string[] echoStringSeq(string[] seq, Ice.Current current)
    {
        return seq;
    }

    public override void sendStructSeq(StringDouble[] seq, Ice.Current current)
    {
    }

    public override StringDouble[] recvStructSeq(Ice.Current current)
    {
        if (_warmup)
        {
            return _warmupStructSeq;
        }
        else
        {
            return _structSeq;
        }
    }

    public override StringDouble[] echoStructSeq(StringDouble[] seq, Ice.Current current)
    {
        return seq;
    }

    public override void sendFixedSeq(Fixed[] seq, Ice.Current current)
    {
    }

    public override Fixed[] recvFixedSeq(Ice.Current current)
    {
        if (_warmup)
        {
            return _warmupFixedSeq;
        }
        else
        {
            return _fixedSeq;
        }
    }

    public override Fixed[] echoFixedSeq(Fixed[] seq, Ice.Current current)
    {
        return seq;
    }

    public override void shutdown(Ice.Current current)
    {
        current.adapter.getCommunicator().shutdown();
    }

    private readonly byte[] _byteSeq;
    private readonly string[] _stringSeq;
    private readonly StringDouble[] _structSeq;
    private readonly Fixed[] _fixedSeq;

    private readonly byte[] _warmupByteSeq = new byte[1];
    private readonly string[] _warmupStringSeq = new string[1];
    private readonly StringDouble[] _warmupStructSeq = new StringDouble[] { new() };
    private readonly Fixed[] _warmupFixedSeq = new Fixed[1];

    private bool _needsWarmup = true;
    private bool _warmup = false;
}
