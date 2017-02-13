// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;

public sealed class CallbackReceiverI : CallbackReceiverDisp_
{
    public override void callback(Ice.Current current)
    {
        System.Console.Out.WriteLine("received callback");
    }
}
