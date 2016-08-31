// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class HelloPluginI implements com.zeroc.Ice.Plugin
{
    public HelloPluginI(com.zeroc.Ice.Communicator communicator)
    {
        _communicator = communicator;
    }

    @Override
    public void initialize()
    {
        com.zeroc.Ice.ObjectAdapter adapter = _communicator.createObjectAdapter("Hello");
        adapter.add(new HelloI(), com.zeroc.Ice.Util.stringToIdentity("hello"));
        adapter.activate();
    }

    @Override
    public void destroy()
    {
    }

    private com.zeroc.Ice.Communicator _communicator;
}
