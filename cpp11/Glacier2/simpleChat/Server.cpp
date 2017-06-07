// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Glacier2/Glacier2.h>
#include <ChatSessionI.h>

using namespace std;
using namespace Demo;

class DummyPermissionsVerifierI : public Glacier2::PermissionsVerifier
{
public:

    virtual bool
    checkPermissions(string userId, string password, string&, const Ice::Current&) const override
    {
        cout << "verified user `" << userId << "' with password `" << password << "'" << endl;
        return true;
    }
};

class ChatServer : public Ice::Application
{
public:

    virtual int
    run(int argc, char*[]) override
    {
        if(argc > 1)
        {
            cerr << appName() << ": too many arguments" << endl;
            return EXIT_FAILURE;
        }

        auto adapter = communicator()->createObjectAdapter("ChatServer");

        auto dpv = make_shared<DummyPermissionsVerifierI>();
        adapter->add(dpv, Ice::stringToIdentity("ChatSessionVerifier"));
        auto csm = make_shared<ChatSessionManagerI>();
        adapter->add(csm, Ice::stringToIdentity("ChatSessionManager"));
        adapter->activate();
        communicator()->waitForShutdown();
        csm->destroy();

        return EXIT_SUCCESS;
    }
};

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    ChatServer app;
    return app.main(argc, argv, "config.server");
}
