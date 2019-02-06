//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.bidir.Demo.*;

public class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<String>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.client", extraArgs))
        {
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Ice.bidir");
            //
            // Install shutdown hook to (also) destroy communicator during JVM shutdown.
            // This ensures the communicator gets destroyed when the user interrupts the application with Ctrl-C.
            //
            Runtime.getRuntime().addShutdownHook(new Thread(() -> communicator.destroy()));

            if(!extraArgs.isEmpty())
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                status = run(communicator);
            }
        }

        System.exit(status);
    }

    private static int run(com.zeroc.Ice.Communicator communicator)
    {
        CallbackSenderPrx server = CallbackSenderPrx.checkedCast(
            communicator.propertyToProxy("CallbackSender.Proxy"));
        if(server == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }

        //
        // Create an object adapter with no name and no endpoints for receiving callbacks
        // over bidirectional connections.
        //
        com.zeroc.Ice.ObjectAdapter adapter = communicator.createObjectAdapter("");

        //
        // Register the callback receiver servant with the object adapter and activate
        // the adapter.
        //
        CallbackReceiverPrx proxy = CallbackReceiverPrx.uncheckedCast(adapter.addWithUUID(new CallbackReceiverI()));
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
        return 0;
    }
}
