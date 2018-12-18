// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Contact.h>

using namespace std;
using namespace Demo;

int run();

//
// Global variable for destroyCommunicator
//
Ice::CommunicatorPtr communicator;

//
// Callback for CtrlCHandler
//
void
destroyCommunicator(int)
{
    communicator->destroy();
}

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
        Ice::CommunicatorHolder ich(argc, argv, "config.client");
        communicator = ich.communicator();

        //
        // Destroy communicator on Ctrl-C
        //
        ctrlCHandler.setCallback(&destroyCommunicator);

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
            status = run();
        }
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}

int
run()
{
    ContactDBPrx contactdb = ContactDBPrx::checkedCast(communicator->propertyToProxy("ContactDB.Proxy"));
    if(!contactdb)
    {
        cerr << "invalid proxy" << endl;
        return 1;
    }

    //
    // Add a contact for "john". All parameters are provided.
    //
    const string johnNumber = "123-456-7890";
    contactdb->addContact("john", HOME, johnNumber, 0);

    cout << "Checking john... " << flush;

    //
    // Find the phone number for "john"
    //
    IceUtil::Optional<string> number = contactdb->queryNumber("john");
    //
    // operator bool() tests if an optional value is set.
    //
    if(!number)
    {
        cout << "number is incorrect " << flush;
    }

    //
    // Operator overloading on the IceUtil::Optional makes comparison convenient.
    //
    if(number != johnNumber)
    {
        cout << "number is incorrect " << flush;
    }

    //
    // .get() can also be called to get the value directly if necessary.
    //
    if(number.get() != johnNumber)
    {
        cout << "number is incorrect " << flush;
    }

    //
    // operator-> is overloaded, meaning members of the contained type
    // can be called directly.
    //
    if(number->size() != johnNumber.size())
    {
        cout << "number is incorrect " << flush;
    }

    // Optional can also be used in an out parameter.
    IceUtil::Optional<Ice::Int> dialgroup;
    contactdb->queryDialgroup("john", dialgroup);
    if(!dialgroup || dialgroup != 0)
    {
        cout << "dialgroup is incorrect " << flush;
    }

    ContactPtr info = contactdb->query("john");
    //
    // All of the info parameters should be set.
    //
    if(!info->type || !info->number || !info->dialGroup)
    {
        cout << "info is incorrect " << flush;
    }
    if(info->type != HOME || info->number != johnNumber || info->dialGroup != 0)
    {
        cout << "info is incorrect " << flush;
    }
    cout << "ok" << endl;

    //
    // Add a contact for "steve". The behavior of the server is to
    // default construct the Contact, and then assign  all set parameters.
    // Since the default value of NumberType in the slice definition
    // is HOME and in this case the NumberType is unset it will take
    // the default value.
    //
    const string steveNumber = "234-567-8901";
    contactdb->addContact("steve", IceUtil::None, steveNumber, 1);

    cout << "Checking steve... " << flush;
    number = contactdb->queryNumber("steve");
    if(number != steveNumber)
    {
        cout << "number is incorrect " << flush;
    }

    info = contactdb->query("steve");
    //
    // Check the value for the NumberType.
    //
    if(!info->type || info->type != HOME)
    {
        cout << "info is incorrect " << flush;
    }

    if(info->number != steveNumber || info->dialGroup != 1)
    {
        cout << "info is incorrect " << flush;
    }

    contactdb->queryDialgroup("steve", dialgroup);
    if(!dialgroup || dialgroup != 1)
    {
        cout << "dialgroup is incorrect " << flush;
    }

    cout << "ok" << endl;

    //
    // Add a contact from "frank". Here the dialGroup field isn't set.
    //
    const string frankNumber = "345-678-9012";
    contactdb->addContact("frank", CELL, frankNumber, IceUtil::None);

    cout << "Checking frank... " << flush;

    number = contactdb->queryNumber("frank");
    if(number != frankNumber)
    {
        cout << "number is incorrect " << flush;
    }

    info = contactdb->query("frank");
    //
    // The dial group field should be unset.
    //
    if(info->dialGroup)
    {
        cout << "info is incorrect " << flush;
    }
    if(info->type != CELL || info->number != frankNumber)
    {
        cout << "info is incorrect " << flush;
    }

    contactdb->queryDialgroup("frank", dialgroup);
    if(dialgroup)
    {
        cout << "dialgroup is incorrect " << flush;
    }
    cout << "ok" << endl;

    //
    // Add a contact from "anne". The number field isn't set.
    //
    contactdb->addContact("anne", OFFICE, IceUtil::None, 2);

    cout << "Checking anne... " << flush;
    number = contactdb->queryNumber("anne");
    if(number)
    {
        cout << "number is incorrect " << flush;
    }

    info = contactdb->query("anne");
    //
    // The number field should be unset.
    //
    if(info->number)
    {
        cout << "info is incorrect " << flush;
    }
    if(info->type != OFFICE || info->dialGroup != 2)
    {
        cout << "info is incorrect " << flush;
    }

    contactdb->queryDialgroup("anne", dialgroup);
    if(!dialgroup || dialgroup != 2)
    {
        cout << "dialgroup is incorrect " << flush;
    }

    //
    // The optional fields can be used to determine what fields to
    // update on the contact.  Here we update only the number for anne,
    // the remainder of the fields are unchanged.
    //
    const string anneNumber = "456-789-0123";
    contactdb->updateContact("anne", IceUtil::None, anneNumber, IceUtil::None);
    number = contactdb->queryNumber("anne");
    if(number != anneNumber)
    {
        cout << "number is incorrect " << flush;
    }
    info = contactdb->query("anne");
    if(info->number != anneNumber || info->type != OFFICE || info->dialGroup != 2)
    {
        cout << "info is incorrect " << flush;
    }
    cout << "ok" << endl;

    contactdb->shutdown();

    return 0;
}
