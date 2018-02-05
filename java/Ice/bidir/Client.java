// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client extends com.zeroc.Ice.Application
{
    @Override
    public int run(String[] args)
    {
        if(args.length > 0)
        {
            System.err.println(appName() + ": too many arguments");
            return 1;
        }

        CallbackSenderPrx server = CallbackSenderPrx.checkedCast(
            communicator().propertyToProxy("CallbackSender.Proxy"));
        if(server == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }

        //
        // Create an object adapter with no name and no endpoints for receiving callbacks
        // over bidirectional connections.
        //
        com.zeroc.Ice.ObjectAdapter adapter = communicator().createObjectAdapter("");

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
        communicator().waitForShutdown();
        return 0;
    }

    public static void main(String[] args)
    {
        Client app = new Client();
        int status = app.main("Client", args, "config.client");
        System.exit(status);
    }
}
