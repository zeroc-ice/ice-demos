// Copyright (c) ZeroC, Inc.

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include <iostream>
#include <string>

#include "TemperatureRange.h"

using namespace std;

void printSample(const DataStorm::Sample<string, float>& sample)
{
    cout << "sample: key=" << sample.getKey() << ", value=" << sample.getValue() << std::endl;
}

void printSamples(const vector<DataStorm::Sample<string, float>>& samples)
{
    for (const auto& sample : samples)
    {
        printSample(sample);
    }
}

int
main(int argc, char* argv[])
{
    // CtrlCHandler must be created before the communicator is created or any other threads are started.
    Ice::CtrlCHandler ctrlCHandler;

    // Instantiates DataStorm node.
    DataStorm::Node node{argc, argv};

    ctrlCHandler.setCallback(
        [&ctrlCHandler, &node](int)
        {
            std::cout << "Shutting down..." << std::endl;
            node.shutdown();
            // Reset the callback to nullptr to avoid calling it again.
            ctrlCHandler.setCallback(nullptr);
        });

    // Instantiates the "temperature" topic. The topic uses strings for the keys and float for the values.
    DataStorm::Topic<string, float> topic{node, "temperature"};

    // Create an any key reader with a sample filter that only receives temperatures outside of the given range.
    // The filter criteria TemperatureRange is defined in TemperatureRange.ice Slice file.
    auto reader = DataStorm::makeAnyKeyReader(
        topic,
        DataStorm::Filter<ClearSky::TemperatureRange>("not-in-range-filter", ClearSky::TemperatureRange{21.0f, 19.0f}),
        "temperature-reader");

    reader.onSamples(printSamples, printSample);

    node.waitForShutdown();
    return 0;
}
