// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>

#include <CounterObserverI.h>

using namespace std;
using namespace Demo;

int run(const Ice::CommunicatorPtr&);

int
main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config.client");

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if(argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            status = run(ich.communicator());
        }
    }
    catch(const std::exception& ex)
    {
        cerr << argv[0] << ": " << ex.what() << endl;
        status = 1;
    }

    return status;
}

void
menu(const MTPrinterPtr& printer)
{
    printer->print(
        "usage:\n"
        "i: increment the counter\n"
        "d: decrement the counter\n"
        "x: exit\n"
        "?: help\n");
}

int
run(const Ice::CommunicatorPtr& communicator)
{
    Ice::PropertiesPtr properties = communicator->getProperties();

    const string proxyProperty = "Counter.Proxy";
    string proxy = properties->getProperty(proxyProperty);
    if(proxy.empty())
    {
        cerr << "property `" << proxyProperty << "' not set" << endl;
        return 1;
    }

    CounterPrx counter = CounterPrx::uncheckedCast(communicator->stringToProxy(proxy));
    if(!counter)
    {
        cerr << "invalid proxy" << endl;
        return 1;
    }

    MTPrinterPtr printer = new MTPrinter();

    Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapterWithEndpoints("Observer", "tcp");
    CounterObserverPrx observer =
        CounterObserverPrx::uncheckedCast(adapter->addWithUUID(new CounterObserverI(printer)));
    adapter->activate();

    counter->subscribe(observer);

    menu(printer);

    char c = 'x';
    do
    {
        try
        {
            printer->print("==> ");
            cin >> c;
            if(c == 'i')
            {
                counter->inc(1);
            }
            else if(c == 'd')
            {
                counter->inc(-1);
            }
            else if(c == 'x')
            {
                // Nothing to do
            }
            else if(c == '?')
            {
                menu(printer);
            }
            else
            {
                cout << "unknown command `" << c << "'" << endl;
                menu(printer);
            }
        }
        catch(const Ice::Exception& ex)
        {
            cerr << ex << endl;
        }
    }
    while(cin.good() && c != 'x');

    counter->unsubscribe(observer);

    return 0;
}
