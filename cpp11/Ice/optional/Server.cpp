// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <ContactDBI.h>

using namespace std;

class ContactServer : public Ice::Application
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

    auto adapter = communicator()->createObjectAdapter("ContactDB");
    auto contactdb = make_shared<ContactDBI>();
    adapter->add(contactdb, Ice::stringToIdentity("contactdb"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
