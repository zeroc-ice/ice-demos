// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Printer.h>

using namespace std;
using namespace Demo;

int
main(int argc, char * argv[])
{
    int status = 0;
    try
    {
        auto ic = Ice::initialize(argc, argv);
        auto printer = Ice::checkedCast<PrinterPrx>(ic->stringToProxy("SimplePrinter:default -h localhost -p 10000"));
        if(!printer)
        {
            throw "Invalid proxy";
        }
        printer->printString("Hello World!");
    }
    catch(const exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }
    return status;
}
