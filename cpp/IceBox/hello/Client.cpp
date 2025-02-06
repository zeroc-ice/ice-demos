// Copyright (c) ZeroC, Inc.

#include "Hello.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Demo;

int run(const shared_ptr<Ice::Communicator>&);

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
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
        const Ice::CommunicatorHolder ich{argc, argv};

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if (argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            status = run(ich.communicator());
        }
    }
    catch (const std::exception& ex)
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
    HelloPrx twoway{communicator, "hello:tcp -h localhost -p 10000:udp -h localhost -p 10000"};

    auto oneway = twoway->ice_oneway();
    auto batchOneway = twoway->ice_batchOneway();
    auto datagram = twoway->ice_datagram();
    auto batchDatagram = twoway->ice_batchDatagram();

    menu();

    char c = 'x';
    do
    {
        try
        {
            cout << "==> ";
            cin >> c;
            if (c == 't')
            {
                twoway->sayHello();
            }
            else if (c == 'o')
            {
                oneway->sayHello();
            }
            else if (c == 'O')
            {
                batchOneway->sayHello();
            }
            else if (c == 'd')
            {
                datagram->sayHello();
            }
            else if (c == 'D')
            {
                batchDatagram->sayHello();
            }
            else if (c == 'f')
            {
                batchOneway->ice_flushBatchRequests();
                batchDatagram->ice_flushBatchRequests();

            }
            else if (c == 'x')
            {
                // Nothing to do
            }
            else if (c == '?')
            {
                menu();
            }
            else
            {
                cout << "unknown command `" << c << "'" << endl;
                menu();
            }
        }
        catch (const Ice::Exception& ex)
        {
            cerr << ex << endl;
        }
    } while (cin.good() && c != 'x');

    return 0;
}

void
menu()
{
    cout << "usage:\n"
            "t: send greeting as twoway\n"
            "o: send greeting as oneway\n"
            "O: send greeting as batch oneway\n"
            "d: send greeting as datagram\n"
            "D: send greeting as batch datagram\n"
            "f: flush all batch requests\n"
            "x: exit\n"
            "?: help\n";
}
