// Copyright (c) ZeroC, Inc.

#ifndef CONSOLE_PRINTER1_H
#define CONSOLE_PRINTER1_H

#include "WeatherStation1.h"

#include <Ice/Ice.h>
#include <iostream>

namespace Server
{
    /// ConsolePrinter is an Ice servant that implements Slice interface ClearSky::WeatherStation. It prints all
    /// information reported by the sensors to the console.
    class ConsolePrinter : public ClearSky::WeatherStation
    {
    public:
        void report(std::string sensorId, ClearSky::AtmosphericConditionsPtr reading, const Ice::Current&) override
        {
            std::cout << sensorId << " reported " << reading << " to station v1" << std::endl;
        }
    };
}

#endif
