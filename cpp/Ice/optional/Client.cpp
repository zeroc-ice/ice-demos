//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "Contact.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Demo;

int run(const shared_ptr<Ice::Communicator>&, const string&);

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
        // and its dtor destroys this communicator.
        //
        const Ice::CommunicatorHolder ich(argc, argv, "config.client");
        const auto& communicator = ich.communicator();

        ctrlCHandler.setCallback([communicator](int) { communicator->destroy(); });

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
            status = run(communicator, argv[0]);
        }
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}

int
run(const shared_ptr<Ice::Communicator>& communicator, const string& appName)
{
    auto contactDb = Ice::checkedCast<ContactDBPrx>(communicator->propertyToProxy("ContactDB.Proxy"));
    if (!contactDb)
    {
        cerr << appName << ": invalid proxy" << endl;
        return 1;
    }

    //
    // Add a contact for "john". All parameters are provided.
    //
    const string johnNumber = "123-456-7890";
    contactDb->addContact("john", NumberType::HOME, johnNumber, 0);

    cout << "Checking john... " << flush;

    //
    // Find the phone number for "john" and put it in a Ice::optional, a temporary alias
    // for C++17 std::optional http://en.cppreference.com/w/cpp/utility/optional
    //
    // number is an Ice::optional<std::string>
    //
    auto number = contactDb->queryNumber("john");
    //
    // tests if an optional value is set.
    //
    if (!number)
    {
        cout << "number is incorrect " << flush;
    }

    //
    // Operator overloading makes comparison convenient.
    //
    if (number != johnNumber)
    {
        cout << "number is incorrect " << flush;
    }

    //
    // .value() can also be called to get the value directly if necessary.
    //
    if (number.value() != johnNumber)
    {
        cout << "number is incorrect " << flush;
    }

    //
    // operator-> is overloaded, meaning members of the contained type
    // can be called directly.
    //
    if (number->size() != johnNumber.size())
    {
        cout << "number is incorrect " << flush;
    }

    // Optional can also be used in an out parameter.
    optional<int> dialGroup;
    contactDb->queryDialgroup("john", dialGroup);
    if (!dialGroup || dialGroup != 0)
    {
        cout << "dialGroup is incorrect " << flush;
    }

    auto info = contactDb->query("john");
    //
    // All of the info parameters should be set.
    //
    if (!info->type || !info->number || !info->dialGroup)
    {
        cout << "info is incorrect " << flush;
    }
    if (info->type != NumberType::HOME || info->number != johnNumber || info->dialGroup != 0)
    {
        cout << "info is incorrect " << flush;
    }
    cout << "ok" << endl;

    //
    // Add a contact for "steve". The behavior of the server is to
    // default construct the Contact, and then assign all set parameters.
    // Since the default value of NumberType in the slice definition
    // is HOME and in this case the NumberType is unset it will take
    // the default value.
    //
    const string steveNumber = "234-567-8901";
    contactDb->addContact("steve", nullopt, steveNumber, 1);

    cout << "Checking steve... " << flush;
    number = contactDb->queryNumber("steve");
    if (number != steveNumber)
    {
        cout << "number is incorrect " << flush;
    }

    info = contactDb->query("steve");
    //
    // Check the value for the NumberType.
    //
    if (!info->type || info->type != NumberType::HOME)
    {
        cout << "info is incorrect " << flush;
    }

    if (info->number != steveNumber || info->dialGroup != 1)
    {
        cout << "info is incorrect " << flush;
    }

    contactDb->queryDialgroup("steve", dialGroup);
    if (!dialGroup || dialGroup != 1)
    {
        cout << "dialGroup is incorrect " << flush;
    }

    cout << "ok" << endl;

    //
    // Add a contact from "frank". Here the dialGroup field isn't set.
    //
    const string frankNumber = "345-678-9012";
    contactDb->addContact("frank", NumberType::CELL, frankNumber, nullopt);

    cout << "Checking frank... " << flush;

    number = contactDb->queryNumber("frank");
    if (number != frankNumber)
    {
        cout << "number is incorrect " << flush;
    }

    info = contactDb->query("frank");
    //
    // The dial group field should be unset.
    //
    if (info->dialGroup)
    {
        cout << "info is incorrect " << flush;
    }
    if (info->type != NumberType::CELL || info->number != frankNumber)
    {
        cout << "info is incorrect " << flush;
    }

    contactDb->queryDialgroup("frank", dialGroup);
    if (dialGroup)
    {
        cout << "dialGroup is incorrect " << flush;
    }
    cout << "ok" << endl;

    //
    // Add a contact from "anne". The number field isn't set.
    //
    contactDb->addContact("anne", NumberType::OFFICE, nullopt, 2);

    cout << "Checking anne... " << flush;
    number = contactDb->queryNumber("anne");
    if (number)
    {
        cout << "number is incorrect " << flush;
    }

    info = contactDb->query("anne");
    //
    // The number field should be unset.
    //
    if (info->number)
    {
        cout << "info is incorrect " << flush;
    }
    if (info->type != NumberType::OFFICE || info->dialGroup != 2)
    {
        cout << "info is incorrect " << flush;
    }

    contactDb->queryDialgroup("anne", dialGroup);
    if (!dialGroup || dialGroup != 2)
    {
        cout << "dialGroup is incorrect " << flush;
    }

    //
    // The optional fields can be used to determine what fields to
    // update on the contact.  Here we update only the number for anne,
    // the remainder of the fields are unchanged.
    //
    const string anneNumber = "456-789-0123";
    contactDb->updateContact("anne", nullopt, anneNumber, nullopt);
    number = contactDb->queryNumber("anne");
    if (number != anneNumber)
    {
        cout << "number is incorrect " << flush;
    }
    info = contactDb->query("anne");
    if (info->number != anneNumber || info->type != NumberType::OFFICE || info->dialGroup != 2)
    {
        cout << "info is incorrect " << flush;
    }
    cout << "ok" << endl;

    contactDb->shutdown();

    return 0;
}
