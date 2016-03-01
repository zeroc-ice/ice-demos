// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
#include <Hello.h>

using namespace std;
using namespace Demo;

class HelloClient : public Ice::Application
{
public:

    HelloClient();
    virtual int run(int, char*[]);

private:

    void menu();
    void usage();
};

int
main(int argc, char* argv[])
{
    HelloClient app;
    return app.main(argc, argv, "config.client");
}

HelloClient::HelloClient() :
    //
    // Since this is an interactive demo we don't want any signal
    // handling.
    //
    Ice::Application(Ice::NoSignalHandling)
{
}

int
HelloClient::run(int argc, char* argv[])
{
    if(argc > 2)
    {
        usage();
        return EXIT_FAILURE;
    }

    bool addContext = false;
    if(argc == 2)
    {
        if(string(argv[1]) == "--context")
        {
            addContext = true;
        }
        else
        {
            usage();
            return EXIT_FAILURE;
        }
    }

    //
    // Add the context entry that allows the client to use the locator
    //
    if(addContext)
    {
        Ice::LocatorPrx locator = communicator()->getDefaultLocator();
        Ice::Context ctx;
        ctx["SECRET"] = "LetMeIn";
        communicator()->setDefaultLocator(locator->ice_context(ctx));
    }

    //
    // Now we try to connect to the object with the `hello' identity.
    //
    HelloPrx hello = HelloPrx::checkedCast(communicator()->stringToProxy("hello"));
    if(!hello)
    {
        cerr << argv[0] << ": couldn't find a `hello' object." << endl;
        return EXIT_FAILURE;
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
            else if(c == 't')
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

    return EXIT_SUCCESS;
}

void
HelloClient::menu()
{
    cout <<
        "usage:\n"
        "t: send greeting\n"
        "s: shutdown server\n"
        "x: exit\n"
        "?: help\n";
}

void
HelloClient::usage()
{
     cerr << "Usage: " << appName() << " [--context]" << endl;
}

