// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Callback.h>

using namespace std;
using namespace Demo;

class CallbackReceiverI : public CallbackReceiver
{
public:

    virtual void callback(const Ice::Current&)
    {
        cout << "received callback" << endl;
    }
};

int run(const Ice::CommunicatorPtr&);

int
main(int argc, char* argv[])
{
    int status = EXIT_SUCCESS;

    try
    {
        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config.client");

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if(argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = EXIT_FAILURE;
        }
        else
        {
            status = run(ich.communicator());
        }
    }
    catch(const std::exception& ex)
    {
        cerr << argv[0] << ": " << ex.what() << endl;
        status = EXIT_FAILURE;
    }

    return status;
}

void menu();

int
run(const Ice::CommunicatorPtr& communicator)
{
    CallbackSenderPrx sender = CallbackSenderPrx::checkedCast(
        communicator->propertyToProxy("CallbackSender.Proxy")->ice_twoway()->ice_timeout(-1)->ice_secure(false));
    if(!sender)
    {
        cerr << "invalid proxy" << endl;
        return EXIT_FAILURE;
    }

    Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("Callback.Client");
    CallbackReceiverPtr cr = new CallbackReceiverI;
    adapter->add(cr, Ice::stringToIdentity("callbackReceiver"));
    adapter->activate();

    CallbackReceiverPrx receiver = CallbackReceiverPrx::uncheckedCast(
        adapter->createProxy(Ice::stringToIdentity("callbackReceiver")));

    menu();

    char c = 'x';
    do
    {
        try
        {
            cout << "==> ";
            cin >> c;
            if(c == 't')
            {
                sender->initiateCallback(receiver);
            }
            else if(c == 's')
            {
                sender->shutdown();
            }
            else if(c == 'x')
            {
                // Nothing to do
            }
            else if(c == '?')
            {
                menu();
            }
            else
            {
                cout << "unknown command `" << c << "'" << endl;
                menu();
            }
        }
        catch(const Ice::Exception& ex)
        {
            cerr << ex << endl;
        }
    }
    while(cin.good() && c != 'x');

    return EXIT_SUCCESS;
}

void
menu()
{
    cout <<
        "usage:\n"
        "t: send callback\n"
        "s: shutdown server\n"
        "x: exit\n"
        "?: help\n";
}
