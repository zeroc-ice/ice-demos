// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifdef _MSC_VER
// For localtime warning
#  define _CRT_SECURE_NO_WARNINGS
#endif

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>
#include <Clock.h>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace Demo;

class Publisher : public Ice::Application
{
public:

    virtual int run(int, char*[]) override;
};

int
main(int argc, char* argv[])
{
    Publisher app;
    return app.main(argc, argv, "config.pub");
}

void
usage(const string& n)
{
    cerr << "Usage: " << n << " [--datagram|--twoway|--oneway] [topic]" << endl;
}

int
Publisher::run(int argc, char* argv[])
{
    enum class Option { None, Datagram, Twoway, Oneway };
    Option option = Option::None;
    string topicName = "time";
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
        else if(optionString.substr(0, 2) == "--")
        {
            usage(argv[0]);
            return EXIT_FAILURE;
        }
        else
        {
            topicName = argv[i++];
            break;
        }

        if(oldoption != option && oldoption != Option::None)
        {
            usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    if(i != argc)
    {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    auto manager = Ice::checkedCast<IceStorm::TopicManagerPrx>(
        communicator()->propertyToProxy("TopicManager.Proxy"));
    if(!manager)
    {
        cerr << appName() << ": invalid proxy" << endl;
        return EXIT_FAILURE;
    }

    //
    // Retrieve the topic.
    //
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
            return EXIT_FAILURE;
        }
    }

    //
    // Get the topic's publisher object, and create a Clock proxy with
    // the mode specified as an argument of this application.
    //
    auto publisher = topic->getPublisher();
    if(option == Option::Datagram)
    {
        publisher = publisher->ice_datagram();
    }
    else if(option == Option::Twoway)
    {
        // Do nothing.
    }
    else if(option == Option::Oneway || option == Option::None)
    {
        publisher = publisher->ice_oneway();
    }

    auto clock = Ice::uncheckedCast<ClockPrx>(publisher);

    cout << "publishing tick events. Press ^C to terminate the application." << endl;
    try
    {
        bool stop = false;
        while(!stop)
        {
            auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
            ostringstream os;

            os << put_time(localtime(&now), "%F %T");
            clock->tick(os.str());
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
    catch(const Ice::CommunicatorDestroyedException&)
    {
        // Ignore
    }

    return EXIT_SUCCESS;
}
