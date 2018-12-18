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

class ClockI : public Clock
{
public:

    virtual void
    tick(const string& time, const Ice::Current&)
    {
        cout << time << endl;
    }
};

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
        Ice::CommunicatorHolder ich(argc, argv, "config.sub");
        communicator = ich.communicator();

        //
        // Shutdown communicator on Ctrl-C
        //
        ctrlCHandler.setCallback(&shutdownCommunicator);

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
     cerr << "Usage: " << n
          << " [--batch] [--datagram|--twoway|--ordered|--oneway] [--retryCount count] [--id id] [topic]" << endl;
}

int
run(int argc, char* argv[])
{
    Ice::StringSeq args = Ice::argsToStringSeq(argc, argv);
    args = communicator->getProperties()->parseCommandLineOptions("Clock", args);
    Ice::stringSeqToArgs(args, argc, argv);

    bool batch = false;
    enum Option { None, Datagram, Twoway, Oneway, Ordered};
    Option option = None;
    string topicName = "time";
    string id;
    string retryCount;
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
        else if(optionString == "--ordered")
        {
            option = Ordered;
        }
        else if(optionString == "--batch")
        {
            batch = true;
        }
        else if(optionString == "--id")
        {
            ++i;
            if(i >= argc)
            {
                usage(argv[0]);
                return 1;
            }
            id = argv[i];
        }
        else if(optionString == "--retryCount")
        {
            ++i;
            if(i >= argc)
            {
                usage(argv[0]);
                return 1;
            }
            retryCount = argv[i];
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

    if(!retryCount.empty())
    {
        if(option == None)
        {
            option = Twoway;
        }
        else if(option != Twoway && option != Ordered)
        {
            cerr << argv[0] << ": retryCount requires a twoway proxy" << endl;
            return 1;
        }
    }

    if(batch && (option == Twoway || option == Ordered))
    {
        cerr << argv[0] << ": batch can only be set with oneway or datagram" << endl;
        return 1;
    }

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

    Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("Clock.Subscriber");

    //
    // Add a servant for the Ice object. If --id is used the identity
    // comes from the command line, otherwise a UUID is used.
    //
    // id is not directly altered since it is used below to detect
    // whether subscribeAndGetPublisher can raise AlreadySubscribed.
    //
    Ice::Identity subId;
    subId.name = id;
    if(subId.name.empty())
    {
        subId.name = Ice::generateUUID();
    }
    Ice::ObjectPrx subscriber = adapter->add(new ClockI, subId);

    //
    // Activate the object adapter before subscribing.
    //
    adapter->activate();

    IceStorm::QoS qos;
    if(!retryCount.empty())
    {
        qos["retryCount"] = retryCount;
    }

    //
    // Set up the proxy.
    //
    if(option == Datagram)
    {
        if(batch)
        {
            subscriber = subscriber->ice_batchDatagram();
        }
        else
        {
            subscriber = subscriber->ice_datagram();
        }
    }
    else if(option == Twoway)
    {
        // Do nothing to the subscriber proxy. Its already twoway.
    }
    else if(option == Ordered)
    {
        // Do nothing to the subscriber proxy. Its already twoway.
        qos["reliability"] = "ordered";
    }
    else if(option == Oneway || option == None)
    {
        if(batch)
        {
            subscriber = subscriber->ice_batchOneway();
        }
        else
        {
            subscriber = subscriber->ice_oneway();
        }
    }

    try
    {
        topic->subscribeAndGetPublisher(qos, subscriber);
    }
    catch(const IceStorm::AlreadySubscribed&)
    {
        // If we're manually setting the subscriber id ignore.
        if(id.empty())
        {
            throw;
        }
        cout << "reactivating persistent subscriber" << endl;
    }

    communicator->waitForShutdown();
    topic->unsubscribe(subscriber);
    return 0;
}
