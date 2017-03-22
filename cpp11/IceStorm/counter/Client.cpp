// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <CounterObserverI.h>

using namespace std;
using namespace Demo;

class Client : public Ice::Application
{
public:

    virtual int run(int, char*[]) override;

private:

    void menu();
};

int
main(int argc, char* argv[])
{
    Client app;
    return app.main(argc, argv, "config.client");
}

int
Client::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto properties = communicator()->getProperties();

    const string proxyProperty = "Counter.Proxy";
    auto proxy = properties->getProperty(proxyProperty);
    if(proxy.empty())
    {
        cerr << appName() << ": property `" << proxyProperty << "' not set" << endl;
        return EXIT_FAILURE;
    }

    auto counter = Ice::uncheckedCast<CounterPrx>(communicator()->stringToProxy(proxy));
    if(!counter)
    {
        cerr << appName() << ": invalid proxy" << endl;
        return EXIT_FAILURE;
    }

    auto adapter = communicator()->createObjectAdapterWithEndpoints("Observer", "tcp");
    auto observer =
        Ice::uncheckedCast<CounterObserverPrx>(adapter->addWithUUID(make_shared<CounterObserverI>()));
    adapter->activate();

    counter->subscribe(observer);

    menu();

    char c = 'x';
    do
    {
        try
        {
            print("==> ");
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
                menu();
            }
            else
            {
                cout << "unknown command `" << c << "'" << endl;
                menu();
            }
        }
        catch(const Ice::Exception& ex)
        {
            cerr << ex << endl;
        }
    }
    while(cin.good() && c != 'x');

    counter->unsubscribe(observer);

    return EXIT_SUCCESS;
}

void
Client::menu()
{
    print("usage:\n"
          "i: increment the counter\n"
          "d: decrement the counter\n"
          "x: exit\n"
          "?: help\n");
}
