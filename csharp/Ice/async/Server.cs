// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Reflection;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IceAsyncServer")]
[assembly: AssemblyDescription("Ice async demo server")]
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

            callbackOnInterrupt();

            var adapter = communicator().createObjectAdapter("Hello");
            _workQueue = new WorkQueue();
            adapter.add(new HelloI(_workQueue), Ice.Util.stringToIdentity("hello"));

            _workQueue.Start();
            adapter.activate();

            communicator().waitForShutdown();
            _workQueue.Join();
            return 0;
        }

        public override void interruptCallback(int sig)
        {
            _workQueue.destroy();
            communicator().shutdown();
        }

        private WorkQueue _workQueue;
    }

    public static int Main(string[] args)
    {
        var app = new App();
        return app.main(args, "config.server");
    }
}
