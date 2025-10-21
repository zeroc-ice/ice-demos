// Copyright (c) ZeroC, Inc.

#include "../../common/Time.h"
#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>
#include <iostream>
#include <random>
#include <string>
#include <thread>

using namespace std;

int
main(int argc, char* argv[])
{
    try
    {
        // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the
        // beginning of the program, before creating a DataStorm node or starting any thread.
        Ice::CtrlCHandler ctrlCHandler;

        // Default properties.
        auto defaultProperties = make_shared<Ice::Properties>();
        // Connect to the DataStorm node running on localhost port 10000
        defaultProperties->setProperty("DataStorm.Node.ConnectTo", "tcp -h localhost -p 10000");
        // Disable the multicast endpoint
        defaultProperties->setProperty("DataStorm.Node.Multicast.Enabled", "0");

        Ice::InitializationData initData;
        initData.properties = Ice::createProperties(argc, argv, defaultProperties);

        // Create an Ice communicator. We'll use this communicator to instantiate the DataStorm node.
        Ice::CommunicatorPtr communicator = Ice::initialize(initData);

        // Make sure the communicator is destroyed at the end of this scope.
        Ice::CommunicatorHolder communicatorHolder{communicator};

        // Instantiates node.
        DataStorm::Node node{communicator};

        // Shutdown the node on Ctrl-C.
        ctrlCHandler.setCallback([&node](int) { node.shutdown(); });

        // Instantiates the "time" topic.
        DataStorm::Topic<int, string> topic{node, "time"};

        // Setup a random generator to generate an identifier for the writer.
        random_device rd;
        mt19937 generator(rd());
        uniform_int_distribution<> id(1);

        // Instantiate a writer to write the local time. The writer key is a random
        // integer value that identifies the writer.
        auto writer = DataStorm::makeSingleKeyWriter(topic, id(generator));

        while (!node.isShutdown())
        {
            auto timeString = Time::formatTime(chrono::system_clock::now());
            writer.update(timeString);
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
