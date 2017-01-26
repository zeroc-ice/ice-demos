// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class HelloPluginI : Ice.Plugin
{
    public HelloPluginI(Ice.Communicator communicator)
    {
        _communicator = communicator;
    }

    public void initialize()
    {
        var adapter = _communicator.createObjectAdapter("Hello");
        adapter.add(new HelloI(), Ice.Util.stringToIdentity("hello"));
        adapter.activate();
    }

    public void destroy()
    {
    }

    private Ice.Communicator _communicator;
}
