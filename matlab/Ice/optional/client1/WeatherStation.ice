// Copyright (c) ZeroC, Inc.

#pragma once

// Version 1 of the WeatherStation Slice definitions.

["matlab:identifier:clearsky"]
module ClearSky
{
    /// Represents the atmospheric conditions measured by a sensor.
    class AtmosphericConditions
    {
        /// The temperature in degrees Celsius.
        double temperature;

        /// The humidity in percent.
        double humidity;
    }

    /// A weather station collects readings from sensors.
    interface WeatherStation
    {
        /// Reports a new reading to the weather station.
        /// @param sensorId The unique identifier of the sensor that took the reading.
        /// @param reading The atmospheric conditions measured by the sensor.
        void report(string sensorId, AtmosphericConditions reading);
    }
}
