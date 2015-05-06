// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Chat Demo is licensed to you under the terms described
// in the CHAT_DEMO_LICENSE file included in this distribution.
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
    bool trace = communicator()->getProperties()->getPropertyAsIntWithDefault("Server.Trace", 0) != 0;
    Ice::LoggerPtr logger = communicator()->getLogger();
    ReaperTaskPtr reaper = new ReaperTask(timeout, trace, logger);
    _timer->scheduleRepeated(reaper, IceUtil::Time::seconds(timeout));
    try
    {
        _adapter = communicator()->createObjectAdapter("ChatServer");

        ChatRoomPtr chatRoom = new ChatRoom(trace, logger);
        if(trace)
        {
            ostringstream os;
            os << "Chat room created ok.";
            logger->trace("info", os.str());
        }
        _adapter->add(new ChatSessionManagerI(chatRoom, trace, logger),
                     communicator()->stringToIdentity("ChatSessionManager"));

        if(trace)
        {
            ostringstream os;
            os << "Chat session manager created ok.";
            logger->trace("info", os.str());
        }
        _adapter->add(new PollingChatSessionFactoryI(chatRoom, reaper, trace, logger),
                     communicator()->stringToIdentity("PollingChatSessionFactory"));

        if(trace)
        {
            ostringstream os;
            os << "Polling chat session factory created ok.";
            logger->trace("info", os.str());
        }

        _adapter->activate();
        if(trace)
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
    ChatServer app;
    return app.main(argc, argv);
}
