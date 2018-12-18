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

//
// Global variable for shutdownCommunicator
//
Ice::CommunicatorPtr communicator;

//
// Callback for CtrlCHandler
//
void
shutdownCommunicator(int)
{
    communicator->shutdown();
}

int
main(int argc, char* argv[])
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
        // and it's dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config.server");
        communicator = ich.communicator();

        //
        // Shutdown communicator on Ctrl-C
        //
        ctrlCHandler.setCallback(&shutdownCommunicator);

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
            Ice::PropertiesPtr properties = communicator->getProperties();

            IceStorm::TopicManagerPrx manager = IceStorm::TopicManagerPrx::checkedCast(
                communicator->propertyToProxy("TopicManager.Proxy"));
            if(!manager)
            {
                cerr << argv[0] << ": invalid proxy" << endl;
                return 1;
            }

            IceStorm::TopicPrx topic;
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
            Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("Counter");
            Demo::CounterPtr counter = new CounterI(topic);
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
