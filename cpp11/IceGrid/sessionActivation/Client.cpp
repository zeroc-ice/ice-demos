// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
#include <Hello.h>

using namespace std;
using namespace Demo;

int run(const shared_ptr<Ice::Communicator>&);

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceLocatorDiscovery();
#endif

    int status = EXIT_SUCCESS;

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
            status = EXIT_FAILURE;
        }
        else
        {
            status = run(ich.communicator());
        }
    }
    catch(const std::exception& ex)
    {
        cerr << argv[0] << ": " << ex.what() << endl;
        status = EXIT_FAILURE;
    }

    return status;
}

void menu();
string trim(const string&);

int
run(const shared_ptr<Ice::Communicator>& communicator)
{
    int status = EXIT_SUCCESS;

    auto registry =
        Ice::checkedCast<IceGrid::RegistryPrx>(communicator->stringToProxy("DemoIceGrid/Registry"));
    if(!registry)
    {
        cerr << "could not contact registry" << endl;
        return EXIT_FAILURE;
    }

    shared_ptr<IceGrid::SessionPrx> session;
    while(!session)
    {
        cout << "This demo accepts any user-id / password combination.\n";

        string id;
        cout << "user id: " << flush;
        getline(cin, id);
        id = trim(id);

        string password;
        cout << "password: " << flush;
        getline(cin, password);
        password = trim(password);

        try
        {
            session = registry->createSession(id, password);
            break;
        }
        catch(const IceGrid::PermissionDeniedException& ex)
        {
            cout << "permission denied:\n" << ex.reason << endl;
        }
    }

    //
    // Enable heartbeats on the session connection to maintain the
    // connection alive even if idle.
    //
    session->ice_getConnection()->setACM(registry->getACMTimeout(), Ice::nullopt, Ice::ACMHeartbeat::HeartbeatOnIdle);

    try
    {
        auto hello = Ice::checkedCast<HelloPrx>(session->allocateObjectById(Ice::stringToIdentity("hello")));

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
    }
    catch(const IceGrid::AllocationException& ex)
    {
        cerr << "could not allocate object: " << ex.reason << endl;
        status = EXIT_FAILURE;
    }
    catch(const IceGrid::ObjectNotRegisteredException&)
    {
        cerr << "object not registered with registry" << endl;
        status = EXIT_FAILURE;
    }
    catch(const Ice::Exception& ex)
    {
        cerr << ex << endl;
        status = EXIT_FAILURE;
    }
    catch(...)
    {
        cerr << "unexpected exception" << endl;
        status = EXIT_FAILURE;
    }

    session->destroy();

    return status;
}

void
menu()
{
    cout <<
        "usage:\n"
        "t: send greeting\n"
        "x: exit\n"
        "?: help\n";
}

string
trim(const string& s)
{
    static const string delims = "\t\r\n ";
    string::size_type last = s.find_last_not_of(delims);
    if(last != string::npos)
    {
        return s.substr(s.find_first_not_of(delims), last+1);
    }
    return s;
}

