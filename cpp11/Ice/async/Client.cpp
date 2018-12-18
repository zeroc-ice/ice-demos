// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Hello.h>
#include <exception>

using namespace std;

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
    auto hello = Ice::checkedCast<Demo::HelloPrx>(communicator->propertyToProxy("Hello.Proxy"));
    if(!hello)
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
            if(c == 'i')
            {
                hello->sayHello(0);
            }
            else if(c == 'd')
            {
                hello->sayHelloAsync(
                    5000,
                    nullptr,
                    [](exception_ptr e)
                    {
                        try
                        {
                            rethrow_exception(e);
                        }
                        catch(const exception& ex)
                        {
                            cerr << "sayHello AMI call failed:\n" << ex.what() << endl;
                        }
                    });
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

void
menu()
{
    cout <<
        "usage:\n"
        "i: send immediate greeting\n"
        "d: send delayed greeting\n"
        "s: shutdown server\n"
        "x: exit\n"
        "?: help\n";
}
