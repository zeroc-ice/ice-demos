// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Reflection;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IcePrinterServer")]
[assembly: AssemblyDescription("Ice printer demo server")]
[assembly: AssemblyCompany("ZeroC, Inc.")]

public class PrinterI : Demo.PrinterDisp_
{
    public override void printString(string s, Ice.Current current)
    {
        Console.WriteLine(s);
    }
}

public class Server
{
    public static int Main(string[] args)
    {
        int status = 0;
        Ice.Communicator ic = null;
        try
        {
            ic = Ice.Util.initialize(ref args);
            var adapter = ic.createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -h localhost -p 10000");
            adapter.add(new PrinterI(), Ice.Util.stringToIdentity("SimplePrinter"));
            adapter.activate();
            ic.waitForShutdown();
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
