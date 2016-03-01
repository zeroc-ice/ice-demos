// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

class ObjectFactory implements Ice.ObjectFactory
{
    @Override
    public Ice.Object
    create(String type)
    {
        if(type.equals("::Demo::Printer"))
        {
            return new PrinterI();
        }

        if(type.equals("::Demo::DerivedPrinter"))
        {
            return new DerivedPrinterI();
        }

        if(type.equals("::Demo::ClientPrinter"))
        {
            return new ClientPrinterI();
        }

        assert(false);
        return null;
    }

    @Override
    public void
    destroy()
    {
        // Nothing to do
    }
}
