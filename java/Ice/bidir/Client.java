// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<String>();

        //
        // try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.client", extraArgs))
        {
            Runtime.getRuntime().addShutdownHook(new Thread(() ->
            {
                communicator.destroy();
            }));

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

        com.zeroc.Ice.ObjectAdapter adapter = communicator.createObjectAdapter("");
        com.zeroc.Ice.Identity ident = new com.zeroc.Ice.Identity();
        ident.name = java.util.UUID.randomUUID().toString();
        ident.category = "";
        adapter.add(new CallbackReceiverI(), ident);
        adapter.activate();
        server.ice_getConnection().setAdapter(adapter);
        server.addClient(ident);

        communicator.waitForShutdown();

        return 0;
    }
}
