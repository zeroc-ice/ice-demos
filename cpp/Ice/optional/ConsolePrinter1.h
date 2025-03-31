// Copyright (c) ZeroC, Inc.

#include "WeatherStation1.h"

#include <Ice/Ice.h>
#include <iostream>

namespace Server
{
    class ConsolePrinter : public ClearSky::WeatherStation
    {
    public:
        void report(std::string sensorId, ClearSky::AtmosphericConditionsPtr reading, const Ice::Current&) override
        {
            std::cout << sensorId << " reported " << reading << " to station v1" << std::endl;
        }
    };
}
