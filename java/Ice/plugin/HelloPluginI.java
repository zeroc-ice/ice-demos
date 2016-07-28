// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************



public class HelloPluginI implements Ice.Plugin
{
    public
    HelloPluginI(Ice.Communicator communicator)
    {
        _communicator = communicator;
    }

    @Override
    public void
    initialize()
    {
        Ice.ObjectAdapter adapter = _communicator.createObjectAdapter("Hello");
        adapter.add(new HelloI(), Ice.Util.stringToIdentity("hello"));
        adapter.activate();
    }

    @Override
    public void
    destroy()
    {
    }

    private Ice.Communicator _communicator;
}
