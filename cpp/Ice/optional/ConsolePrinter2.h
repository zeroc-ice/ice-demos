// Copyright (c) ZeroC, Inc.

#ifndef CONSOLE_PRINTER2_H
#define CONSOLE_PRINTER2_H

#include "WeatherStation2.h"

#include <Ice/Ice.h>
#include <iostream>

namespace Server
{
    /// ConsolePrinter is an Ice servant that implements Slice interface ClearSky::WeatherStation. It prints all
    /// information reported by the sensors to the console.
    class ConsolePrinter : public ClearSky::WeatherStation
    {
    public:
        // Implements the pure virtual function in the base class (ClearSky::WeatherStation) generated by the Slice
        // compiler.
        void report(std::string sensorId, ClearSky::AtmosphericConditionsPtr reading, const Ice::Current&) override;
    };
}

#endif
