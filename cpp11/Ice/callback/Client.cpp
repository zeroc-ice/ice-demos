// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
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

class CallbackClient : public Ice::Application
{
public:

    CallbackClient();

    virtual int run(int, char*[]);

private:

    void menu();
};


int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    CallbackClient app;
    return app.main(argc, argv, "config.client");
}

CallbackClient::CallbackClient() :
    //
    // Since this is an interactive demo we don't want any signal
    // handling.
    //
    Ice::Application(Ice::NoSignalHandling)
{
}

int
CallbackClient::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto sender = Ice::checkedCast<CallbackSenderPrx>(
        communicator()->propertyToProxy("CallbackSender.Proxy")->ice_twoway()->ice_timeout(-1)->ice_secure(false));
    if(!sender)
    {
        cerr << appName() << ": invalid proxy" << endl;
        return EXIT_FAILURE;
    }

    auto adapter = communicator()->createObjectAdapter("Callback.Client");
    adapter->add(make_shared<CallbackReceiverI>(), Ice::stringToIdentity("callbackReceiver"));
    adapter->activate();

    auto receiver = Ice::uncheckedCast<CallbackReceiverPrx>(
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
CallbackClient::menu()
{
    cout <<
        "usage:\n"
        "t: send callback\n"
        "s: shutdown server\n"
        "x: exit\n"
        "?: help\n";
}
