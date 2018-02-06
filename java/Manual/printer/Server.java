// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server
{
    public static void main(String[] args) throws Exception
    {
        try(com.zeroc.Ice.Communicator ic = com.zeroc.Ice.Util.initialize(args))
        {
            //
            // Install shutdown hook for user interrupt like Ctrl-C
            //
            Runtime.getRuntime().addShutdownHook(new Thread(() ->
            {
                //
                // Initiate communicator shutdown, waitForShutdown returns when complete
                // calling shutdown on a destroyed communicator is no-op
                //
                ic.shutdown();
            }));

            com.zeroc.Ice.ObjectAdapter adapter =
                ic.createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -h localhost -p 10000");
            com.zeroc.Ice.Object object = new PrinterI();
            adapter.add(object, com.zeroc.Ice.Util.stringToIdentity("SimplePrinter"));
            adapter.activate();
            ic.waitForShutdown();
        }
    }
}
