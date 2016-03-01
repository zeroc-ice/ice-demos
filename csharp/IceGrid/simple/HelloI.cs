// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using Demo;

public class HelloI : HelloDisp_
{
    public HelloI(string name)
    {
        _name = name;
    }

    public override void sayHello(Ice.Current current)
    {
        Console.WriteLine(_name + " says Hello World!");
    }

    public override void shutdown(Ice.Current current)
    {
        Console.WriteLine(_name + " shutting down...");
        current.adapter.getCommunicator().shutdown();
    }

    private string _name;
}
