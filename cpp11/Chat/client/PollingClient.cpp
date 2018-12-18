// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>

#include <PollingChat.h>
#include <ChatUtils.h>
#include <chrono>

using namespace std;
static const unsigned int maxMessageSize = 1024;

namespace
{

// mutex to prevent intertwined cout output
mutex coutMutex;

}

class GetUpdatesTask
{
public:

    GetUpdatesTask(const shared_ptr<PollingChat::PollingChatSessionPrx>& session) :
        _session(session)
    {
    }

    ~GetUpdatesTask()
    {
        {
            lock_guard<mutex> lock(_mutex);
            _done = true;
        }
        _cond.notify_all();

        assert(_asyncResult.valid());
        try
        {
            _asyncResult.get();
        }
        catch(const std::exception& ex)
        {
            cerr << "Update task failed with: " << ex.what() << endl;
        }
    }

    void start(int period)
    {
        assert(!_asyncResult.valid()); // run must be called just once
        _asyncResult = async(launch::async, [this, period] { run(period); });
    }

    void run(int period)
    {
        unique_lock<mutex> lock(_mutex);
        while(!_done)
        {
            lock.unlock();
            try
            {
                auto updates = _session->getUpdates();
                for(const auto& u : updates)
                {
                    auto joinedEvt = dynamic_pointer_cast<PollingChat::UserJoinedEvent>(u);
                    if(joinedEvt)
                    {
                        lock_guard<mutex> lkg(coutMutex);
                        cout << ">>>> " << joinedEvt->name << " joined." << endl;
                    }
                    else
                    {
                        auto leftEvt = dynamic_pointer_cast<PollingChat::UserLeftEvent>(u);
                        if(leftEvt)
                        {
                            lock_guard<mutex> lkg(coutMutex);
                            cout << ">>>> " << leftEvt->name << " left." << endl;
                        }
                        else
                        {
                            auto messageEvt = dynamic_pointer_cast<PollingChat::MessageEvent>(u);
                            if(messageEvt)
                            {
                                lock_guard<mutex> lkg(coutMutex);
                                cout << messageEvt->name << " > " << ChatUtils::unstripHtml(messageEvt->message) << endl;
                            }
                        }
                    }
                }
            }
            catch(const Ice::LocalException& ex)
            {
                {
                    lock_guard<mutex> lkg(_mutex);
                    _done = true;
                }
                if(!dynamic_cast<const Ice::ObjectNotExistException*>(&ex))
                {
                    cerr << "session lost:" << ex << endl;
                }
            }

            lock.lock();
            if(!_done)
            {
                _cond.wait_for(lock, chrono::seconds(period));
            }
        }
    }

    bool isDone() const
    {
        lock_guard<mutex> lock(const_cast<mutex&>(_mutex));
        return _done;
    }

private:

    const shared_ptr<PollingChat::PollingChatSessionPrx> _session;
    std::future<void> _asyncResult; // only used by the main thread

    bool _done = false;
    mutex _mutex;
    condition_variable _cond;
};

int run(const shared_ptr<Ice::Communicator>&);

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
    Ice::registerIceWS();
#endif

    int status = 0;

    try
    {
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

        Ice::CommunicatorHolder ich(argc, argv, initData);

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

int
run(const shared_ptr<Ice::Communicator>& communicator)
{
    auto sessionFactory =
        Ice::checkedCast<PollingChat::PollingChatSessionFactoryPrx>(
            communicator->propertyToProxy("PollingChatSessionFactory"));

    if(!sessionFactory)
    {
        cerr << "PollingChatSessionFactory proxy is not properly configured" << endl;
        return 1;
    }

    shared_ptr<PollingChat::PollingChatSessionPrx> session;
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
    if(communicator->getProperties()->getPropertyAsInt("OverrideSessionEndpoints") != 0)
    {
        session = session->ice_endpoints(sessionFactory->ice_getEndpoints());
    }

    GetUpdatesTask getUpdatesTask(session);
    getUpdatesTask.start(1);

    menu();

    auto users = session->getInitialUsers();
    {
        lock_guard<mutex> lock(coutMutex);
        cout << "Users: ";
        for(auto it = users.begin(); it != users.end();)
        {
            cout << *it;
            it++;
            if(it != users.end())
            {
                cout << ", ";
            }
        }
        cout << endl;
    }

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
                        lock_guard<mutex> lock(coutMutex);
                        cout << "Message length exceeded, maximum length is " << maxMessageSize << " characters.";
                    }
                    else
                    {
                        session->send(s);
                    }
                }
            }
        }
        while(cin.good() && !getUpdatesTask.isDone());
    }
    catch(const Ice::LocalException& ex)
    {
        cerr << "Communication with the server failed:\n" << ex << endl;
        try
        {
            session->destroy();
        }
        catch(const Ice::LocalException&)
        {
        }
        return 1;
    }

    try
    {
        session->destroy();
    }
    catch(const Ice::LocalException&)
    {
    }
    return 0;
}

void
menu()
{
    lock_guard<mutex> lock(coutMutex);
    cout << "enter /quit to exit." << endl;
}
