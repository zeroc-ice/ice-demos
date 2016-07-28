// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Reflection;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IceGridSimpleServer")]
[assembly: AssemblyDescription("IceGrid simple demo server")]
[assembly: AssemblyCompany("ZeroC, Inc.")]

public class Server
{
    public class App : Ice.Application
    {
        public override int run(string[] args)
        {
            if(args.Length > 0)
            {
                System.Console.Error.WriteLine(appName() + ": too many arguments");
                return 1;
            }

            var adapter = communicator().createObjectAdapter("Hello");
            var properties = communicator().getProperties();
            var id = Ice.Util.stringToIdentity(properties.getProperty("Identity"));
            adapter.add(new HelloI(properties.getProperty("Ice.ProgramName")), id);
            adapter.activate();
            communicator().waitForShutdown();
            return 0;
        }
    }

    static public int Main(string[] args)
    {
        var app = new App();
        return app.main(args);
    }
}
