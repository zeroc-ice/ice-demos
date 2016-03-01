// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;

public class HelloI : HelloDisp_
{
    public HelloI(string name)
    {
        _name = name;
    }
    
    public override string getGreeting(Ice.Current current)
    {
        return _name + " says Hello World!";
    }
    
    public override void shutdown(Ice.Current current)
    {
        System.Console.Out.WriteLine("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }

    private string _name;
}
