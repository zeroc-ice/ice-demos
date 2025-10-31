// Copyright (c) ZeroC, Inc.

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include <iostream>
#include <string>
#include <thread>

#include "TimePoint.h"

using namespace std;

int
main(int argc, char* argv[])
{
    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the
    // beginning of the program, before creating a DataStorm node or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Instantiates node.
    DataStorm::Node node{argc, argv};

    // Shutdown the node on Ctrl-C.
    ctrlCHandler.setCallback([&node](int) { node.shutdown(); });

    // Asks for city name to publish updates
    string city;
    cout << "Please enter city name: ";
    getline(cin, city);

    // Instantiates the "time" topic.
    DataStorm::Topic<string, chrono::system_clock::time_point> topic{node, "time"};

    // Instantiate a writer to write the time from the given city.
    auto writer = DataStorm::makeSingleKeyWriter(topic, city);

    while (!node.isShutdown())
    {
        writer.update(chrono::system_clock::now());
        this_thread::sleep_for(chrono::seconds(1));
    }
    return 0;
}
