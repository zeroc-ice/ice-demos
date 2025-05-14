// Copyright (c) ZeroC, Inc.

#include "Hello.h"
#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
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
string trim(const string&);

int
run(const shared_ptr<Ice::Communicator>& communicator)
{
    int status = 0;

    auto registry = Ice::checkedCast<IceGrid::RegistryPrx>(communicator->stringToProxy("DemoIceGrid/Registry"));
    if (!registry)
    {
        cerr << "could not contact registry" << endl;
        return 1;
    }

    optional<IceGrid::SessionPrx> session;
    while (!session)
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
        catch (const IceGrid::PermissionDeniedException& ex)
        {
            cout << "permission denied:\n" << ex.reason << endl;
        }
    }

    try
    {
        auto hello = Ice::checkedCast<HelloPrx>(session->allocateObjectById(Ice::Identity{"hello"}));

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
    }
    catch (const IceGrid::AllocationException& ex)
    {
        cerr << "could not allocate object: " << ex.reason << endl;
        status = 1;
    }
    catch (const IceGrid::ObjectNotRegisteredException&)
    {
        cerr << "object not registered with registry" << endl;
        status = 1;
    }
    catch (const std::exception& ex)
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
    cout << "usage:\n"
            "t: send greeting\n"
            "x: exit\n"
            "?: help\n";
}

string
trim(const string& s)
{
    static const string delims = "\t\r\n ";
    const string::size_type last = s.find_last_not_of(delims);
    if (last != string::npos)
    {
        return s.substr(s.find_first_not_of(delims), last + 1);
    }
    return s;
}
