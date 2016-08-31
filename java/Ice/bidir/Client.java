// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
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

        com.zeroc.Ice.ObjectAdapter adapter = communicator().createObjectAdapter("");
        com.zeroc.Ice.Identity ident = new com.zeroc.Ice.Identity();
        ident.name = java.util.UUID.randomUUID().toString();
        ident.category = "";
        adapter.add(new CallbackReceiverI(), ident);
        adapter.activate();
        server.ice_getConnection().setAdapter(adapter);
        server.addClient(ident);
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
