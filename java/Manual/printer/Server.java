// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server
{
    public static void main(String[] args)
    {
        int status = 0;
        com.zeroc.Ice.Communicator ic = null;
        try
        {
            com.zeroc.Ice.Util.InitializeResult ir = com.zeroc.Ice.Util.initialize(args);
            ic = ir.communicator;
            com.zeroc.Ice.ObjectAdapter adapter =
                ic.createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -h localhost -p 10000");
            com.zeroc.Ice.Object object = new PrinterI();
            adapter.add(object, com.zeroc.Ice.Util.stringToIdentity("SimplePrinter"));
            adapter.activate();
            ic.waitForShutdown();
        }
        catch(Exception e)
        {
            e.printStackTrace();
            status = 1;
        }
        if(ic != null)
        {
            try
            {
                ic.destroy();
            }
            catch(Exception e)
            {
                System.err.println(e.getMessage());
                status = 1;
            }
        }
        System.exit(status);
    }
}
