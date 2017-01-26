// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Reflection;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IceNestedServer")]
[assembly: AssemblyDescription("Ice nested demo server")]
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

            var adapter = communicator().createObjectAdapter("Nested.Server");
            var self = 
                NestedPrxHelper.uncheckedCast(adapter.createProxy(Ice.Util.stringToIdentity("nestedServer")));
            adapter.add(new NestedI(self), Ice.Util.stringToIdentity("nestedServer"));
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
