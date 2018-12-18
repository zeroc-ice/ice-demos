// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Client
{
    public static void
    main(String[] args) throws Exception
    {
        try(Ice.Communicator ic = Ice.Util.initialize(args))
        {
            Ice.ObjectPrx base = ic.stringToProxy("SimplePrinter:default -h localhost -p 10000");
            Demo.PrinterPrx printer = Demo.PrinterPrxHelper.checkedCast(base);
            if(printer == null)
            {
                throw new Error("Invalid proxy");
            }

            printer.printString("Hello World!");
        }
    }
}
