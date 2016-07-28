// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using IceBox;

class HelloServiceI : Service
{
    public void start(string name, Ice.Communicator communicator, string[] args)
    {
        _adapter = communicator.createObjectAdapter("Hello-" + name);
        
        string helloIdentity = communicator.getProperties().getProperty("Hello.Identity");
        _adapter.add(new HelloI(name), Ice.Util.stringToIdentity(helloIdentity));
        _adapter.activate();
    }

    public void stop()
    {
        _adapter.destroy();
    }

    private Ice.ObjectAdapter _adapter;
}
