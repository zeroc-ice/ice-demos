// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <ValueFactory.h>
#include <ValueI.h>

using namespace std;

Ice::ObjectPtr
ValueFactory::create(const string& type)
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
