// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <memory>
#include <Ice/Ice.h>
#include <CalculatorI.h>

using namespace std;

class Server : public Ice::Application
{
public:

    virtual int run(int, char*[]) override;
};

int
main(int argc, char* argv[])
{
    Server app;
    return app.main(argc, argv, "config.server");
}

int
Server::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return 1;
    }

    auto adapter = communicator()->createObjectAdapter("Calculator");
    adapter->add(make_shared<CalculatorI>(), Ice::stringToIdentity("calculator"));
    adapter->activate();

    communicator()->waitForShutdown();
    return 0;
}
