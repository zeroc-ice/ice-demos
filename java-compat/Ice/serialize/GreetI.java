// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class GreetI extends _GreetDisp
{
    @Override
    public void
    sendGreeting(MyGreeting greeting, Ice.Current current)
    {
        if(greeting != null)
        {
            System.out.println(greeting.text);
        }
        else
        {
            System.out.println("Received null greeting");
        }
    }

    @Override
    public void
    shutdown(Ice.Current current)
    {
        System.out.println("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
