//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "Printer.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Demo;

class PrinterI final : public Printer
{
public:
    void printString(string s, const Ice::Current&) final;
};

void
PrinterI::printString(string s, const Ice::Current&)
{
    cout << s << endl;
}

int
main(int argc, char* argv[])
{
    try
    {
        const Ice::CommunicatorHolder ich(argc, argv);
        auto adapter = ich->createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -p 10000");
        auto servant = make_shared<PrinterI>();
        adapter->add(servant, Ice::stringToIdentity("SimplePrinter"));
        adapter->activate();

        ich->waitForShutdown();
    }
    catch (const std::exception& e)
    {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}
