// Copyright (c) ZeroC, Inc.

#include "../../common/Time.h"
#include "WeatherStation.h"

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>
#include <iostream>
#include <optional>
#include <random>
#include <thread>

using namespace ClearSky;
using namespace std;

int
main(int argc, char* argv[])
{
    // CtrlCHandler must be created before the communicator is created or any other threads are started.
    Ice::CtrlCHandler ctrlCHandler;

    // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create a proxy to the IceStorm topic manager.
    IceStorm::TopicManagerPrx topicManager{communicator, "ClearSky/TopicManager:tcp -p 4061 -h localhost"};

    // Retrieve a proxy to the "weather" topic: we first create a topic with the given name (in case we are the first),
    // and then retrieve the proxy if the topic was already created.
    std::optional<IceStorm::TopicPrx> topic;
    const std::string topicName = "weather";
    try
    {
        topic = topicManager->create(topicName);
    }
    catch (const IceStorm::TopicExists&)
    {
        topic = topicManager->retrieve(topicName);
    }

    // The proxy returned by create and retrieve is never null.
    assert(topic);

    // Create a WeatherStation proxy using the publisher proxy of the topic.
    std::optional<Ice::ObjectPrx> publisher = topic->getPublisher();
    // The publisher proxy is never null.
    assert(publisher);

    // Create a WeatherStation proxy from the publisher proxy.
    auto weatherStation = Ice::uncheckedCast<WeatherStationPrx>(*publisher);

    // The proxy returned by IceStorm is a two-way proxy. We can convert it into a oneway proxy if we don't need
    // acknowledgments from IceStorm.
    assert(weatherStation->ice_isTwoway());

    // Generate a sensor ID for this weather station.
    string uuid = Ice::generateUUID();
    string sensorId = "sensor-" + uuid.substr(uuid.length() - 4);

    cout << sensorId << " reporting. Press Ctrl+C to stop." << endl;

    // Shutdown the sensor on Ctrl-C.
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
    std::uniform_real_distribution<> tempDist{19.0, 23.0};     // Temperature range: 19.0 to 23.0
    std::uniform_real_distribution<> humidityDist{45.0, 55.0}; // Humidity range: 45.0 to 55.0

    while (true)
    {
        // Generate random temperature and humidity values.
        double randomTemperature = tempDist(gen);
        double randomHumidity = humidityDist(gen);

        auto now = std::chrono::system_clock::now();
        const string timeStamp = Time::formatTime(now);

        // Create an AtmosphericConditions object with random values.
        auto reading = std::make_shared<AtmosphericConditions>(randomTemperature, randomHumidity);

        // Report this reading to the weather station.
        weatherStation->report(sensorId, timeStamp, reading);

        // Wait for second or for the shutdown signal.
        if (shutdownFuture.wait_for(std::chrono::seconds(1)) == std::future_status::timeout)
        {
            break;
        }
    }

    return 0;
}
