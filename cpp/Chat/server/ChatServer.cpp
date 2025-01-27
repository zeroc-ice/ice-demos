// Copyright (c) ZeroC, Inc.

#include "ChatSessionManagerI.h"
#include "PollingChatSessionFactoryI.h"
#include <Ice/Ice.h>
#include <Ice/Service.h>
using namespace std;

class ChatServer final : public Ice::Service
{
public:
    bool start(int argc, char* argv[], int&) final;
    bool stop() final;

private:
    shared_ptr<Ice::ObjectAdapter> _adapter;
};

bool
ChatServer::start(int, char*[], int& status)
{
    const bool traceEnabled = communicator()->getProperties()->getPropertyAsIntWithDefault("Server.Trace", 0) != 0;
    auto logger = communicator()->getLogger();

    try
    {
        _adapter = communicator()->createObjectAdapter("ChatServer");

        auto chatRoom = make_shared<ChatRoom>(traceEnabled, logger);
        if (traceEnabled)
        {
            Ice::Trace out(logger, "info");
            out << "Chat room created ok.";
        }
        _adapter->add(
            make_shared<ChatSessionManagerI>(chatRoom, traceEnabled, logger),
            Ice::stringToIdentity("ChatSessionManager"));

        if (traceEnabled)
        {
            Ice::Trace out(logger, "info");
            out << "Chat session manager created ok.";
        }
        _adapter->add(
            make_shared<PollingChatSessionFactoryI>(chatRoom, traceEnabled, logger),
            Ice::stringToIdentity("PollingChatSessionFactory"));

        if (traceEnabled)
        {
            Ice::Trace out(logger, "info");
            out << "Polling chat session factory created ok.";
        }

        _adapter->activate();
        if (traceEnabled)
        {
            Ice::Trace out(logger, "info");
            out << "Chat server started ok.";
        }
    }
    catch (const Ice::LocalException&)
    {
        status = 1;
        throw;
    }
    status = 0;
    return true;
}

bool
ChatServer::stop()
{
    return true;
}

int
main(int argc, char* argv[])
{
    ChatServer app;
    return app.main(argc, argv);
}
