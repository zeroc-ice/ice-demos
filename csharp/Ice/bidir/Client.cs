// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Reflection;

public class Client
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

            var server = CallbackSenderPrxHelper.checkedCast(communicator().propertyToProxy("CallbackSender.Proxy"));
            if(server == null)
            {
                Console.Error.WriteLine("invalid proxy");
                return 1;
            }

            //
            // Create an object adapter with no name and no endpoints for receiving callbacks
            // over bidirectional connections.
            //
            var adapter = communicator().createObjectAdapter("");

            //
            // Register the callback receiver servant with the object adapter and activate
            // the adapter.
            //
            var proxy = CallbackReceiverPrxHelper.uncheckedCast(adapter.addWithUUID(new CallbackReceiverI()));
            adapter.activate();

            //
            // Associate the object adapter with the bidirectional connection.
            //
            server.ice_getConnection().setAdapter(adapter);

            //
            // Provide the proxy of the callback receiver object to the server and wait for
            // shutdown.
            //
            server.addClient(proxy);
            communicator().waitForShutdown();
            return 0;
        }
    }

    public static int Main(string[] args)
    {
        var app = new App();
        return app.main(args, "config.client");
    }
}
