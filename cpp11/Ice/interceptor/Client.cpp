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
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config.client");

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
            status = run(ich.communicator());
        }
    }
    catch(const exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}

void menu();

int
run(const shared_ptr<Ice::Communicator>& communicator)
{
    auto thermostat =
        Ice::checkedCast<Demo::ThermostatPrx>(communicator->propertyToProxy("Thermostat.Proxy"));
    if(!thermostat)
    {
        cerr << "invalid thermostat proxy" << endl;
        return 1;
    }
    auto authenticator =
        Ice::checkedCast<Demo::AuthenticatorPrx>(communicator->propertyToProxy("Authenticator.Proxy"));
    if(!authenticator)
    {
        cerr << "invalid authenticator proxy" << endl;
        return 1;
    }

    menu();

    string line;
    try
    {
        do
        {
            cout << "\n==> " << flush;
            getline(cin, line);

            if(line == "1")
            {
                cout << "Current temperature is " << thermostat->getTemp() << endl;
            }
            else if(line == "2")
            {
                try
                {
                    cout << "Enter desired temperature:" << endl;
                    string temp;
                    getline(cin, temp);
                    thermostat->setTemp(stof(temp));
                    cout << "New temperature is " << thermostat->getTemp() << endl;
                }
                catch(const invalid_argument&)
                {
                    cout << "Provided temperature is not a parsable float." << endl;
                }
                catch(const Demo::TokenExpiredException&)
                {
                    cout << "Failed to set temperature. Token expired!" << endl;
                }
                catch(const Demo::AuthorizationException&)
                {
                    cout << "Failed to set temperature. Access denied!" << endl;
                }
            }
            else if(line == "3")
            {
                //
                // Request an access token from the server's authentication object.
                //
                string token = authenticator->getToken();
                cout << "Successfully retrieved access token: \"" << token << "\"" << endl;
                //
                // Add the access token to the communicator's context, so it will be
                // sent along with every request made through it.
                //
                communicator->getImplicitContext()->put("accessToken", token);
            }
            else if(line == "4")
            {
                auto context = communicator->getImplicitContext();
                if(context->containsKey("accessToken"))
                {
                    context->remove("accessToken");
                }
                else
                {
                    cout << "There is no access token to release." << endl;
                }
            }
            else if(line == "s")
            {
                try
                {
                    thermostat->shutdown();
                }
                catch(const Demo::TokenExpiredException&)
                {
                    cout << "Failed to shutdown thermostat. Token expired!" << endl;
                }
                catch(const Demo::AuthorizationException&)
                {
                    cout << "Failed to shutdown thermostat. Access denied!" << endl;
                }
            }
            else if(line == "x")
            {
                // Nothing to do, the loop will exit on its own.
            }
            else if(line == "?")
            {
                menu();
            }
            else
            {
                cout << "Unknown command `" << line << "'" << endl;
                menu();
            }
        }
        while(cin.good() && (line != "x"));
    }
    catch(const Ice::Exception& ex)
    {
        cerr << ex << endl;
    }

    return 0;
}

void
menu()
{
    cout <<
        "usage:\n"
        "1: gets the current temperature\n"
        "2: sets the temperature\n"
        "3: gets a new token from the server\n"
        "4: releases the currently held token\n"
        "s: shutdown server\n"
        "x: exit\n"
        "?: help\n";
}
