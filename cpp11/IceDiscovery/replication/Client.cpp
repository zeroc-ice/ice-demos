// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Hello.h>

using namespace std;
using namespace Demo;

int run(const shared_ptr<Ice::Communicator>&);

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
    Ice::registerIceDiscovery(false);
#endif

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

int
run(const shared_ptr<Ice::Communicator>& communicator)
{
    //
    // Get the hello proxy. We configure the proxy to not cache the
    // server connection with the proxy and to disable the locator
    // cache. With this configuration, the IceDiscovery locator will be
    // queried for each invocation on the proxy.
    //
    auto obj = communicator->stringToProxy("hello");
    obj = obj->ice_connectionCached(false);
    obj = obj->ice_locatorCacheTimeout(0);

    auto hello = Ice::checkedCast<HelloPrx>(obj);
    if(!hello)
    {
        cerr << "couldn't find object `hello'." << endl;
        return 1;
    }

    string s;
    do
    {
        cout << "enter the number of iterations: ";
        cin >> s;
        int count = atoi(s.c_str());
        cout << "enter the delay between each greetings (in ms): ";
        cin >> s;
        int delay = atoi(s.c_str());
        if(delay < 0)
        {
            delay = 500; // 500 milli-seconds
        }

        for(int i = 0; i < count; i++)
        {
            cout << hello->getGreeting() << endl;
            this_thread::sleep_for(chrono::milliseconds(delay));
        }
    }
    while(cin.good() && s != "x");

    return 0;
}
