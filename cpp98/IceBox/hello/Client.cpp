// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Hello.h>

using namespace std;
using namespace Demo;

int run(const Ice::CommunicatorPtr&);

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
    Ice::registerIceUDP();
    Ice::registerIceWS();
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
        "t: send greeting as twoway\n"
        "o: send greeting as oneway\n"
        "O: send greeting as batch oneway\n"
        "d: send greeting as datagram\n"
        "D: send greeting as batch datagram\n"
        "f: flush all batch requests\n"
        "S: switch secure mode on/off\n"
        "x: exit\n"
        "?: help\n";
}

int
run(const Ice::CommunicatorPtr& communicator)
{
    HelloPrx twoway = HelloPrx::checkedCast(
        communicator->propertyToProxy("Hello.Proxy")->ice_twoway()->ice_timeout(-1)->ice_secure(false));
    if(!twoway)
    {
        cerr << "invalid proxy" << endl;
        return 1;
    }
    HelloPrx oneway = twoway->ice_oneway();
    HelloPrx batchOneway = twoway->ice_batchOneway();
    HelloPrx datagram = twoway->ice_datagram();
    HelloPrx batchDatagram = twoway->ice_batchDatagram();

    bool secure = false;

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
                twoway->sayHello();
            }
            else if(c == 'o')
            {
                oneway->sayHello();
            }
            else if(c == 'O')
            {
                batchOneway->sayHello();
            }
            else if(c == 'd')
            {
                if(secure)
                {
                    cout << "secure datagrams are not supported" << endl;
                }
                else
                {
                    datagram->sayHello();
                }
            }
            else if(c == 'D')
            {
                if(secure)
                {
                    cout << "secure datagrams are not supported" << endl;
                }
                else
                {
                    batchDatagram->sayHello();
                }
            }
            else if(c == 'f')
            {
                batchOneway->ice_flushBatchRequests();
                if(!secure)
                {
                    batchDatagram->ice_flushBatchRequests();
                }
            }
            else if(c == 'S')
            {
                secure = !secure;

                twoway = twoway->ice_secure(secure);
                oneway = oneway->ice_secure(secure);
                batchOneway = batchOneway->ice_secure(secure);
                datagram = datagram->ice_secure(secure);
                batchDatagram = batchDatagram->ice_secure(secure);

                if(secure)
                {
                    cout << "secure mode is now on" << endl;
                }
                else
                {
                    cout << "secure mode is now off" << endl;
                }
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
