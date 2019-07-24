//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <Interceptor.h>

using namespace std;

int run(const shared_ptr<Ice::Communicator>&);

int
main(int argc, char* argv[])
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
        Ice::CommunicatorHolder ich(argc, argv, "config.client");
        auto communicator = ich.communicator();

        ctrlCHandler.setCallback(
            [communicator](int)
            {
                communicator->destroy();
            });

        //
        // The communicator intialization removes all Ice-related arguments from argc/argv.
        //
        if(argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            status = run(communicator);
        }
    }
    catch(const exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}

int
run(const shared_ptr<Ice::Communicator>& communicator)
{
    auto thermostat = Ice::checkedCast<Demo::ThermostatPrx>(communicator->propertyToProxy("Thermostat.Proxy"));
    if(!thermostat)
    {
        cerr << "invalid thermostat proxy" << endl;
        return 1;
    }

    //
    // Read in values from the command line for the username, password, and
    // the temperature to try and set the thermostat to.
    //
    string username;
    string password;
    string temp;
    float desiredTemp;
    try
    {
        cout << "Username:" << endl;
        getline(cin, username);
        cout << "Password:" << endl;
        getline(cin, password);
        cout << "Enter desired temperature:" << endl;
        getline(cin, temp);
        desiredTemp = stof(temp);
    }
    catch(const invalid_argument&)
    {
        cerr << "Specified temperature is not a float value." << endl;
        return 1;
    }

    //
    // Tries calling 'setTemp' without authorization and receives an exception.
    //
    cout << "Attempting to set temperature without access token..." << endl;
    try
    {
        cout << "Current temperature is " << thermostat->getTemp() << endl;
        thermostat->setTemp(desiredTemp);
        cout << "New temperature is " << thermostat->getTemp() << endl;
    }
    catch(const Demo::AuthorizationException&)
    {
        cout << "Failed to set temperature. Access denied!" << endl;
    }
    cout << endl;

    //
    // Request an access token from the server's authentication object.
    //
    auto authenticator = Ice::checkedCast<Demo::AuthenticatorPrx>(communicator->propertyToProxy("Authenticator.Proxy"));
    if(!authenticator)
    {
        cerr << "invalid authenticator proxy" << endl;
        return 1;
    }
    Demo::Token token = authenticator->getToken(username, password);
    cout << "Successfully retrieved access token: \"" + token.value + "\"" << endl;

    //
    // Add the access token to the communicator's context, so it will be
    // sent along with every request made through it.
    //
    auto context = communicator->getImplicitContext();
    context->put("accessToken", token.value);

    //
    // Tries calling 'setTemp' again, this time with the access token.
    //
    cout << "Attempting to set temperature with access token..." << endl;
    try
    {
        cout << "Current temperature is " << thermostat->getTemp() << endl;
        thermostat->setTemp(desiredTemp);
        cout << "New temperature is " << thermostat->getTemp() << endl;
    }
    catch(const Demo::AuthorizationException&)
    {
        //
        // No longer encountered since the client has authorization to use 'setTemp' now.
        //
        cout << "Failed to set temperature. Access denied!" << endl;
    }
    cout << endl;

    return 0;
}
