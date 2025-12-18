// Copyright (c) ZeroC, Inc.

#include "../../common/Terminate.h"

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include "TemperatureRange.h"

#include <iostream>
#include <string>

using namespace std;

int
main(int argc, char* argv[])
{
    // Set a custom terminate handler to print unhandled exceptions to cerr.
    std::set_terminate(Terminate::printCurrentException);

    // CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
    // of the program, before creating a DataStorm node or starting any thread.
    Ice::CtrlCHandler ctrlCHandler;

    // Instantiates DataStorm node.
    DataStorm::Node node{argc, argv};

    // Instantiates the "temperature" topic. The topic uses strings for keys and float for values.
    DataStorm::Topic<string, float> topic{node, "temperature"};

    // Create a sample filter that only reports temperatures outside of the given temperature range.
    // The filter criteria `TemperatureRange` is generated from the TemperatureRange.ice Slice file.
    topic.setSampleFilter<ClearSky::TemperatureRange>(
        "not-in-range-filter",
        [](ClearSky::TemperatureRange range)
        {
            return [range](const DataStorm::Sample<string, float>& sample)
            {
                float value = sample.getValue();
                return value < range.min || value > range.max;
            };
        });

    // Instantiate an any key writer.
    auto writer = DataStorm::makeAnyKeyWriter(topic, "temperature-writer");

    // Wait for a reader to connect
    topic.waitForReaders();

    // Shutdown the writer on Ctrl-C.
    auto shutdownPromise = std::promise<void>();
    auto shutdownFuture = shutdownPromise.get_future();
    ctrlCHandler.setCallback(
        [&shutdownPromise](int)
        {
            std::cout << "Shutting down..." << std::endl;
            shutdownPromise.set_value();
        });

    // Publish temperature readings.
    float temp = 20.0f;
    bool increasing = true;
    while (true)
    {
        cout << "Publishing temperature reading... " << temp << "°C" << endl;
        writer.update("room1", temp);

        if (increasing)
        {
            temp += 0.5f;
            if (temp >= 22.0f)
            {
                increasing = false;
            }
        }
        else
        {
            temp -= 0.5f;
            if (temp <= 18.0f)
            {
                increasing = true;
            }
        }

        // Wait for one second or for the shutdown signal.
        if (shutdownFuture.wait_for(std::chrono::seconds(1)) != std::future_status::timeout)
        {
            break;
        }
    }
}
