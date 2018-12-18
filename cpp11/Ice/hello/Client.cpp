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

void menu();

int
run(const shared_ptr<Ice::Communicator>& communicator)
{
    auto twoway = Ice::checkedCast<HelloPrx>(
        communicator->propertyToProxy("Hello.Proxy")->ice_twoway()->ice_secure(false));
    if(!twoway)
    {
        cerr << "invalid proxy" << endl;
        return 1;
    }
    auto oneway = twoway->ice_oneway();
    auto batchOneway = twoway->ice_batchOneway();
    auto datagram = twoway->ice_datagram();
    auto batchDatagram = twoway->ice_batchDatagram();

    bool secure = false;
    int timeout = -1;
    int delay = 0;

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
                twoway->sayHello(delay);
            }
            else if(c == 'o')
            {
                oneway->sayHello(delay);
            }
            else if(c == 'O')
            {
                batchOneway->sayHello(delay);
            }
            else if(c == 'd')
            {
                if(secure)
                {
                    cout << "secure datagrams are not supported" << endl;
                }
                else
                {
                    datagram->sayHello(delay);
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
                    batchDatagram->sayHello(delay);
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
            else if(c == 'T')
            {
                if(timeout == -1)
                {
                    timeout = 2000;
                }
                else
                {
                    timeout = -1;
                }

                twoway = twoway->ice_invocationTimeout(timeout);
                oneway = oneway->ice_invocationTimeout(timeout);
                batchOneway = batchOneway->ice_invocationTimeout(timeout);

                if(timeout == -1)
                {
                    cout << "timeout is now switched off" << endl;
                }
                else
                {
                    cout << "timeout is now set to 2000ms" << endl;
                }
            }
            else if(c == 'P')
            {
                if(delay == 0)
                {
                    delay = 2500;
                }
                else
                {
                    delay = 0;
                }

                if(delay == 0)
                {
                    cout << "server delay is now deactivated" << endl;
                }
                else
                {
                    cout << "server delay is now set to 2500ms" << endl;
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
            else if(c == 's')
            {
                twoway->shutdown();
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
        "T: set a timeout\n"
        "P: set server delay\n"
        "S: switch secure mode on/off\n"
        "s: shutdown server\n"
        "x: exit\n"
        "?: help\n";
}
