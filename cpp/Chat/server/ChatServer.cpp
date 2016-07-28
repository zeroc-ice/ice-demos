// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>

#include <ChatSessionManagerI.h>
#include <PollingChatSessionFactoryI.h>

using namespace std;

class ChatServer : public Ice::Service
{
public:

    virtual bool start(int argc, char* argv[], int&);
    virtual bool stop();

private:

    IceUtil::TimerPtr _timer;
    Ice::ObjectAdapterPtr _adapter;
};

bool
ChatServer::start(int, char*[], int& status)
{
    _timer = new IceUtil::Timer();
    // Timeout for reaping polling sessions.
    int timeout = communicator()->getProperties()->getPropertyAsIntWithDefault("ReaperTimeout", 10);
    bool traceEnabled = communicator()->getProperties()->getPropertyAsIntWithDefault("Server.Trace", 0) != 0;
    Ice::LoggerPtr logger = communicator()->getLogger();
    ReaperTaskPtr reaper = new ReaperTask(timeout, traceEnabled, logger);
    _timer->scheduleRepeated(reaper, IceUtil::Time::seconds(timeout));
    try
    {
        _adapter = communicator()->createObjectAdapter("ChatServer");

        ChatRoomPtr chatRoom = new ChatRoom(traceEnabled, logger);
        if(traceEnabled)
        {
            ostringstream os;
            os << "Chat room created ok.";
            logger->trace("info", os.str());
        }
        _adapter->add(new ChatSessionManagerI(chatRoom, traceEnabled, logger),
                     Ice::stringToIdentity("ChatSessionManager"));

        if(traceEnabled)
        {
            ostringstream os;
            os << "Chat session manager created ok.";
            logger->trace("info", os.str());
        }
        _adapter->add(new PollingChatSessionFactoryI(chatRoom, reaper, traceEnabled, logger),
                     Ice::stringToIdentity("PollingChatSessionFactory"));

        if(traceEnabled)
        {
            ostringstream os;
            os << "Polling chat session factory created ok.";
            logger->trace("info", os.str());
        }

        _adapter->activate();
        if(traceEnabled)
        {
            ostringstream os;
            os << "Chat server started ok.";
            logger->trace("info", os.str());
        }
    }
    catch(const Ice::LocalException&)
    {
        status = EXIT_FAILURE;
        _timer->destroy();
        throw;
    }
    status = EXIT_SUCCESS;
    return true;
}

bool
ChatServer::stop()
{
    _timer->destroy();
    return true;
}



int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    ChatServer app;
    return app.main(argc, argv);
}
