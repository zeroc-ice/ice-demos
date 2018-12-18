// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Session.h>

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

void
menu()
{
    cout <<
        "usage:\n"
        "c:     create a new per-client hello object\n"
        "0-9:   send a greeting to a hello object\n"
        "s:     shutdown the server and exit\n"
        "x:     exit\n"
        "t:     exit without destroying the session\n"
        "?:     help\n";
}

int
run(const Ice::CommunicatorPtr& communicator)
{
    string name;
    cout << "Please enter your name ==> ";
    cin >> name;
    if(!cin.good())
    {
        return 1;
    }

    Ice::ObjectPrx base = communicator->propertyToProxy("SessionFactory.Proxy");
    SessionFactoryPrx factory = SessionFactoryPrx::checkedCast(base);
    if(!factory)
    {
        cerr << "invalid proxy" << endl;
        return 1;
    }

    SessionPrx session = factory->create(name);

    vector<HelloPrx> hellos;

    menu();

    bool destroy = true;
    bool shutdown = false;
    do
    {
        cout << "==> ";
        char c;
        cin >> c;
        if(cin.good())
        {
            if(isdigit(c))
            {
                string s;
                s += c;
                vector<HelloPrx>::size_type index = atoi(s.c_str());
                if(index < hellos.size())
                {
                    hellos[index]->sayHello();
                }
                else
                {
                    cout << "Index is too high. " << hellos.size() << " hello objects exist so far.\n"
                         << "Use `c' to create a new hello object." << endl;
                }
            }
            else if(c == 'c')
            {
                hellos.push_back(session->createHello());
                cout << "Created hello object " << hellos.size() - 1 << endl;
            }
            else if(c == 's')
            {
                destroy = false;
                shutdown = true;
                break;
            }
            else if(c == 'x')
            {
                break;
            }
            else if(c == 't')
            {
                destroy = false;
                break;
            }
            else if(c == '?')
            {
                menu();
            }
            else
            {
                cout << "Unknown command `" << c << "'." << endl;
                menu();
            }
        }
    } while(cin.good());

    if(cin.good() && destroy)
    {
        session->destroy();
    }

    if(shutdown)
    {
        factory->shutdown();
    }

    return 0;
}
