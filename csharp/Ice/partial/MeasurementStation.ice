//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

module Demo
{
    //
    // A struct type representing distance measurements, the raw value stores
    // the distance in meters, C# public properties to access the value using
    // different units are provided in Meassumernts.cs using a partial struct
    // definition.
    //
    ["protected"] struct Distance
    {
        double value;
    }

    //
    // A class type representing speed measurements, the raw value stores
    // the speed in meters per second (mps), C# public properties to access
    // the value using different units are provided in Meassumernts.cs using
    // a partial class definition.
    //
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
