// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Filesystem;
using System;
using System.Reflection;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IceLifecycleClient")]
[assembly: AssemblyDescription("Ice lifecycle demo client")]
[assembly: AssemblyCompany("ZeroC, Inc.")]

public class Client
{
    public class App : Ice.Application
    {
        public override int run(String[] args)
        {
            //
            // Terminate cleanly on receipt of a signal.
            //
            shutdownOnInterrupt();

            //
            // Create a proxy for the root directory
            //
            var @base = communicator().stringToProxy("RootDir:default -h localhost -p 10000");

            //
            // Down-cast the proxy to a Directory proxy.
            //
            var rootDir = DirectoryPrxHelper.checkedCast(@base);
            if(rootDir == null)
            {
                throw new Error("Invalid proxy");
            }

            var p = new Parser(rootDir);
            return p.parse();
        }

        private class Error : SystemException
        {
            public Error(String msg)
                : base(msg)
            {
            }
        }
    }

    static public int Main(String[] args)
    {
        var app = new App();
        var data = new Ice.InitializationData();
        return app.main(args, data);
    }
}
