// Copyright (c) ZeroC, Inc.

#include "ChatSessionI.h"
#include <Glacier2/Glacier2.h>
#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Demo;

class DummyPermissionsVerifierI final : public Glacier2::PermissionsVerifier
{
public:
    bool checkPermissions(string userId, string password, string&, const Ice::Current&) const final
    {
        cout << "verified user `" << userId << "' with password `" << password << "'" << endl;
        return true;
    }
};

int
main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        //
        // CtrlCHandler must be created before the communicator or any other threads are started
        //
        Ice::CtrlCHandler ctrlCHandler;

        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        const Ice::CommunicatorHolder ich(argc, argv, "config.server");
        const auto& communicator = ich.communicator();

        ctrlCHandler.setCallback([communicator](int) { communicator->shutdown(); });

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if (argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            auto adapter = ich->createObjectAdapter("ChatServer");

            auto dpv = make_shared<DummyPermissionsVerifierI>();
            adapter->add(dpv, Ice::stringToIdentity("ChatSessionVerifier"));
            auto csm = make_shared<ChatSessionManagerI>();
            adapter->add(csm, Ice::stringToIdentity("ChatSessionManager"));
            adapter->activate();

            ich->waitForShutdown();
            csm->destroy();
        }
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
