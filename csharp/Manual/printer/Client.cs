// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Reflection;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IcePrinterClient")]
[assembly: AssemblyDescription("Ice printer demo client")]
[assembly: AssemblyCompany("ZeroC, Inc.")]

public class Client
{
    public static int Main(string[] args)
    {
        int status = 0;
        Ice.Communicator ic = null;
        try
        {
            ic = Ice.Util.initialize(ref args);
            var obj = ic.stringToProxy("SimplePrinter:default -h localhost -p 10000");
            var printer = PrinterPrxHelper.checkedCast(obj);
            if(printer == null)
            {
                throw new ApplicationException("Invalid proxy");
            }

            printer.printString("Hello World!");
        }
        catch(Exception e)
        {
            Console.Error.WriteLine(e);
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
                Console.Error.WriteLine(e);
                status = 1;
            }
        }
        return status;
    }
}
