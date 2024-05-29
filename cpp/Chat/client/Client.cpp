//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "ChatSession.h"
#include "ChatUtils.h"
#include <Glacier2/Glacier2.h>
#include <Ice/Ice.h>
#include <iostream>

using namespace std;

static const unsigned int maxMessageSize = 1024;

namespace
{
    // mutex to prevent intertwined cout output
    mutex coutMutex;

    void menu()
    {
        const lock_guard<mutex> lock(coutMutex);
        cout << "enter /quit to exit." << endl;
    }
}

class ChatRoomCallbackI final : public Chat::ChatRoomCallback
{
public:
    void init(Ice::StringSeq names, const Ice::Current&) final
    {
        const lock_guard<mutex> lock(coutMutex);
        cout << "Users: ";
        for (auto it = names.begin(); it != names.end();)
        {
            cout << *it;
            it++;
            if (it != names.end())
            {
                cout << ", ";
            }
        }
        cout << endl;
    }

    void join(int64_t, string name, const Ice::Current&) final
    {
        const lock_guard<mutex> lock(coutMutex);
        cout << ">>>> " << name << " joined." << endl;
    }

    void leave(int64_t, string name, const Ice::Current&) final
    {
        const lock_guard<mutex> lock(coutMutex);
        cout << "<<<< " << name << " left." << endl;
    }

    void send(int64_t, string name, string message, const Ice::Current&) final
    {
        const lock_guard<mutex> lock(coutMutex);
        cout << name << " > " << ChatUtils::unstripHtml(message) << endl;
    }
};

Chat::ChatSessionPrx
createSession(Glacier2::RouterPrx router)
{
    while (true)
    {
        cout << "This demo accepts any user ID and password.\n";

        string id;
        cout << "user id: " << flush;
        getline(cin, id);
        id = ChatUtils::trim(id);

        string pw;
        cout << "password: " << flush;
        getline(cin, pw);
        pw = ChatUtils::trim(pw);

        try
        {
            optional<Chat::ChatSessionPrx> session{router->createSession(id, pw)};
            if (!session)
            {
                throw runtime_error("Glaicer2::createSession return null. Is the SessionManager configured?");
            }
            router->ice_getCachedConnection()->setCloseCallback(
                [](Ice::ConnectionPtr)
                {
                    const lock_guard<mutex> lock(coutMutex);
                    cout << "The Glacier2 session has been destroyed." << endl;
                });
            return *session;
        }
        catch (const Glacier2::CannotCreateSessionException& ex)
        {
            cout << "Login failed:\n" << ex.reason << endl;
        }
        catch (const Glacier2::PermissionDeniedException& ex)
        {
            cout << "Login failed:\n" << ex.reason << endl;
        }
        catch (const Ice::LocalException& ex)
        {
            cerr << "Communication with the server failed:\n" << ex << endl;
        }
    }
}

void
run(shared_ptr<Ice::Communicator> communicator)
{
    optional<Glacier2::RouterPrx> router{communicator->getDefaultRouter()};
    if (!router)
    {
        throw runtime_error("Ice.Default.Router property not set.");
    }

    // Create a session with the Glacier2 router
    Chat::ChatSessionPrx session = createSession(*router);

    auto adapter = communicator->createObjectAdapterWithRouter("", *router);
    adapter->activate();

    // Create a callback object and add it to the adapter using the client's category
    // provided by the router
    Ice::Identity id{Ice::generateUUID(), router->getCategoryForClient()};

    Chat::ChatRoomCallbackPrx callbackPrx = adapter->add(make_shared<ChatRoomCallbackI>(), id);

    // Set the callback proxy on the session
    session->setCallback(callbackPrx);

    menu();

    do
    {
        string s;
        cout << "";
        getline(cin, s);
        s = ChatUtils::trim(s);

        if (!s.empty())
        {
            if (s[0] == '/')
            {
                if (s == "/quit")
                {
                    break;
                }
                menu();
            }
            else
            {
                if (s.size() > maxMessageSize)
                {
                    const lock_guard<mutex> lock(coutMutex);
                    cout << "Message length exceeded, maximum length is " << maxMessageSize << " characters.";
                }
                else
                {
                    session->send(s);
                }
            }
        }
    } while (cin.good());
}

int
main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties(argc, argv);

        if (argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }

        //
        // Set Ice.Default.Router if not set
        //
        if (initData.properties->getProperty("Ice.Default.Router").empty())
        {
            initData.properties->setProperty("IceSSL.UsePlatformCAs", "1");
            // initData.properties->setProperty("IceSSL.CheckCertName", "2");
            initData.properties->setProperty(
                "Ice.Default.Router",
                "Glacier2/router:wss -p 443 -h zeroc.com -r /demo-proxy/chat/glacier2");
        }
        const Ice::CommunicatorHolder ich(initData);

        run(ich.communicator());
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
