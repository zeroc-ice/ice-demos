// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Printer.h>
#include <stdexcept>

using namespace std;
using namespace Demo;

int
main(int argc, char * argv[])
{
    try
    {
        Ice::CommunicatorHolder icHolder = Ice::initialize(argc, argv);
        Ice::ObjectPrx base = icHolder->stringToProxy("SimplePrinter:default -h localhost -p 10000");
        PrinterPrx printer = PrinterPrx::checkedCast(base);
        if(!printer)
        {
            throw std::runtime_error("Invalid proxy");
        }

        printer->printString("Hello World!");
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
