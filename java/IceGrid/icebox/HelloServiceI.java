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
        _adapter = communicator.createObjectAdapter("Hello-" + name);

        String helloIdentity = communicator.getProperties().getProperty("Hello.Identity");
        _adapter.add(new HelloI(name), com.zeroc.Ice.Util.stringToIdentity(helloIdentity));
        _adapter.activate();
    }

    @Override
    public void stop()
    {
        _adapter.destroy();
    }

    private com.zeroc.Ice.ObjectAdapter _adapter;
}
