// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Reflection;

public class Server
{
    class App : Ice.Application
    {
        public override int run(string[] args)
        {
            if(args.Length > 0)
            {
                Console.Error.WriteLine(appName() + ": too many arguments");
                return 1;
            }

            var adapter = communicator().createObjectAdapter("Context");
            adapter.add(new ContextI(), Ice.Util.stringToIdentity("context"));
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
