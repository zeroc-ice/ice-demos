// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <ContactDBI.h>

using namespace std;

class ContactServer : public Ice::Application
{
public:

    virtual int run(int, char*[]);
};


int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    ContactServer app;
    return app.main(argc, argv, "config.server");
}

int
ContactServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("ContactDB");
    Demo::ContactDBPtr contactdb = new ContactDBI;
    adapter->add(contactdb, Ice::stringToIdentity("contactdb"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
