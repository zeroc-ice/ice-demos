// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;

namespace Server
{
    public class PrinterI : Demo.PrinterDisp_
    {
        public override void printString(string s, Ice.Current current)
        {
            Console.WriteLine(s);
        }
    }

    public class Program
    {
        public static int Main(string[] args)
        {
            try
            {
                using(Ice.Communicator communicator = Ice.Util.initialize(ref args))
                {
                    var adapter =
                        communicator.createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -h localhost -p 10000");
                    adapter.add(new PrinterI(), Ice.Util.stringToIdentity("SimplePrinter"));
                    adapter.activate();
                    communicator.waitForShutdown();
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
}
