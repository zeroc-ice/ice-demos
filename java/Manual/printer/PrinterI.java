// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class PrinterI implements Demo.Printer
{
    @Override public void printString(String s, com.zeroc.Ice.Current current)
    {
        System.out.println(s);
    }
}
