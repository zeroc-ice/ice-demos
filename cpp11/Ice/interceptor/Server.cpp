//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <unordered_set>
#include <Ice/Ice.h>
#include <AuthenticatorI.h>
#include <InterceptorI.h>
#include <ThermostatI.h>

using namespace std;

int main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        //
        // CtrlCHandler must be created before the communicator
        // or any other threads are started.
        //
        Ice::CtrlCHandler ctrlCHandler;

        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config.server");
        auto communicator = ich.communicator();

        ctrlCHandler.setCallback(
            [communicator](int)
            {
                communicator->shutdown();
            });

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
            //
            // Create an object adapter for the authenticator.
            //
            auto authenticatorAdapter = communicator->createObjectAdapter("Authenticator");
            auto authenticator = make_shared<AuthenticatorI>();
            authenticatorAdapter->add(authenticator, Ice::stringToIdentity("authenticator"));
            authenticatorAdapter->activate();

            //
            // List of all the operations to require authorization for.
            //
            unordered_set<string> securedOperations({ "setTemp" });
            //
            // Create an object adapter for the thermostat.
            //
            auto thermostatAdapter = communicator->createObjectAdapter("Thermostat");
            auto thermostat = make_shared<ThermostatI>();
            thermostatAdapter->add(make_shared<InterceptorI>(thermostat, authenticator, securedOperations), Ice::stringToIdentity("thermostat"));
            thermostatAdapter->activate();

            communicator->waitForShutdown();
        }
    }
    catch(const exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
