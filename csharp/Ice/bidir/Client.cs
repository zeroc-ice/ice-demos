// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;

public class Client
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            //
            // using statement - communicator is automatically destroyed
            // at the end of this statement
            //
            using(var communicator = Ice.Util.initialize(ref args, "config.client"))
            {
                //
                // Destroy the communicator on Ctrl+C or Ctrl+Break
                //
                Console.CancelKeyPress += (sender, eventArgs) => communicator.destroy();

                if(args.Length > 0)
                {
                    Console.Error.WriteLine("too many arguments");
                    status = 1;
                }
                else
                {
                    status = run(communicator);
                }
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }

    private static int run(Ice.Communicator communicator)
    {
        var server = CallbackSenderPrxHelper.checkedCast(communicator.propertyToProxy("CallbackSender.Proxy"));
        if(server == null)
        {
            Console.Error.WriteLine("invalid proxy");
            return 1;
        }

        //
        // Create an object adapter with no name and no endpoints for receiving callbacks
        // over bidirectional connections.
        //
        var adapter = communicator.createObjectAdapter("");

        //
        // Register the callback receiver servant with the object adapter
        //
        var proxy = CallbackReceiverPrxHelper.uncheckedCast(adapter.addWithUUID(new CallbackReceiverI()));

        //
        // Associate the object adapter with the bidirectional connection.
        //
        server.ice_getConnection().setAdapter(adapter);

        //
        // Provide the proxy of the callback receiver object to the server and wait for
        // shutdown.
        //
        server.addClient(proxy);
        communicator.waitForShutdown();
        return 0;
    }
}
