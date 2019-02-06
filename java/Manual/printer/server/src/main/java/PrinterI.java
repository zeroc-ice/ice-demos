//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Manual.printer.Demo.*;

public class PrinterI implements Printer
{
    @Override public void printString(String s, com.zeroc.Ice.Current current)
    {
        System.out.println(s);
    }
}
