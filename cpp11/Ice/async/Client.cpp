// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Hello.h>
#include <exception>

using namespace std;
using namespace Demo;

class AsyncClient : public Ice::Application
{
public:

    AsyncClient();

    virtual int run(int, char*[]);

private:

    void menu();
};

AsyncClient::AsyncClient() :
    //
    // Since this is an interactive demo we don't want any signal
    // handling.
    //
    Ice::Application(Ice::NoSignalHandling)
{
}

int
main(int argc, char* argv[])
{
    AsyncClient app;
    return app.main(argc, argv, "config.client");
}

int
AsyncClient::run(int argc, char* argv[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto hello = Ice::checkedCast<HelloPrx>(communicator()->propertyToProxy("Hello.Proxy"));
    if(!hello)
    {
        cerr << argv[0] << ": invalid proxy" << endl;
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

    return EXIT_SUCCESS;
}

void
AsyncClient::menu()
{
    cout <<
        "usage:\n"
        "i: send immediate greeting\n"
        "d: send delayed greeting\n"
        "s: shutdown server\n"
        "x: exit\n"
        "?: help\n";
}
