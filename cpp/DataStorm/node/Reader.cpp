// Copyright (c) ZeroC, Inc.

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include <iostream>

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
        initData.properties = make_shared<Ice::Properties>(argc, argv, defaultProperties);

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

        // Instantiate a reader to read the time from all the topic writers.
        auto reader = DataStorm::makeAnyKeyReader(topic);

        // Prints out the received samples.
        reader.onSamples(
            nullptr,
            [](const DataStorm::Sample<int, string>& sample)
            { cout << "[" << sample.getKey() << "] received time: " << sample.getValue() << endl; });

        // Exit once the user hits Ctrl-C to shutdown the node.
        node.waitForShutdown();
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }
    return 0;
}
