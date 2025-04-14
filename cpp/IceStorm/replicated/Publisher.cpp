// Copyright (c) ZeroC, Inc.

#include "../../common/Time.h"
#include "Clock.h"
#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

using namespace std;
using namespace Demo;

int run(const shared_ptr<Ice::Communicator>& communicator, int argc, char* argv[]);

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
        // and its dtor destroys this communicator.
        //
        const Ice::CommunicatorHolder ich(argc, argv, "config.pub");
        const auto& communicator = ich.communicator();

        ctrlCHandler.setCallback([communicator](int) { communicator->destroy(); });

        status = run(communicator, argc, argv);
    }
    catch (const std::exception& ex)
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
run(const shared_ptr<Ice::Communicator>& communicator, int argc, char* argv[])
{
    enum class Option : uint8_t
    {
        None,
        Datagram,
        Twoway,
        Oneway
    };
    Option option = Option::None;
    string topicName = "time";
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

    auto manager = Ice::checkedCast<IceStorm::TopicManagerPrx>(communicator->propertyToProxy("TopicManager.Proxy"));
    if (!manager)
    {
        cerr << argv[0] << ": invalid proxy" << endl;
        return 1;
    }

    //
    // Retrieve the topic.
    //
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

    //
    // Get the topic's publisher object, and create a Clock proxy with
    // the mode specified as an argument of this application.
    //
    auto publisher = topic->getPublisher();
    if (option == Option::Datagram)
    {
        publisher = publisher->ice_datagram();
    }
    else if (option == Option::Twoway)
    {
        // Do nothing.
    }
    else if (option == Option::Oneway || option == Option::None)
    {
        publisher = publisher->ice_oneway();
    }

    auto clock = Ice::uncheckedCast<ClockPrx>(publisher);

    cout << "publishing tick events. Press ^C to terminate the application." << endl;
    while (true)
    {
        try
        {
            auto timeString = Time::formatTime(chrono::system_clock::now());
            clock->tick(timeString);
            this_thread::sleep_for(chrono::seconds(1));
        }
        catch (const Ice::CommunicatorDestroyedException&)
        {
            break;
        }
    }

    return 0;
}
