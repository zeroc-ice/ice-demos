// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client
{
    static class ShutdownHook implements Runnable
    {
        private Ice.Communicator communicator;

        ShutdownHook(Ice.Communicator communicator)
        {
            this.communicator = communicator;
        }

        @Override
        public void
        run()
        {
            //
            // Destroy communicator to abandon ongoing remote calls
            // calling destroy multiple times is no-op
            //
            communicator.destroy();
        }
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
            // Install shutdown hook for user interrupt like Ctrl-C
            //
            Runtime.getRuntime().addShutdownHook(new Thread(new ShutdownHook(communicator)));

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
