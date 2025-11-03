// Copyright (c) ZeroC, Inc.

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include <iostream>
#include <string>

#include "AtmosphericConditions.h"

using namespace std;

void
printSample(const DataStorm::Sample<string, ClearSky::AtmosphericConditions, ClearSky::UpdateTag>& sample)
{
    cout << "sample: event=" << sample.getEvent() << " key=" << sample.getKey() << ", value=" << sample.getValue()
         << std::endl;
}

void
printSamples(const vector<DataStorm::Sample<string, ClearSky::AtmosphericConditions, ClearSky::UpdateTag>>& samples)
{
    for (const auto& sample : samples)
    {
        printSample(sample);
    }
}

int
main(int argc, char* argv[])
{
    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
    // of the program, before creating a DataStorm node or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Instantiates DataStorm node.
    DataStorm::Node node{argc, argv};

    // Set Ctrl+C handler to shutdown the node.
    ctrlCHandler.setCallback(
        [&ctrlCHandler, &node](int)
        {
            std::cout << "Shutting down..." << std::endl;
            node.shutdown();
        });

    // Instantiates the "atmospheric-conditions" topic.
    // The topic uses strings for keys and AtmosphericConditions for values.
    DataStorm::Topic<string, ClearSky::AtmosphericConditions, ClearSky::UpdateTag> topic{
        node,
        "atmospheric-conditions"};

    // A partial updater for the temperature field.
    topic.setUpdater<float>(
        ClearSky::UpdateTag::TemperatureUpdated,
        [](ClearSky::AtmosphericConditions& t, float temperature) { t.temperature = temperature; });

    // A partial updater for the humidity field.
    topic.setUpdater<float>(
        ClearSky::UpdateTag::HumidityUpdated,
        [](ClearSky::AtmosphericConditions& t, float humidity) { t.humidity = humidity; });

    // Create an any key reader, that read all the samples published on the topic.
    auto reader = DataStorm::makeAnyKeyReader(topic, "atmospheric-conditions-reader");

    reader.onSamples(printSamples, printSample);

    node.waitForShutdown();
    return 0;
}
