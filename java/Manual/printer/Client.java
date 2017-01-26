// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        com.zeroc.Ice.Communicator ic = null;
        try
        {
            com.zeroc.Ice.Util.InitializeResult ir = com.zeroc.Ice.Util.initialize(args);
            ic = ir.communicator;
            com.zeroc.Ice.ObjectPrx base = ic.stringToProxy("SimplePrinter:default -h localhost -p 10000");
            Demo.PrinterPrx printer = Demo.PrinterPrx.checkedCast(base);
            if(printer == null)
            {
                throw new Error("Invalid proxy");
            }

            printer.printString("Hello World!");
        }
        catch(com.zeroc.Ice.LocalException e)
        {
            e.printStackTrace();
            status = 1;
        }
        catch(Exception e)
        {
            System.err.println(e.getMessage());
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
