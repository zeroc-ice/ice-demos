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
            // We use separate object adapters for the authenticator and the thermostat in
            // this demo, so each object has its own endpoints. This way accessibility can
            // be fined tuned on each object separately, through firewall rules for example.
            // It's implemented this way to illustrate best practices, even if the demo
            // doesn't make use of it for simplicity's sake.

            //
            // Create an object adapter for the authenticator.
            //
            auto authenticatorAdapter = communicator->createObjectAdapter("Authenticator");
            auto authenticator = make_shared<AuthenticatorI>();
            authenticatorAdapter->add(authenticator, Ice::stringToIdentity("authenticator"));
            authenticatorAdapter->activate();

            //
            // Set of all the operations to require authorization for.
            //
            unordered_set<string> securedOperations({ "setTemp", "shutdown" });
            //
            // Create an object adapter for the thermostat.
            //
            auto thermostatAdapter = communicator->createObjectAdapter("Thermostat");
            auto thermostat = make_shared<ThermostatI>();
            auto interceptor = make_shared<InterceptorI>(move(thermostat), authenticator, move(securedOperations));
            thermostatAdapter->add(interceptor, Ice::stringToIdentity("thermostat"));
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
