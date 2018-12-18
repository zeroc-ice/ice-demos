// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <NestedI.h>

using namespace std;
using namespace Demo;

int run(const Ice::CommunicatorPtr&);

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
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
run(const Ice::CommunicatorPtr& communicator)
{
    NestedPrx nested = NestedPrx::checkedCast(communicator->propertyToProxy("Nested.Proxy"));
    if(!nested)
    {
        cerr << "invalid proxy" << endl;
        return 1;
    }

    //
    // Ensure the invocation times out if the nesting level is too
    // high and there are no more threads in the thread pool to
    // dispatch the call.
    //
    nested = nested->ice_invocationTimeout(5000);

    Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("Nested.Client");
    NestedPrx self = NestedPrx::uncheckedCast(adapter->createProxy(Ice::stringToIdentity("nestedClient")));
    NestedPtr servant = new NestedI(self);
    adapter->add(servant, Ice::stringToIdentity("nestedClient"));
    adapter->activate();

    cout << "Note: The maximum nesting level is sz * 2, with sz being\n"
         << "the maximum number of threads in the server thread pool. if\n"
         << "you specify a value higher than that, the application will\n"
         << "block or timeout.\n"
         << endl;

    string s;
    do
    {
        try
        {
            cout << "enter nesting level or 'x' for exit: ";
            cin >> s;
            int level = atoi(s.c_str());
            if(level > 0)
            {
                nested->nestedCall(level, self);
            }
        }
        catch(const Ice::Exception& ex)
        {
            cerr << ex << endl;
        }
    }
    while(cin.good() && s != "x");

    return 0;
}
