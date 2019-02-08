//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.IceDiscovery.replication.Demo.*;

public class HelloI implements Hello
{
    public HelloI(String name)
    {
        _name = name;
    }

    @Override
    public String getGreeting(com.zeroc.Ice.Current current)
    {
        return _name + " says Hello World!";
    }

    @Override
    public void shutdown(com.zeroc.Ice.Current current)
    {
        System.out.println("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }

    final private String _name;
}
