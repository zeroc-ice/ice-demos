// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Reflection;

public class Server
{
    public class App : Ice.Application
    {
        public override int run(string[] args)
        {
            var adapter = communicator().createObjectAdapter("Hello");
            var discoverAdapter = communicator().createObjectAdapter("Discover");

            var hello = adapter.addWithUUID(new HelloI());
            discoverAdapter.add(new DiscoverI(hello), Ice.Util.stringToIdentity("discover"));

            discoverAdapter.activate();
            adapter.activate();

            communicator().waitForShutdown();
            return 0;
        }
    }

    public static int Main(string[] args)
    {
        var app = new App();
        return app.main(args, "config.server");
    }
}
