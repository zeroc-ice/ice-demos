// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;

namespace Client
{
    public class Program
    {
        public static int Main(string[] args)
        {
            try
            {
                using(Ice.Communicator communicator = Ice.Util.initialize(ref args))
                {
                    var obj = communicator.stringToProxy("SimplePrinter:default -h localhost -p 10000");
                    var printer = PrinterPrxHelper.checkedCast(obj);
                    if(printer == null)
                    {
                        throw new ApplicationException("Invalid proxy");
                    }

                    printer.printString("Hello World!");
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
