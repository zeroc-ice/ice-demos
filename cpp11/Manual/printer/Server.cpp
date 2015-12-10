// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Printer.h>

using namespace std;
using namespace Demo;

class PrinterI : public Printer
{
public:

    virtual void printString(string, const Ice::Current&);
};

void 
PrinterI::
printString(string s, const Ice::Current&)
{
    cout << s << endl;
}

int
main(int argc, char* argv[])
{
    int status = 0;
    try
    {
        auto ic = Ice::initialize(argc, argv);
        auto adapter =
            ic->createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -h localhost -p 10000");
        auto object = make_shared<PrinterI>();
        adapter->add(object, ic->stringToIdentity("SimplePrinter"));
        adapter->activate();
        ic->waitForShutdown();
    }
    catch(const exception& e)
    {
        cerr << e.what() << endl;
        status = 1;
    }
    return status;
}
