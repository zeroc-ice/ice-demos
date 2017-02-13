// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Reflection;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IceDiscoveryReplicationServer")]
[assembly: AssemblyDescription("IceDiscovery replication demo server")]
[assembly: AssemblyCompany("ZeroC, Inc.")]

public class Server
{
    class App : Ice.Application
    {
        public override int run(string[] args)
        {
            if(args.Length > 0)
            {
                System.Console.Error.WriteLine(appName() + ": too many arguments");
                return 1;
            }

            Ice.ObjectAdapter adapter = communicator().createObjectAdapter("Hello");
            HelloI hello = new HelloI(communicator().getProperties().getProperty("Ice.ProgramName"));
            adapter.add(hello, communicator().stringToIdentity("hello"));
            adapter.activate();
            communicator().waitForShutdown();
            return 0;
        }
    }

    public static int Main(string[] args)
    {
        App app = new App();
        return app.main(args);
    }
}
