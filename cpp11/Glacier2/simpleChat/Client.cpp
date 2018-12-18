// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Glacier2/Glacier2.h>
#include <Chat.h>

using namespace std;
using namespace Demo;

// mutex to prevent intertwined cout output
mutex coutMutex;

class ChatCallbackI : public ChatCallback
{
public:

    virtual void
    message(string data, const Ice::Current&) override
    {
        lock_guard<mutex> lock(coutMutex);
        cout << data << endl;
    }
};

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
string trim(const string&);

void
run(const shared_ptr<Ice::Communicator>& communicator)
{
    shared_ptr<Glacier2::RouterPrx> router = Ice::checkedCast<Glacier2::RouterPrx>(communicator->getDefaultRouter());
    shared_ptr<ChatSessionPrx> session;
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
            session = Ice::uncheckedCast<ChatSessionPrx>(router->createSession(id, pw));
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
                                     lock_guard<mutex> lock(coutMutex);
                                     cout << "The Glacier2 session has been destroyed." << endl;
                                 });

    Ice::Identity callbackReceiverIdent;
    callbackReceiverIdent.name = "cabackReceiver";
    callbackReceiverIdent.category = router->getCategoryForClient();

    auto adapter = communicator->createObjectAdapterWithRouter("", router);
    adapter->activate();

    auto callback = Ice::uncheckedCast<ChatCallbackPrx>(adapter->add(make_shared<ChatCallbackI>(),
                                                                     callbackReceiverIdent));

    session->setCallback(callback);
    menu();

    do
    {
        string s;
        {
            lock_guard<mutex> lock(coutMutex);
            cout << "==> ";
        }
        getline(cin, s);
        s = trim(s);
        if(!s.empty())
        {
            if(s[0] == '/')
            {
                if(s == "/quit")
                {
                    break;
                }
                menu();
            }
            else
            {
                session->say(s);
            }
        }
    }
    while(cin.good());
}

void
menu()
{
    lock_guard<mutex> lock(coutMutex);
    cout << "enter /quit to exit." << endl;
}

string
trim(const string& s)
{
    static const string delims = "\t\r\n ";
    auto last = s.find_last_not_of(delims);
    if(last != string::npos)
    {
        return s.substr(s.find_first_not_of(delims), last+1);
    }
    return s;
}
