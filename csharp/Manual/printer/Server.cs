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
        try
        {
            using(Ice.Communicator ic = Ice.Util.initialize(ref args))
            {
                var adapter =
                    ic.createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -h localhost -p 10000");
                adapter.add(new PrinterI(), Ice.Util.stringToIdentity("SimplePrinter"));
                adapter.activate();
                ic.waitForShutdown();
            }
        }
        catch(Exception e)
        {
            Console.Error.WriteLine(e);
            return 1;
        }
        return 0;
    }
}
