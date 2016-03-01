// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Reflection;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IceValueServer")]
[assembly: AssemblyDescription("Ice value demo server")]
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

            communicator().getValueFactoryManager().add(ValueFactory.create, Demo.Printer.ice_staticId());

            Ice.ObjectAdapter adapter = communicator().createObjectAdapter("Value");
            Ice.Object @object = new InitialI(adapter);
            adapter.add(@object, communicator().stringToIdentity("initial"));
            adapter.activate();
            communicator().waitForShutdown();
            return 0;
        }
    }

    public static int Main(string[] args)
    {
        App app = new App();
        return app.main(args, "config.server");
    }
}
