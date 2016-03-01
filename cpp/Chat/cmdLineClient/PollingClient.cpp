// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>

#include <PollingChat.h>
#include <ChatUtils.h>

using namespace std;
static const unsigned int maxMessageSize = 1024;

class GetUpdatesTask : public IceUtil::TimerTask
{
public:

    GetUpdatesTask(const PollingChat::PollingChatSessionPrx& session) :
        _session(session),
        _destroyed(false)
    {
    }

    virtual void
    runTimerTask()
    {
        IceUtil::Mutex::Lock sync(_mutex);
        bool destroyed = _destroyed;
        sync.release();
        if(!destroyed)
        {
            try
            {
                PollingChat::ChatRoomEventSeq updates = _session->getUpdates();
                for(PollingChat::ChatRoomEventSeq::const_iterator it = updates.begin(); it != updates.end(); ++it)
                {
                    for(;;)
                    {
                        PollingChat::UserJoinedEventPtr joinedEvt = PollingChat::UserJoinedEventPtr::dynamicCast(*it);
                        if(joinedEvt)
                        {
                            cout << ">>>> " << joinedEvt->name << " joined." << endl;
                            break;
                        }

                        PollingChat::UserLeftEventPtr leftEvt = PollingChat::UserLeftEventPtr::dynamicCast(*it);
                        if(leftEvt)
                        {
                            cout << ">>>> " << leftEvt->name << " left." << endl;
                            break;
                        }

                        PollingChat::MessageEventPtr messageEvt = PollingChat::MessageEventPtr::dynamicCast(*it);
                        if(messageEvt)
                        {
                            cout << messageEvt->name << " > " << ChatUtils::unstripHtml(messageEvt->message) << endl;
                            break;
                        }

                        break;
                    }
                }
            }
            catch(const Ice::LocalException& ex)
            {
                sync.acquire();
                _destroyed = true;
                if(!dynamic_cast<const Ice::ObjectNotExistException*>(&ex))
                {
                    cerr << "session lost:" << ex << endl;
                }
            }
        }
    }

    bool isDestroyed()
    {
        IceUtil::Mutex::Lock sync(_mutex);
        return _destroyed;
    }

private:

    const PollingChat::PollingChatSessionPrx _session;
    bool _destroyed;
    IceUtil::Mutex _mutex;
};
typedef IceUtil::Handle<GetUpdatesTask> GetUpdatesTaskPtr;

class ChatClient : public Ice::Application
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

    virtual int
    run(int argc, char*[])
    {
        if(argc > 1)
        {
            cerr << appName() << ": too many arguments" << endl;
            return EXIT_FAILURE;
        }

        PollingChat::PollingChatSessionFactoryPrx sessionFactory =
            PollingChat::PollingChatSessionFactoryPrx::checkedCast(
                communicator()->propertyToProxy("PollingChatSessionFactory"));

        if(sessionFactory == 0)
        {
            cerr << "PollingChatSessionFactory proxy is not properly configured" << endl;
            return EXIT_FAILURE;
        }

        PollingChat::PollingChatSessionPrx session;
        while(!session)
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
                session = sessionFactory->create(id, pw);
            }
            catch(const PollingChat::CannotCreateSessionException& ex)
            {
                cout << "Login failed:\n" << ex.reason << endl;
            }
            catch(const Ice::LocalException& ex)
            {
                cout << "Communication with the server failed:\n" << ex << endl;
            }

            if(session)
            {
                break;
            }
        }

        //
        // Override session proxy's endpoints if necessary
        //
        if(communicator()->getProperties()->getPropertyAsInt("OverrideSessionEndpoints") != 0)
        {
            session = session->ice_endpoints(sessionFactory->ice_getEndpoints());
        }

        IceUtil::TimerPtr timer = new IceUtil::Timer();
        GetUpdatesTaskPtr getUpdatesTask = new GetUpdatesTask(session);
        timer->scheduleRepeated(getUpdatesTask, IceUtil::Time::seconds(1));

        menu();

        Ice::StringSeq users = session->getInitialUsers();
        cout << "Users: ";
        for(Ice::StringSeq::const_iterator it = users.begin(); it != users.end();)
        {
            cout << *it;
            it++;
            if(it != users.end())
            {
                cout << ", ";
            }
        }
        cout << endl;

        try
        {
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
                            cout << "Message length exceeded, maximum length is " << maxMessageSize << " characters.";
                        }
                        else
                        {
                            session->send(s);
                        }
                    }
                }
            }
            while(cin.good() && !getUpdatesTask->isDestroyed());
        }
        catch(const Ice::LocalException& ex)
        {
            cerr << "Communication with the server failed:\n" << ex << endl;
            if(session)
            {
                try
                {
                    session->destroy();
                }
                catch(const Ice::LocalException&)
                {
                }
            }
            if(timer)
            {
                timer->destroy();
            }
            return EXIT_FAILURE;
        }

        if(session)
        {
            try
            {
                session->destroy();
            }
            catch(const Ice::LocalException&)
            {
            }
        }
        if(timer)
        {
            timer->destroy();
        }
        return EXIT_SUCCESS;
    }

private:

    void
    menu()
    {
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
    // Set PollingChatSessionFactory if not set
    //
    if(initData.properties->getProperty("PollingChatSessionFactory").empty())
    {
        initData.properties->setProperty("Ice.Plugin.IceSSL","IceSSL:createIceSSL");
        initData.properties->setProperty("IceSSL.UsePlatformCAs", "1");
        initData.properties->setProperty("IceSSL.CheckCertName", "1");
        initData.properties->setProperty("PollingChatSessionFactory",
                                         "PollingChatSessionFactory:wss -h zeroc.com -p 443 -r /demo-proxy/chat/poll");
        initData.properties->setProperty("OverrideSessionEndpoints", "1");
    }

    ChatClient app;
    return app.main(argc, argv, initData);

}
