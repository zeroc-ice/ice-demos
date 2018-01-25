// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

// TODO: remove when no longer needed
#ifdef _MSC_VER
#    pragma warning(disable:4702) // unreachable code in optional
#endif

#include <Ice/Ice.h>
#include <Contact.h>

using namespace std;
using namespace Demo;

class ContactClient : public Ice::Application
{
public:

    virtual int run(int, char*[]) override;
};

int
#ifdef _WIN32
wmain(int argc, wchar_t* argv[])
#else
main(int argc, char* argv[])
#endif
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    ContactClient app;
    return app.main(argc, argv, "config.client");
}

int
ContactClient::run(int argc, char* argv[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto contactdb = Ice::checkedCast<ContactDBPrx>(communicator()->propertyToProxy("ContactDB.Proxy"));
    if(!contactdb)
    {
        cerr << argv[0] << ": invalid proxy" << endl;
        return EXIT_FAILURE;
    }

    //
    // Add a contact for "john". All parameters are provided.
    //
    const string johnNumber = "123-456-7890";
    contactdb->addContact("john", NumberType::HOME, johnNumber, 0);

    cout << "Checking john... " << flush;

    //
    // Find the phone number for "john" and put it in a Ice::optional, a temporary alias
    // for C++17 std::optional http://en.cppreference.com/w/cpp/utility/optional
    //
    // number is an Ice::optional<std::string>
    //
    auto number = contactdb->queryNumber("john");
    //
    // tests if an optional value is set.
    //
    if(!number)
    {
        cout << "number is incorrect " << flush;
    }

    //
    // Operator overloading makes comparison convenient.
    //
    if(number != johnNumber)
    {
        cout << "number is incorrect " << flush;
    }

    //
    // .value() can also be called to get the value directly if necessary.
    //
    if(number.value() != johnNumber)
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
    Ice::optional<int> dialgroup;
    contactdb->queryDialgroup("john", dialgroup);
    if(!dialgroup || dialgroup != 0)
    {
        cout << "dialgroup is incorrect " << flush;
    }

    auto info = contactdb->query("john");
    //
    // All of the info parameters should be set.
    //
    if(!info->type || !info->number || !info->dialGroup)
    {
        cout << "info is incorrect " << flush;
    }
    if(info->type != NumberType::HOME || info->number != johnNumber || info->dialGroup != 0)
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
    contactdb->addContact("steve", Ice::nullopt, steveNumber, 1);

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
    if(!info->type || info->type != NumberType::HOME)
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
    contactdb->addContact("frank", NumberType::CELL, frankNumber, Ice::nullopt);

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
    if(info->type != NumberType::CELL || info->number != frankNumber)
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
    contactdb->addContact("anne", NumberType::OFFICE, Ice::nullopt, 2);

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
    if(info->type != NumberType::OFFICE || info->dialGroup != 2)
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
    contactdb->updateContact("anne", Ice::nullopt, anneNumber, Ice::nullopt);
    number = contactdb->queryNumber("anne");
    if(number != anneNumber)
    {
        cout << "number is incorrect " << flush;
    }
    info = contactdb->query("anne");
    if(info->number != anneNumber || info->type != NumberType::OFFICE || info->dialGroup != 2)
    {
        cout << "info is incorrect " << flush;
    }
    cout << "ok" << endl;

    contactdb->shutdown();

    return EXIT_SUCCESS;
}
