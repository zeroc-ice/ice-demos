// Copyright (c) ZeroC, Inc.

internal class HelloServiceI : IceBox.Service
{
    public void start(string name, Ice.Communicator communicator, string[] args)
    {
        _adapter = communicator.createObjectAdapter(name);
        _adapter.add(new HelloI(), Ice.Util.stringToIdentity("hello"));
        _adapter.activate();
    }

    public void stop()
    {
        _adapter.destroy();
    }

    private Ice.ObjectAdapter _adapter;
}
