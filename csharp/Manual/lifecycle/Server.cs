// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using FilesystemI;
using System;
using System.Reflection;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IceLifecycleServer")]
[assembly: AssemblyDescription("Ice lifecycle demo server")]
[assembly: AssemblyCompany("ZeroC, Inc.")]

public class Server
{
    class App : Ice.Application
    {
        public override int run(string[] args)
        {
            //
            // Terminate cleanly on receipt of a signal.
            //
            shutdownOnInterrupt();

            //
            // Create an object adapter
            //
            var adapter = communicator().createObjectAdapterWithEndpoints(
                "LifecycleFilesystem", "default -h localhost -p 10000");

            //
            // Create the root directory.
            //
            var root = new DirectoryI();
            var id = new Ice.Identity();
            id.name = "RootDir";
            adapter.add(root, id);

            //
            // All objects are created, allow client requests now.
            //
            adapter.activate();

            //
            // Wait until we are done.
            //
            communicator().waitForShutdown();
            if(interrupted())
            {
                Console.Error.WriteLine(appName() + ": received signal, shutting down");
            }

            return 0;
        }
    }

    static public int Main(string[] args)
    {
        var app = new App();
        return app.main(args);
    }
}
