// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;

public class HelloI : HelloDisp_
{
    public override void sayHello(int delay, Ice.Current current)
    {
        if(delay > 0)
        {
            System.Threading.Thread.Sleep(delay);
        }
        System.Console.Out.WriteLine("Hello World!");
    }
    
    public override void shutdown(Ice.Current current)
    {
        System.Console.Out.WriteLine("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
