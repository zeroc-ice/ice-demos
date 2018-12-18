// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class PrinterI extends Demo._PrinterDisp
{
    @Override public void
    printString(String s, Ice.Current current)
    {
        System.out.println(s);
    }
}
