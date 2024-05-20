//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <ChatSessionManagerI.h>
#include <Ice/Ice.h>
#include <PollingChatSessionFactoryI.h>

using namespace std;

class ChatServer : public Ice::Service
{
public:
    virtual bool start(int argc, char* argv[], int&) override;
    virtual bool stop() override;

private:
    shared_ptr<Ice::ObjectAdapter> _adapter;
};

bool
ChatServer::start(int, char*[], int& status)
{
    const int timeout = communicator()->getProperties()->getPropertyAsIntWithDefault("PollingChatSessionTimeout", 10);
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
            make_shared<PollingChatSessionFactoryI>(chatRoom, timeout, traceEnabled, logger),
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
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
    Ice::registerIceWS();
#endif
    ChatServer app;
    return app.main(argc, argv);
}
