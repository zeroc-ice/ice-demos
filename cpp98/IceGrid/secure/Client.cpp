// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
#include <Hello.h>

using namespace std;
using namespace Demo;

int run(const Ice::CommunicatorPtr&);

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceLocatorDiscovery();
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

void
menu()
{
    cout <<
        "usage:\n"
        "t: send greeting\n"
        "s: shutdown server\n"
        "x: exit\n"
        "?: help\n";
}

int
run(const Ice::CommunicatorPtr& communicator)
{
    //
    // First we try to connect to the object with the `hello'
    // identity. If it's not registered with the registry, we
    // search for an object with the ::Demo::Hello type.
    //
    HelloPrx hello;
    try
    {
        hello = HelloPrx::checkedCast(communicator->stringToProxy("hello"));
    }
    catch(const Ice::NotRegisteredException&)
    {
        IceGrid::QueryPrx query = IceGrid::QueryPrx::checkedCast(communicator->stringToProxy("DemoIceGrid/Query"));
        hello = HelloPrx::checkedCast(query->findObjectByType("::Demo::Hello"));
    }
    if(!hello)
    {
        cerr << "couldn't find a `::Demo::Hello' object." << endl;
        return 1;
    }

    menu();

    char c = 'x';
    do
    {
        try
        {
            cout << "==> ";
            cin >> c;
            if(c == 't')
            {
                hello->sayHello();
            }
            else if(c == 's')
            {
                hello->shutdown();
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

    return 0;
}
