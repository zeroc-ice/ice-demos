// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

#include <Clock.h>

using namespace std;
using namespace Demo;

class ClockI : public Clock
{
public:

    virtual void
    tick(string time, const Ice::Current&) override
    {
        cout << time << endl;
    }
};

class Subscriber : public Ice::Application
{
public:

    virtual int run(int, char*[]) override;
};

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceUDP();
#endif

    Subscriber app;
    return app.main(argc, argv, "config.sub");
}

void
usage(const string& n)
{
     cerr << "Usage: " << n
          << " [--batch] [--datagram|--twoway|--ordered|--oneway] [--retryCount count] [--id id] [topic]" << endl;
}

int
Subscriber::run(int argc, char* argv[])
{
    auto args = Ice::argsToStringSeq(argc, argv);
    args = communicator()->getProperties()->parseCommandLineOptions("Clock", args);
    Ice::stringSeqToArgs(args, argc, argv);

    bool batch = false;
    enum class Option { None, Datagram, Twoway, Oneway, Ordered};
    Option option = Option::None;
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
            option = Option::Datagram;
        }
        else if(optionString == "--twoway")
        {
            option = Option::Twoway;
        }
        else if(optionString == "--oneway")
        {
            option = Option::Oneway;
        }
        else if(optionString == "--ordered")
        {
            option = Option::Ordered;
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

        if(oldoption != option && oldoption != Option::None)
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
        if(option == Option::None)
        {
            option = Option::Twoway;
        }
        else if(option != Option::Twoway && option != Option::Ordered)
        {
            cerr << argv[0] << ": retryCount requires a twoway proxy" << endl;
            return 1;
        }
    }

    if(batch && (option == Option::Twoway || option == Option::Ordered))
    {
        cerr << argv[0] << ": batch can only be set with oneway or datagram" << endl;
        return 1;
    }

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
            cerr << appName() << ": temporary failure. try again." << endl;
            return 1;
        }
    }

    auto adapter = communicator()->createObjectAdapter("Clock.Subscriber");

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
    auto subscriber = adapter->add(make_shared<ClockI>(), subId);

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
    if(option == Option::Datagram)
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
    else if(option == Option::Twoway)
    {
        // Do nothing to the subscriber proxy. Its already twoway.
    }
    else if(option == Option::Ordered)
    {
        // Do nothing to the subscriber proxy. Its already twoway.
        qos["reliability"] = "ordered";
    }
    else if(option == Option::Oneway || option == Option::None)
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

    shutdownOnInterrupt();
    communicator()->waitForShutdown();

    topic->unsubscribe(subscriber);

    return 0;
}
