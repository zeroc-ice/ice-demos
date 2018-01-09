// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

#include <CounterI.h>

#include <map>

using namespace std;
using namespace Demo;

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

    auto properties = communicator()->getProperties();

    auto manager = Ice::checkedCast<IceStorm::TopicManagerPrx>(
        communicator()->propertyToProxy("TopicManager.Proxy"));
    if(!manager)
    {
        cerr << appName() << ": invalid proxy" << endl;
        return 1;
    }

    shared_ptr<IceStorm::TopicPrx> topic;
    try
    {
        topic = manager->retrieve("counter");
    }
    catch(const IceStorm::NoSuchTopic&)
    {
        try
        {
            topic = manager->create("counter");
        }
        catch(const IceStorm::TopicExists&)
        {
            cerr << appName() << ": topic exists, please try again." << endl;
            return 1;
        }
    }

    //
    // Create the servant to receive the events.
    //
    auto adapter = communicator()->createObjectAdapter("Counter");
    auto counter = make_shared<CounterI>(topic);
    adapter->add(counter, Ice::stringToIdentity("counter"));
    adapter->activate();

    communicator()->waitForShutdown();

    return 0;
}
