// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Glacier2/Glacier2.h>
#include <ChatSessionI.h>

using namespace std;
using namespace Demo;

//
// Global variable for shutdownCommunicator
//
Ice::CommunicatorPtr communicator;

//
// Callback for CtrlCHandler
//
void
shutdownCommunicator(int)
{
    communicator->shutdown();
}

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

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    int status = 0;

    try
    {
        //
        // CtrlCHandler must be created before the communicator or any other threads are started
        //
        Ice::CtrlCHandler ctrlCHandler;

        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and it's dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config.server");
        communicator = ich.communicator();

        //
        // Shutdown communicator on Ctrl-C
        //
        ctrlCHandler.setCallback(&shutdownCommunicator);

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if(argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("ChatServer");

            Glacier2::PermissionsVerifierPtr dpv = new DummyPermissionsVerifierI;
            adapter->add(dpv, Ice::stringToIdentity("ChatSessionVerifier"));
            ChatSessionManagerIPtr csm = new ChatSessionManagerI;
            adapter->add(csm, Ice::stringToIdentity("ChatSessionManager"));
            adapter->activate();

            communicator->waitForShutdown();
            csm->destroy();
        }
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
