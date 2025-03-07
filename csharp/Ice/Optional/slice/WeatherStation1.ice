// Copyright (c) ZeroC, Inc.

#pragma once

// Version 1 of the WeatherStation Slice definitions.

module ClearSky
{
    /// Represents the atmospheric conditions measured by a sensor.
    class AtmosphericConditions
    {
        /// The temperature in degrees Celsius.
        ["cs:identifier:Temperature"]
        double temperature;

        /// The humidity in percent.
        ["cs:identifier:Humidity"]
        double humidity;
    }

    /// A weather station collects readings from sensors.
    interface WeatherStation
    {
        /// Reports a new reading to the weather station
        /// @param sensorId The unique identifier of the sensor that took the reading.
        /// @param reading The atmospheric conditions measured by the sensor.
        ["cs:identifier:Report"] // We prefer PascalCase for C# methods.
        void report(string sensorId, AtmosphericConditions reading);
    }
}
