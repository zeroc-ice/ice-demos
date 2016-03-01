// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
using System.Diagnostics;

class ValueFactory
{
    public static Ice.Object create(string type)
    {
        if(type.Equals("::Demo::Printer"))
        {
            return new PrinterI();
        }

        if(type.Equals("::Demo::DerivedPrinter"))
        {
            return new DerivedPrinterI();
        }

        if(type.Equals("::Demo::ClientPrinter"))
        {
            return new ClientPrinterI();
        }

        Debug.Assert(false);
        return null;
    }
}
