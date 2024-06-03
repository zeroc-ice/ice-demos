// Copyright (c) ZeroC, Inc.

using Demo;

public sealed class CallbackReceiverI : CallbackReceiverDisp_
{
    public override void callback(int num, Ice.Current current)
    {
        System.Console.Out.WriteLine("received callback #" + num);
    }
}
