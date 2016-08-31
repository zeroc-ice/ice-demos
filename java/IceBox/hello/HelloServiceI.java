// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class HelloServiceI implements com.zeroc.IceBox.Service
{
    @Override
    public void start(String name, com.zeroc.Ice.Communicator communicator, String[] args)
    {
        _adapter = communicator.createObjectAdapter(name);
        _adapter.add(new HelloI(), com.zeroc.Ice.Util.stringToIdentity("hello"));
        _adapter.activate();
    }

    @Override
    public void stop()
    {
        _adapter.destroy();
    }

    private com.zeroc.Ice.ObjectAdapter _adapter;
}
