// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;

public sealed class CallbackReceiverI : CallbackReceiverDisp_
{
    public override void callback(Ice.Current current)
    {
        Console.WriteLine("received callback");
    }
}
