// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
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
#ifdef _WIN32
wmain(int argc, wchar_t* argv[])
#else
main(int argc, char* argv[])
#endif
{
    try
    {
        Ice::CommunicatorHolder icHolder = Ice::initialize(argc, argv);
        Ice::ObjectAdapterPtr adapter =
            icHolder->createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -h localhost -p 10000");
        Ice::ObjectPtr object = new PrinterI;
        adapter->add(object, Ice::stringToIdentity("SimplePrinter"));
        adapter->activate();
        icHolder->waitForShutdown();
    }
    catch(const std::exception& e)
    {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}
