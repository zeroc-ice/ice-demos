// Copyright (c) ZeroC, Inc.

public class HelloPluginI(Ice.Communicator communicator) : Ice.Plugin
{
    public void initialize()
    {
        var adapter = _communicator.createObjectAdapter("Hello");
        adapter.add(new HelloI(), Ice.Util.stringToIdentity("hello"));
        adapter.activate();
    }

    public void destroy()
    {
    }

    private Ice.Communicator _communicator = communicator;
}
