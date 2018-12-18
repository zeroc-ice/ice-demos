// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client
{
    static class ShutdownHook extends Thread
    {
        @Override
        public void
        run()
        {
            _communicator.destroy();
        }

        ShutdownHook(Ice.Communicator communicator)
        {
            _communicator = communicator;
        }

        private final Ice.Communicator _communicator;
    }

    public static void
    main(String[] args)
    {
        int status = 0;
        Ice.StringSeqHolder argsHolder = new Ice.StringSeqHolder(args);

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(Ice.Communicator communicator = Ice.Util.initialize(argsHolder, "config.client"))
        {
            //
            // Install shutdown hook to (also) destroy communicator during JVM shutdown.
            // This ensures the communicator gets destroyed when the user interrupts the application with Ctrl-C.
            //
            Runtime.getRuntime().addShutdownHook(new ShutdownHook(communicator));

            if(argsHolder.value.length > 0)
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                CallbackSenderPrx server = CallbackSenderPrxHelper.checkedCast(communicator.propertyToProxy("CallbackSender.Proxy"));

                if(server == null)
                {
                    System.err.println("invalid proxy");
                    status = 1;
                }
                else
                {
                     //
                    // Create an object adapter with no name and no endpoints for receiving callbacks
                    // over bidirectional connections.
                    //
                    Ice.ObjectAdapter adapter = communicator.createObjectAdapter("");

                    //
                    // Register the callback receiver servant with the object adapter and activate
                    // the adapter.
                    //
                    CallbackReceiverPrx proxy =
                        CallbackReceiverPrxHelper.uncheckedCast(adapter.addWithUUID(new CallbackReceiverI()));
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

                    communicator.waitForShutdown();
                }
            }
        }

        System.exit(status);
    }
}
