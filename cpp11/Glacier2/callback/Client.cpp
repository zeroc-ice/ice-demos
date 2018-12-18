// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Glacier2/Glacier2.h>
#include <CallbackI.h>

using namespace std;
using namespace Demo;

void run(const shared_ptr<Ice::Communicator>&);

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
            run(ich.communicator());
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

void
run(const shared_ptr<Ice::Communicator>& communicator)
{
    shared_ptr<Glacier2::RouterPrx> router = Ice::checkedCast<Glacier2::RouterPrx>(communicator->getDefaultRouter());
    shared_ptr<Glacier2::SessionPrx> session;
    while(!session)
    {
        cout << "This demo accepts any user-id / password combination.\n";

        string id;
        cout << "user id: " << flush;
        getline(cin, id);

        string pw;
        cout << "password: " << flush;
        getline(cin, pw);

        try
        {
            session = router->createSession(id, pw);
            break;
        }
        catch(const Glacier2::PermissionDeniedException& ex)
        {
            cout << "permission denied:\n" << ex.reason << endl;
        }
        catch(const Glacier2::CannotCreateSessionException& ex)
        {
            cout << "cannot create session:\n" << ex.reason << endl;
        }
    }

    Ice::Int acmTimeout = router->getACMTimeout();
    Ice::ConnectionPtr connection = router->ice_getCachedConnection();
    assert(connection);
    connection->setACM(acmTimeout, IceUtil::None, Ice::ACMHeartbeat::HeartbeatAlways);
    connection->setCloseCallback([](Ice::ConnectionPtr)
                                 {
                                     cout << "The Glacier2 session has been destroyed." << endl;
                                 });

    Ice::Identity callbackReceiverIdent;
    callbackReceiverIdent.name = "cabackReceiver";
    callbackReceiverIdent.category = router->getCategoryForClient();

    Ice::Identity callbackReceiverFakeIdent;
    callbackReceiverFakeIdent.name = "cabackReceiver";
    callbackReceiverFakeIdent.category = "fake";

    auto base = communicator->propertyToProxy("Callback.Proxy");
    auto twoway = Ice::checkedCast<CallbackPrx>(base);
    auto oneway = twoway->ice_oneway();
    auto batchOneway = twoway->ice_batchOneway();

    auto adapter = communicator->createObjectAdapterWithRouter("", router);
    adapter->activate();

    adapter->add(make_shared<CallbackReceiverI>(), callbackReceiverIdent);

    // Should never be called for the fake identity.
    adapter->add(make_shared<CallbackReceiverI>(), callbackReceiverFakeIdent);

    auto twowayR = Ice::uncheckedCast<CallbackReceiverPrx>(adapter->createProxy(callbackReceiverIdent));
    auto onewayR = twowayR->ice_oneway();

    string override;
    bool fake = false;

    menu();

    char c = 'x';
    do
    {
        cout << "==> ";
        cin >> c;
        if(c == 't')
        {
            Ice::Context context;
            context["_fwd"] = "t";
            if(!override.empty())
            {
                context["_ovrd"] = override;
            }
            twoway->initiateCallback(twowayR, context);
        }
        else if(c == 'o')
        {
            Ice::Context context;
            context["_fwd"] = "o";
            if(!override.empty())
            {
                context["_ovrd"] = override;
            }
            oneway->initiateCallback(onewayR, context);
        }
        else if(c == 'O')
        {
            Ice::Context context;
            context["_fwd"] = "O";
            if(!override.empty())
            {
                context["_ovrd"] = override;
            }
            batchOneway->initiateCallback(onewayR, context);
        }
        else if(c == 'f')
        {
            batchOneway->ice_flushBatchRequests();
        }
        else if(c == 'v')
        {
            if(override.empty())
            {
                override = "some_value";
                cout << "override context field is now `" << override << "'" << endl;
            }
            else
            {
                override.clear();
                cout << "override context field is empty" << endl;
            }
        }
        else if(c == 'F')
        {
            fake = !fake;

            if(fake)
            {
                twowayR = Ice::uncheckedCast<CallbackReceiverPrx>(twowayR->ice_identity(callbackReceiverFakeIdent));
                onewayR = Ice::uncheckedCast<CallbackReceiverPrx>(onewayR->ice_identity(callbackReceiverFakeIdent));
            }
            else
            {
                twowayR = Ice::uncheckedCast<CallbackReceiverPrx>(twowayR->ice_identity(callbackReceiverIdent));
                onewayR = Ice::uncheckedCast<CallbackReceiverPrx>(onewayR->ice_identity(callbackReceiverIdent));
            }

            cout << "callback receiver identity: " << Ice::identityToString(twowayR->ice_getIdentity())
                 << endl;
        }
        else if(c == 's')
        {
            twoway->shutdown();
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
    while(cin.good() && c != 'x');
}

void
menu()
{
    cout <<
        "usage:\n"
        "t: invoke callback as twoway\n"
        "o: invoke callback as oneway\n"
        "O: invoke callback as batch oneway\n"
        "f: flush all batch requests\n"
        "v: set/reset override context field\n"
        "F: set/reset fake category\n"
        "s: shutdown server\n"
        "x: exit\n"
        "?: help\n";
}
