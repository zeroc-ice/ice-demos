// Copyright (c) ZeroC, Inc.

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include "AtmosphericConditions.h"

#include <iostream>
#include <random>
#include <string>

using namespace std;

int
main(int argc, char* argv[])
{
    // CtrlCHandler must be created before the node is created or any other threads are started.
    Ice::CtrlCHandler ctrlCHandler;

    // Instantiates DataStorm node.
    DataStorm::Node node{argc, argv};

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

    // Create a single key writer that writes data for the "floor1/studio" key.
    auto writer = DataStorm::makeSingleKeyWriter(topic, "floor1/studio", "atmospheric-conditions-writer");

    // Wait for a reader to connect
    topic.waitForReaders();

    // Stop writing samples on Ctrl-C.
    auto shutdownPromise = std::promise<void>();
    auto shutdownFuture = shutdownPromise.get_future();
    ctrlCHandler.setCallback(
        [&ctrlCHandler, &shutdownPromise](int)
        {
            std::cout << "Shutting down..." << std::endl;
            shutdownPromise.set_value();
            // Reset the callback to nullptr to avoid calling it again.
            ctrlCHandler.setCallback(nullptr);
        });

    // Initialize random number generators.
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> atmosphericDist{0, 10};

    ClearSky::AtmosphericConditions conditions{21.5f, 50.0f};
    writer.update(conditions);

    // Publish AtmosphericConditions readings.
    float maxTemp = 23.0f;
    float minTemp = 19.0f;
    float maxHumidity = 55.0f;
    float minHumidity = 45.0f;

    bool humidityIncreasing = true;
    bool tempIncreasing = true;

    while (true)
    {
        // Generate random changes where either temperature or humidity changes about 50% of the time.
        bool tempChanged = atmosphericDist(gen) > 5;
        bool humidityChanged = atmosphericDist(gen) > 5;

        if (tempChanged)
        {
            if (tempIncreasing)
            {
                conditions.temperature += 0.5f;
            }
            else
            {
                conditions.temperature -= 0.5f;
            }

            tempIncreasing = (tempIncreasing && conditions.temperature < maxTemp) ||
                             (!tempIncreasing && conditions.temperature <= minTemp);
        }

        if (humidityChanged)
        {
            if (humidityIncreasing)
            {
                conditions.humidity += 1.0f;
            }
            else
            {
                conditions.humidity -= 1.0f;
            }

            humidityIncreasing = (humidityIncreasing && conditions.humidity < maxHumidity) ||
                                 (!humidityIncreasing && conditions.humidity <= minHumidity);
        }

        // If both temperature and humidity changed, publish a full update. Otherwise, publish a partial update for the
        // changed field.
        if (tempChanged && humidityChanged)
        {
            cout << "Publishing full update... " << conditions << endl;
            writer.update(conditions);
        }
        else if (humidityChanged)
        {
            cout << "Publishing humidity update... " << conditions.humidity << "%" << endl;
            writer.partialUpdate<float>(ClearSky::UpdateTag::HumidityUpdated)(conditions.humidity);
        }
        else if (tempChanged)
        {
            cout << "Publishing temperature update... " << conditions.temperature << "°C" << endl;
            writer.partialUpdate<float>(ClearSky::UpdateTag::TemperatureUpdated)(conditions.temperature);
        }

        // Wait for half a second or for the shutdown signal.
        if (shutdownFuture.wait_for(std::chrono::milliseconds(500)) != std::future_status::timeout)
        {
            break;
        }
    }
}
