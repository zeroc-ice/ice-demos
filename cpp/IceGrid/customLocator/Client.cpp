// Copyright (c) ZeroC, Inc.

#include "Hello.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Demo;

int run(const shared_ptr<Ice::Communicator>&, bool);

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
        if (argc > 2)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            if (argc == 2)
            {
                if (string(argv[1]) == "--context")
                {
                    status = run(ich.communicator(), true);
                }
                else
                {
                    cerr << "Usage: " << argv[0] << " [--context]" << endl;
                    status = 1;
                }
            }
            else
            {
                status = run(ich.communicator(), false);
            }
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
run(const shared_ptr<Ice::Communicator>& communicator, bool addContext)
{
    //
    // Add the context entry that allows the client to use the locator
    //
    if (addContext)
    {
        auto locator = communicator->getDefaultLocator();
        Ice::Context ctx;
        ctx["SECRET"] = "LetMeIn";
        communicator->setDefaultLocator(locator->ice_context(ctx));
    }

    //
    // Now we try to connect to the object with the `hello' identity.
    //
    auto hello = Ice::checkedCast<HelloPrx>(communicator->stringToProxy("hello"));
    if (!hello)
    {
        cerr << "couldn't find a `hello' object." << endl;
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
            if (c == 't')
            {
                hello->sayHello();
            }
            else if (c == 's')
            {
                hello->shutdown();
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
            "t: send greeting\n"
            "s: shutdown server\n"
            "x: exit\n"
            "?: help\n";
}
