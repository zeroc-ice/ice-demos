// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h> // For generateUUID.
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

class ChatSessionManagerI : public Glacier2::SessionManager
{
public:

    virtual Glacier2::SessionPrx
    create(const string& userId, const Glacier2::SessionControlPrx&, const Ice::Current& current)
    {
        Ice::Identity ident = { Ice::generateUUID(), "session" };
        return Glacier2::SessionPrx::uncheckedCast(current.adapter->add(new ChatSessionI(userId), ident));
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
        Glacier2::SessionManagerPtr csm = new ChatSessionManagerI;
        adapter->add(csm, Ice::stringToIdentity("ChatSessionManager"));
        adapter->activate();
        communicator()->waitForShutdown();

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
