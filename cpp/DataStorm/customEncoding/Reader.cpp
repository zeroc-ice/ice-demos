// Copyright (c) ZeroC, Inc.

#include "../../common/Terminate.h"
#include "../../common/Time.h"

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include <chrono>
#include <iostream>

#include "TimePoint.h"

using namespace std;

int
main(int argc, char* argv[])
{
    // Set a custom terminate handler to print unhandled exceptions to cerr.
    std::set_terminate(Terminate::printCurrentException);

    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the
    // beginning of the program, before creating a DataStorm node or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Instantiates node.
    DataStorm::Node node{argc, argv};

    // Shutdown the node on Ctrl-C.
    ctrlCHandler.setCallback([&node](int) { node.shutdown(); });

    // Instantiates the "time" topic.
    DataStorm::Topic<string, chrono::system_clock::time_point> topic{node, "time"};

    // Instantiate a reader to read the time from all the topic cities.
    auto reader = DataStorm::makeAnyKeyReader(topic);

    // Wait for at least one writer to connect.
    reader.waitForWriters();

    // Prints out the received samples.
    reader.onSamples(
        nullptr,
        [](const DataStorm::Sample<string, chrono::system_clock::time_point>& sample)
        {
            auto timeString = Time::formatTime(sample.getValue());
            cout << "received time for `" << sample.getKey() << "': " << timeString << endl;
        });

    // Exit once the user hits Ctrl-C to shutdown the node.
    node.waitForShutdown();
    return 0;
}
