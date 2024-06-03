// Copyright (c) ZeroC, Inc.

using Demo;

public sealed class CallbackReceiverI : CallbackReceiverDisp_
{
    public override void callback(Ice.Current current)
    {
        Console.WriteLine("received callback");
    }
}
