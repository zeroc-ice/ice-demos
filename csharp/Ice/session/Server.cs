// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Threading;
using System.Reflection;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IceSessionServer")]
[assembly: AssemblyDescription("Ice session demo server")]
[assembly: AssemblyCompany("ZeroC, Inc.")]

public class Server
{
    public class App : Ice.Application
    {
        public override int run(string[] args)
        {
            if(args.Length > 0)
            {
                Console.Error.WriteLine(appName() + ": too many arguments");
                return 1;
            }

            var adapter = communicator().createObjectAdapter("SessionFactory");

            var reaper = new ReapThread();
            var reaperThread = new Thread(new ThreadStart(reaper.run));
            reaperThread.Start();

            adapter.add(new SessionFactoryI(reaper), Ice.Util.stringToIdentity("SessionFactory"));
            adapter.activate();
            communicator().waitForShutdown();

            reaper.terminate();
            reaperThread.Join();

            return 0;
        }
    }

    public static int Main(string[] args)
    {
        var app = new App();
        return app.main(args, "config.server");
    }
}
