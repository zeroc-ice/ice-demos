// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server
{
    public static void
    main(String[] args)
    {
        int status = 0;
        Ice.Communicator ic = null;
        try
        {
            ic = Ice.Util.initialize(args);
            Ice.ObjectAdapter adapter = ic.createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -h localhost -p 10000");
            Ice.Object object = new PrinterI();
            adapter.add(object, Ice.Util.stringToIdentity("SimplePrinter"));
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
