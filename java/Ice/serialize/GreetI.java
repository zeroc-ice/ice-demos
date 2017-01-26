// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class GreetI implements Greet
{
    @Override
    public void sendGreeting(MyGreeting greeting, com.zeroc.Ice.Current current)
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
    public void shutdown(com.zeroc.Ice.Current current)
    {
        System.out.println("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
