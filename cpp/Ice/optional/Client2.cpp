// Copyright (c) ZeroC, Inc.

#include "WeatherStation2.h"

#include <Ice/Ice.h>
#include <iostream>
#include <random>

using namespace ClearSky;

int
main(int argc, char* argv[])
{
    // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create a proxy to the Weather station.
    WeatherStationPrx weatherStation{communicator, "weatherStation:tcp -p 4061"};

    // Initialize random number generators.
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<> tempDist{19.0, 23.0};         // Temperature range: 19.0 to 23.0
    std::uniform_real_distribution<> humidityDist{45.0, 55.0};     // Humidity range: 45.0 to 55.0
    std::uniform_real_distribution<> pressureDist{1000.0, 1050.0}; // Pressure range: 1000 to 1050

    // Generate random temperature and humidity values.
    double randomTemperature = tempDist(gen);
    double randomHumidity = humidityDist(gen);
    double randomPressure = pressureDist(gen);

    // Create an AtmosphericConditions object with random values.
    auto reading = std::make_shared<AtmosphericConditions>(randomTemperature, randomHumidity, randomPressure);

    // Report this reading to the weather station.
    weatherStation->report("sensor v2", reading);

    std::cout << "sensor v2: sent reading to weather station" << std::endl;

    return 0;
}
