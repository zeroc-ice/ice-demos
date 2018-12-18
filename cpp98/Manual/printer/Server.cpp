// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Printer.h>

using namespace std;
using namespace Demo;

class PrinterI : public Printer
{
public:

    virtual void printString(const string &, const Ice::Current&);
};

void
PrinterI::printString(const string &s, const Ice::Current&)
{
    cout << s << endl;
}

int
main(int argc, char* argv[])
{
    try
    {
        Ice::CommunicatorHolder ich(argc, argv);
        Ice::ObjectAdapterPtr adapter =
            ich->createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -h localhost -p 10000");
        Ice::ObjectPtr object = new PrinterI;
        adapter->add(object, Ice::stringToIdentity("SimplePrinter"));
        adapter->activate();

        ich->waitForShutdown();
    }
    catch(const std::exception& e)
    {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}
