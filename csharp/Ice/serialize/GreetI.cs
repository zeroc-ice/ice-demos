// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;

public class GreetI : GreetDisp_
{
    public override void sendGreeting(MyGreeting greeting, Ice.Current current)
    {
        if(greeting != null)
        {
            Console.Out.WriteLine(greeting.text);
        }
        else
        {
            Console.Out.WriteLine("Received null greeting!");
        }
    }
    
    public override void shutdown(Ice.Current current)
    {
        Console.Out.WriteLine("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
