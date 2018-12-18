// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

#include <Clock.h>

using namespace std;
using namespace Demo;

int run(int, char*[]);

//
// Global variable for destroyCommunicator
//
Ice::CommunicatorPtr communicator;

//
// Callback for CtrlCHandler
//
void
destroyCommunicator(int)
{
    communicator->destroy();
}

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceUDP();
#endif
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
        Ice::CommunicatorHolder ich(argc, argv, "config.pub");
        communicator = ich.communicator();

        //
        // Destroy communicator on Ctrl-C
        //
        ctrlCHandler.setCallback(&destroyCommunicator);

        status = run(argc, argv);
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}

void
usage(const string& n)
{
    cerr << "Usage: " << n << " [--datagram|--twoway|--oneway] [topic]" << endl;
}

int
run(int argc, char* argv[])
{
    enum Option { None, Datagram, Twoway, Oneway };
    Option option = None;
    string topicName = "time";
    int i;

    for(i = 1; i < argc; ++i)
    {
        string optionString = argv[i];
        Option oldoption = option;
        if(optionString == "--datagram")
        {
            option = Datagram;
        }
        else if(optionString == "--twoway")
        {
            option = Twoway;
        }
        else if(optionString == "--oneway")
        {
            option = Oneway;
        }
        else if(optionString.substr(0, 2) == "--")
        {
            usage(argv[0]);
            return 1;
        }
        else
        {
            topicName = argv[i++];
            break;
        }

        if(oldoption != option && oldoption != None)
        {
            usage(argv[0]);
            return 1;
        }
    }

    if(i != argc)
    {
        usage(argv[0]);
        return 1;
    }

    IceStorm::TopicManagerPrx manager = IceStorm::TopicManagerPrx::checkedCast(
        communicator->propertyToProxy("TopicManager.Proxy"));
    if(!manager)
    {
        cerr << argv[0] << ": invalid proxy" << endl;
        return 1;
    }

    //
    // Retrieve the topic.
    //
    IceStorm::TopicPrx topic;
    try
    {
        topic = manager->retrieve(topicName);
    }
    catch(const IceStorm::NoSuchTopic&)
    {
        try
        {
            topic = manager->create(topicName);
        }
        catch(const IceStorm::TopicExists&)
        {
            cerr << argv[0] << ": temporary failure. try again." << endl;
            return 1;
        }
    }

    //
    // Get the topic's publisher object, and create a Clock proxy with
    // the mode specified as an argument of this application.
    //
    Ice::ObjectPrx publisher = topic->getPublisher();
    if(option == Datagram)
    {
        publisher = publisher->ice_datagram();
    }
    else if(option == Twoway)
    {
        // Do nothing.
    }
    else if(option == Oneway || option == None)
    {
        publisher = publisher->ice_oneway();
    }

    ClockPrx clock = ClockPrx::uncheckedCast(publisher);

    cout << "publishing tick events. Press ^C to terminate the application." << endl;
    try
    {
        bool stop = false;
        while(!stop)
        {
            clock->tick(IceUtil::Time::now().toDateTime());
            IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(1));
        }
    }
    catch(const Ice::CommunicatorDestroyedException&)
    {
        // Ignore
    }

    return 0;
}
