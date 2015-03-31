// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <ObjectFactory.h>
#include <ValueI.h>

using namespace std;

Ice::ObjectPtr
ObjectFactory::create(const string& type)
{
    if(type == Demo::Printer::ice_staticId())
    {
        return new PrinterI;
    }

    if(type == Demo::DerivedPrinter::ice_staticId())
    {
        return new DerivedPrinterI;
    }

    if(type == Demo::ClientPrinter::ice_staticId())
    {
        return new ClientPrinterI;
    }

    assert(false);
    return 0;
}

void
ObjectFactory::destroy()
{
    // Nothing to do
}
