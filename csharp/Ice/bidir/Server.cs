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

[assembly: AssemblyTitle("IceBidirServer")]
[assembly: AssemblyDescription("Ice bidir demo server")]
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

            var adapter = communicator().createObjectAdapter("Callback.Server");
            var sender = new CallbackSenderI(communicator());
            adapter.add(sender, Ice.Util.stringToIdentity("sender"));
            adapter.activate();

            var t = new Thread(new ThreadStart(sender.Run));
            t.Start();

            try
            {
                communicator().waitForShutdown();
            }
            finally
            {
                sender.destroy();
                t.Join();
            }

            return 0;
        }
    }

    public static int Main(string[] args)
    {
        var app = new App();
        return app.main(args, "config.server");
    }
}
