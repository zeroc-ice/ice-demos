// Copyright (c) ZeroC, Inc.

#include "WeatherStation1.h"

#include <Ice/Ice.h>
#include <iostream>
#include <random>

using namespace ClearSky;

int
main(int argc, char* argv[])
{
    // Create an Ice communicator to initialize the Ice runtime.
    Ice::CommunicatorPtr communicator = Ice::initialize(argc, argv);

    // Make sure the communicator is destroyed at the end of this scope.
    Ice::CommunicatorHolder communicatorHolder{communicator};

    // Create a proxy to the Weather station.
    WeatherStationPrx weatherStation{communicator, "weatherStation:tcp -p 4061"};

    // Initialize random number generators.
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<> tempDist{19.0, 23.0};     // Temperature range: 19.0 to 23.0
    std::uniform_real_distribution<> humidityDist{45.0, 55.0}; // Humidity range: 45.0 to 55.0

    // Generate random temperature and humidity values.
    double randomTemperature = tempDist(gen);
    double randomHumidity = humidityDist(gen);

    // Create an AtmosphericConditions object with random values.
    AtmosphericConditions reading{randomTemperature, randomHumidity};

    // Report this reading to the weather station.
    weatherStation->report("sensor v1", std::make_shared<AtmosphericConditions>(reading));

    std::cout << "sensor v1: sent reading to weather station" << std::endl;

    return 0;
}
