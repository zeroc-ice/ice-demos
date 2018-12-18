// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

#include <CounterI.h>

#include <map>

using namespace std;
using namespace Demo;

int main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        //
        // CtrlCHandler must be created before the communicator or any other threads are started
        //
        Ice::CtrlCHandler ctrlCHandler;

        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config.server");
        auto communicator = ich.communicator();

        ctrlCHandler.setCallback(
            [communicator](int)
            {
                communicator->shutdown();
            });

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
            auto properties = communicator->getProperties();

            auto manager = Ice::checkedCast<IceStorm::TopicManagerPrx>(
                communicator->propertyToProxy("TopicManager.Proxy"));
            if(!manager)
            {
                cerr << argv[0] << ": invalid proxy" << endl;
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
                    cerr << argv[0] << ": topic exists, please try again." << endl;
                    return 1;
                }
            }

            //
            // Create the servant to receive the events.
            //
            auto adapter = communicator->createObjectAdapter("Counter");
            auto counter = make_shared<CounterI>(topic);
            adapter->add(counter, Ice::stringToIdentity("counter"));
            adapter->activate();

            communicator->waitForShutdown();
        }
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
