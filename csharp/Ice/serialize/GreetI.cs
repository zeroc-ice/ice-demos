// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;

public class GreetI : GreetDisp_
{
    public override void sendGreeting(MyGreeting greeting, Ice.Current current)
    {
        if(greeting != null)
        {
            System.Console.Out.WriteLine(greeting.text);
        }
        else
        {
            System.Console.Out.WriteLine("Received null greeting!");
        }
    }
    
    public override void shutdown(Ice.Current current)
    {
        System.Console.Out.WriteLine("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
