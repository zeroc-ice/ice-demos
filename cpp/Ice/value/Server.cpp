// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <ValueI.h>
#include <ValueFactory.h>

using namespace std;

class ValueServer : public Ice::Application
{
public:

    virtual int run(int, char*[]);
};

int
main(int argc, char* argv[])
{
    ValueServer app;
    return app.main(argc, argv, "config.server");
}

int
ValueServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    Ice::ValueFactoryPtr factory = new ValueFactory;
    communicator()->getValueFactoryManager()->add(factory, Demo::Printer::ice_staticId());

    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Value");
    Demo::InitialPtr initial = new InitialI(adapter);
    adapter->add(initial, Ice::stringToIdentity("initial"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
