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
    checkPermissions(const string& userId, const string& password, string&, const Ice::Current&) const
    {
        cout << "verified user `" << userId << "' with password `" << password << "'" << endl;
        return true;
    }
};

class ChatServer : public Ice::Application
{
public:

    virtual int
    run(int argc, char*[])
    {
        if(argc > 1)
        {
            cerr << appName() << ": too many arguments" << endl;
            return EXIT_FAILURE;
        }

        Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("ChatServer");

        Glacier2::PermissionsVerifierPtr dpv = new DummyPermissionsVerifierI;
        adapter->add(dpv, Ice::stringToIdentity("ChatSessionVerifier"));
        ChatSessionManagerIPtr csm = new ChatSessionManagerI;
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
