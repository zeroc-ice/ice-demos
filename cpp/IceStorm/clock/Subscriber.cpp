// Copyright (c) ZeroC, Inc.

#include "Clock.h"

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

#include <iostream>

using namespace std;
using namespace Demo;

/// ClockI is an Ice servant that implements Slice interface Clock.
class ClockI final : public Clock
{
public:
    void tick(string time, const Ice::Current&) final { cout << time << endl; }
};

void
usage(const char* name)
{
    cerr << "Usage: " << name
         << " [--batch] [--datagram|--twoway|--ordered|--oneway] [--retryCount count] [--id id] [topic]" << endl;
}

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceUDP();
#endif

    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
    // of the program, before creating an Ice communicator or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator to initialize the Ice runtime.
    const Ice::CommunicatorHolder communicatorHolder{argc, argv, "config.sub"};
    const Ice::CommunicatorPtr& communicator = communicatorHolder.communicator();

    // Parse command-line options.

    auto args = Ice::argsToStringSeq(argc, argv);
    args = communicator->getProperties()->parseCommandLineOptions("Clock", args);
    Ice::stringSeqToArgs(args, argc, argv);

    bool batch = false;
    enum class Option : uint8_t
    {
        None,
        Datagram,
        Twoway,
        Oneway,
        Ordered
    };

    Option option = Option::None;
    string topicName = "time";
    string id;
    string retryCount;
    int i;

    for (i = 1; i < argc; ++i)
    {
        const string optionString = argv[i];
        const Option oldoption = option;
        if (optionString == "--datagram")
        {
            option = Option::Datagram;
        }
        else if (optionString == "--twoway")
        {
            option = Option::Twoway;
        }
        else if (optionString == "--oneway")
        {
            option = Option::Oneway;
        }
        else if (optionString == "--ordered")
        {
            option = Option::Ordered;
        }
        else if (optionString == "--batch")
        {
            batch = true;
        }
        else if (optionString == "--id")
        {
            ++i;
            if (i >= argc)
            {
                usage(argv[0]);
                return 1;
            }
            id = argv[i];
        }
        else if (optionString == "--retryCount")
        {
            ++i;
            if (i >= argc)
            {
                usage(argv[0]);
                return 1;
            }
            retryCount = argv[i];
        }
        else if (optionString.substr(0, 2) == "--")
        {
            usage(argv[0]);
            return 1;
        }
        else
        {
            topicName = argv[i++];
            break;
        }

        if (oldoption != option && oldoption != Option::None)
        {
            usage(argv[0]);
            return 1;
        }
    }

    if (i != argc)
    {
        usage(argv[0]);
        return 1;
    }

    if (batch && (option == Option::Twoway || option == Option::Ordered))
    {
        cerr << argv[0] << ": batch can only be set with oneway or datagram" << endl;
        return 1;
    }

    // Create the topic manager proxy.
    auto manager = communicator->propertyToProxy<IceStorm::TopicManagerPrx>("TopicManager.Proxy");
    if (!manager)
    {
        cerr << argv[0] << ": invalid proxy" << endl;
        return 1;
    }

    // Retrieve the topic from IceStorm.
    optional<IceStorm::TopicPrx> topic;
    try
    {
        topic = manager->retrieve(topicName);
    }
    catch (const IceStorm::NoSuchTopic&)
    {
        try
        {
            topic = manager->create(topicName);
        }
        catch (const IceStorm::TopicExists&)
        {
            cerr << argv[0] << ": temporary failure. try again." << endl;
            return 1;
        }
    }

    // Create the object adapter that hosts the subscriber servant.
    auto adapter = communicator->createObjectAdapter("Clock.Subscriber");

    // Add a servant to the object adapter. If --id is used the identity comes from the command line, otherwise a UUID
    // is used. id is not directly altered since it is used below to detect whether subscribeAndGetPublisher can throw
    // AlreadySubscribed.
    Ice::Identity subId{.name = id};
    if (subId.name.empty())
    {
        subId.name = Ice::generateUUID();
    }
    Ice::ObjectPrx subscriber = adapter->add(make_shared<ClockI>(), subId);

    // Activate the object adapter before subscribing.
    adapter->activate();

    IceStorm::QoS qos;
    if (!retryCount.empty())
    {
        qos["retryCount"] = retryCount;
    }

    // Set up the subscriber proxy.
    if (option == Option::Datagram)
    {
        if (batch)
        {
            subscriber = subscriber->ice_batchDatagram();
        }
        else
        {
            subscriber = subscriber->ice_datagram();
        }
    }
    else if (option == Option::Twoway)
    {
        // Do nothing to the subscriber proxy. Its already twoway.
    }
    else if (option == Option::Ordered)
    {
        // Do nothing to the subscriber proxy. Its already twoway.
        qos["reliability"] = "ordered";
    }
    else if (option == Option::Oneway || option == Option::None)
    {
        if (batch)
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
        // Register the subscriber with the IceStorm topic.
        topic->subscribeAndGetPublisher(qos, subscriber);
    }
    catch (const IceStorm::AlreadySubscribed&)
    {
        // This should never occur when subscribing with a UUID.
        assert(!id.empty());
        cout << "reactivating persistent subscriber" << endl;
    }

    // Wait until the user presses Ctrl+C.
    int signal = ctrlCHandler.wait();
    cout << "Caught signal " << signal << ", exiting..." << endl;

    // Initiate shutdown and wait for all dispatches to complete.
    communicator->shutdown();
    communicator->waitForShutdown();

    // Unsubscribe from the topic in IceStorm.
    topic->unsubscribe(subscriber);
    return 0;
}
