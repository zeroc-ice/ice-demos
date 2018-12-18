// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server
{
    public static void
    main(String[] args)
    {
        try(Ice.Communicator ic = Ice.Util.initialize(args))
        {
            Ice.ObjectAdapter adapter =
                ic.createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -h localhost -p 10000");
            Ice.Object object = new PrinterI();
            adapter.add(object, Ice.Util.stringToIdentity("SimplePrinter"));
            adapter.activate();
            ic.waitForShutdown();
        }
    }
}
