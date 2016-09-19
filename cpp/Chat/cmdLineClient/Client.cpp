// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include <Glacier2/Glacier2.h>

#include <ChatSession.h>
#include <ChatUtils.h>

using namespace std;

static const unsigned int maxMessageSize = 1024;

namespace
{

// mutex to prevent intertwined cout output
IceUtil::Mutex coutMutex;

}

class ChatRoomCallbackI : public Chat::ChatRoomCallback
{
public:

    virtual void
    init(const Ice::StringSeq& names, const Ice::Current&)
    {
        IceUtil::Mutex::Lock lock(coutMutex);
        cout << "Users: ";
        for(Ice::StringSeq::const_iterator it = names.begin(); it != names.end();)
        {
            cout << *it;
            it++;
            if(it != names.end())
            {
                cout << ", ";
            }
        }
        cout << endl;
    }

    virtual void
    join(Ice::Long, const string& name, const Ice::Current&)
    {
        IceUtil::Mutex::Lock lock(coutMutex);
        cout << ">>>> " << name << " joined." << endl;
    }

    virtual void
    leave(Ice::Long, const string& name, const Ice::Current&)
    {
        IceUtil::Mutex::Lock lock(coutMutex);
        cout << "<<<< " << name << " left." << endl;
    }

    virtual void
    send(Ice::Long, const string& name, const string& message, const Ice::Current&)
    {
        IceUtil::Mutex::Lock lock(coutMutex);
        cout << name << " > " << ChatUtils::unstripHtml(message) << endl;
    }
};

class ChatClient : public Glacier2::Application
{
public:

    ChatClient() :
        //
        // Since this is an interactive demo we don't want any signal
        // handling.
        //
        Application(Ice::NoSignalHandling)
    {
    }

    virtual Glacier2::SessionPrx
    createSession()
    {
        Glacier2::SessionPrx sessionPrx;
        while(!sessionPrx)
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
                sessionPrx = router()->createSession(id, pw);
                break;
            }
            catch(const Glacier2::CannotCreateSessionException& ex)
            {
                cout << "Login failed:\n" << ex.reason << endl;
            }
            catch(const Glacier2::PermissionDeniedException& ex)
            {
                cout << "Login failed:\n" << ex.reason << endl;
            }
            catch(const Ice::LocalException& ex)
            {
                cerr << "Communication with the server failed:\n" << ex << endl;
            }
        }
        return sessionPrx;
    }

    virtual void
    sessionDestroyed()
    {
        cerr << "Session destroyed " << endl;
    }

    virtual int
    runWithSession(int argc, char*[])
    {
        if(argc > 1)
        {
            throw string("usage: ") + appName();
        }

        Chat::ChatSessionPrx sessionPrx = Chat::ChatSessionPrx::uncheckedCast(this->session());
        sessionPrx->setCallback(Chat::ChatRoomCallbackPrx::uncheckedCast(addWithUUID(new ChatRoomCallbackI())));

        menu();

        do
        {
            string s;
            cout << "";
            getline(cin, s);
            s = ChatUtils::trim(s);

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
                    if(s.size() > maxMessageSize)
                    {
                        IceUtil::Mutex::Lock lock(coutMutex);
                        cout << "Message length exceeded, maximum length is " << maxMessageSize << " characters.";
                    }
                    else
                    {
                        sessionPrx->send(s);
                    }
                }
            }
        }
        while(cin.good());
        return EXIT_SUCCESS;
    }

private:

    void
    menu()
    {
        IceUtil::Mutex::Lock lock(coutMutex);
        cout << "enter /quit to exit." << endl;
    }
};


int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    Ice::InitializationData initData;
    initData.properties = Ice::createProperties(argc, argv);

    //
    // Set Ice.Default.Router if not set
    //
    if(initData.properties->getProperty("Ice.Default.Router").empty())
    {
        initData.properties->setProperty("Ice.Plugin.IceSSL","IceSSL:createIceSSL");
        initData.properties->setProperty("IceSSL.UsePlatformCAs", "1");
        initData.properties->setProperty("IceSSL.CheckCertName", "1");
        initData.properties->setProperty("Ice.Default.Router",
                                         "Glacier2/router:wss -p 443 -h zeroc.com -r /demo-proxy/chat/glacier2");
    }

    ChatClient app;
    return app.main(argc, argv, initData);
}
