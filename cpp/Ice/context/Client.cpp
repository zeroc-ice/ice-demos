//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "Context.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Demo;

int run(const shared_ptr<Ice::Communicator>&);

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
        const Ice::CommunicatorHolder ich(argc, argv, "config.client");

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
    auto proxy = Ice::checkedCast<ContextPrx>(communicator->propertyToProxy("Context.Proxy"));
    if (!proxy)
    {
        cerr << "invalid proxy" << endl;
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
            if (c == '1')
            {
                proxy->call();
            }
            else if (c == '2')
            {
                Ice::Context ctx;
                ctx["type"] = "Explicit";
                proxy->call(ctx);
            }
            else if (c == '3')
            {
                Ice::Context ctx;
                ctx["type"] = "Per-Proxy";
                auto proxy2 = proxy->ice_context(ctx);
                proxy2->call();
            }
            else if (c == '4')
            {
                auto ic = communicator->getImplicitContext();
                Ice::Context ctx;
                ctx["type"] = "Implicit";
                ic->setContext(ctx);
                proxy->call();
                ic->setContext(Ice::Context());
            }
            else if (c == 's')
            {
                proxy->shutdown();
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
            "1: use no request context\n"
            "2: use explicit request context\n"
            "3: use per-proxy request context\n"
            "4: use implicit request context\n"
            "s: shutdown server\n"
            "x: exit\n"
            "?: help\n";
}
