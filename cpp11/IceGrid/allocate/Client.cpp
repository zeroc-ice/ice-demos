// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
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
string trim(const string&);

int
run(const shared_ptr<Ice::Communicator>& communicator)
{

    int status = 0;

    auto registry = Ice::checkedCast<IceGrid::RegistryPrx>(
        communicator->stringToProxy("DemoIceGrid/Registry"));
    if(!registry)
    {
        cerr << "could not contact registry" << endl;
        return 1;
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
    session->ice_getConnection()->setACM(registry->getACMTimeout(), Ice::nullopt, Ice::ACMHeartbeat::HeartbeatAlways);

    try
    {
        //
        // First try to retrieve object by identity, which will work
        // if the application-single.xml descriptor is used. Otherwise
        // we retrieve object by type, which will succeed if the
        // application-multiple.xml descriptor is used.
        //
        shared_ptr<HelloPrx> hello;
        try
        {
            hello = Ice::checkedCast<HelloPrx>(session->allocateObjectById(Ice::stringToIdentity("hello")));
        }
        catch(const IceGrid::ObjectNotRegisteredException&)
        {
            hello = Ice::checkedCast<HelloPrx>(session->allocateObjectByType("::Demo::Hello"));
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
    }
    catch(const IceGrid::AllocationException& ex)
    {
        cerr << "could not allocate object: " << ex.reason << endl;
        status = 1;
    }
    catch(const std::exception& ex)
    {
        cerr << "unexpected exception: " << ex.what() << endl;
        status = 1;
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
        "s: shutdown server\n"
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
