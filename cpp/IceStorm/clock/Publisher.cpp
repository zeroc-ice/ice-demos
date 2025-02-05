// Copyright (c) ZeroC, Inc.

#include "Clock.h"

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

#include <chrono>
#include <ctime>
#include <future>
#include <iostream>

using namespace std;
using namespace Demo;

void
usage(const char* name)
{
    cerr << "Usage: " << name << " [--datagram|--twoway|--oneway] [topic]" << endl;
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
    const Ice::CommunicatorHolder communicatorHolder{argc, argv, "config.pub"};
    const Ice::CommunicatorPtr& communicator = communicatorHolder.communicator();

    // Parse command-line options.

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

    // Get the topic's publisher object, and create a Clock proxy with the mode specified as an argument of this
    // application.
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

    // Downcast the proxy to the Clock type using uncheckedCast.
    auto clock = Ice::uncheckedCast<ClockPrx>(publisher);

    cout << "publishing tick events. Press ^C to terminate the application." << endl;

    // Send a tick every second until cancelled in a background task.
    auto future = std::async(
        std::launch::async,
        [clock = std::move(clock)]()
        {
            while (true)
            {
                try
                {
                    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
                    char timeString[100];
                    if (strftime(timeString, sizeof(timeString), "%x %X", localtime(&now)) == 0)
                    {
                        timeString[0] = '\0';
                    }
                    clock->tick(timeString);
                    this_thread::sleep_for(chrono::seconds(1));
                }
                catch (const Ice::CommunicatorDestroyedException&)
                {
                    break; // done
                }
            }
        });

    // Wait until the user presses Ctrl+C.
    ctrlCHandler.wait();

    // Destroy the communicator; the next call to tick will throw CommunicatorDestroyedException.
    communicator->destroy();

    // Wait for the background task to complete.
    future.wait();

    return 0;
}
