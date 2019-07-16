//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

module Demo
{
    const double YardConversionFactor = 1.0936;
    ["protected"] struct Distance
    {
        double value;
    }

    const double MilesPerHourConversionFactor = 2.2369;
    const double KilometresPerHourConversionFactor = 3.6;

    ["protected"] class Speed
    {
        double value;
    }

    interface MeasurementStation
    {
        Distance getDistance();
        Speed getSpeed();

        void shutdown();
    }
}
