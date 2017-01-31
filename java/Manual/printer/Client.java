// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Client
{
    public static void main(String[] args) throws Exception
    {
        try(com.zeroc.Ice.Communicator ic = com.zeroc.Ice.Util.initialize(args).communicator)
        {
            com.zeroc.Ice.ObjectPrx base = ic.stringToProxy("SimplePrinter:default -h localhost -p 10000");
            Demo.PrinterPrx printer = Demo.PrinterPrx.checkedCast(base);
            if(printer == null)
            {
                throw new Error("Invalid proxy");
            }

            printer.printString("Hello World!");
        }
    }
}
