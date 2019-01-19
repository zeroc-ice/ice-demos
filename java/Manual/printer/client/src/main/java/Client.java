//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Manual.printer.Demo.*;

public class Client
{
    public static void main(String[] args)
    {
        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator ic = com.zeroc.Ice.Util.initialize(args))
        {
            ic.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Manual.printer");
            com.zeroc.Ice.ObjectPrx base = ic.stringToProxy("SimplePrinter:default -h localhost -p 10000");
            PrinterPrx printer = PrinterPrx.checkedCast(base);
            if(printer == null)
            {
                throw new Error("Invalid proxy");
            }

            printer.printString("Hello World!");
        }
    }
}
