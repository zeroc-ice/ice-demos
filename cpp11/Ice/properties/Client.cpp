// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Props.h>

using namespace std;

using namespace Demo;

class PropsClient : public Ice::Application
{
public:

    PropsClient();

    virtual int run(int, char*[]);

private:

    void menu();
    void show(const shared_ptr<Ice::PropertiesAdminPrx>&);
};


int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    PropsClient app;
    return app.main(argc, argv, "config.client");
}

PropsClient::PropsClient() :
    //
    // Since this is an interactive demo we don't want any signal
    // handling.
    //
    Ice::Application(Ice::NoSignalHandling)
{
}

int
PropsClient::run(int argc, char* argv[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto props = Ice::checkedCast<PropsPrx>(communicator()->propertyToProxy("Props.Proxy"));
    if(!props)
    {
        cerr << argv[0] << ": invalid proxy" << endl;
        return EXIT_FAILURE;
    }

    auto admin = Ice::checkedCast<Ice::PropertiesAdminPrx>(communicator()->propertyToProxy("Admin.Proxy"));

    Ice::PropertyDict batch1;
    batch1["Demo.Prop1"] = "1";
    batch1["Demo.Prop2"] = "2";
    batch1["Demo.Prop3"] = "3";

    Ice::PropertyDict batch2;
    batch2["Demo.Prop1"] = "10";
    batch2["Demo.Prop2"] = ""; // An empty value removes this property
    batch2["Demo.Prop3"] = "30";

    show(admin);
    menu();

    char c = 'x';
    do
    {
        try
        {
            cout << "==> ";
            cin >> c;
            if(c == '1' || c == '2')
            {
                auto dict = c == '1' ? batch1 : batch2;
                cout << "Sending:" << endl;
                for(const auto& p : dict)
                {
                    if(p.first.find("Demo") == 0)
                    {
                        cout << "  " << p.first << "=" << p.second << endl;
                    }
                }
                cout << endl;

                admin->setProperties(dict);

                cout << "Changes:" << endl;
                Ice::PropertyDict changes = props->getChanges();
                if(changes.empty())
                {
                    cout << "  None." << endl;
                }
                else
                {
                    for(const auto& p : changes)
                    {
                        cout << "  " << p.first;
                        if(p.second.empty())
                        {
                            cout << " was removed" << endl;
                        }
                        else
                        {
                            cout << " is now " << p.second << endl;
                        }
                    }
                }
            }
            else if(c == 'c')
            {
                show(admin);
            }
            else if(c == 's')
            {
                props->shutdown();
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
PropsClient::menu()
{
    cout << endl <<
        "usage:\n"
        "1: set properties (batch 1)\n"
        "2: set properties (batch 2)\n"
        "c: show current properties\n"
        "s: shutdown server\n"
        "x: exit\n"
        "?: help\n";
}

void
PropsClient::show(const shared_ptr<Ice::PropertiesAdminPrx>& admin)
{
    auto props = admin->getPropertiesForPrefix("Demo");
    cout << "Server's current settings:" << endl;
    for(const auto& p : props)
    {
        cout << "  " << p.first << "=" << p.second << endl;
    }
}
