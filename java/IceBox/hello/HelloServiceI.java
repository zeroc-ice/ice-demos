// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class HelloServiceI implements IceBox.Service
{
    @Override
    public void
    start(String name, Ice.Communicator communicator, String[] args)
    {
        _adapter = communicator.createObjectAdapter(name);
        _adapter.add(new HelloI(), communicator.stringToIdentity("hello"));
        _adapter.activate();
    }

    @Override
    public void
    stop()
    {
        _adapter.destroy();
    }

    private Ice.ObjectAdapter _adapter;
}
