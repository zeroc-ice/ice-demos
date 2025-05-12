// Copyright (c) ZeroC, Inc.

#pragma once

module ClearSky
{
    /// Represents the atmospheric conditions measured by a sensor.
    ["cpp:custom-print"]
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
        /// Reports a new reading to the weather station(s) via the IceStorm service.
        /// @param sensorId The unique identifier of the sensor that took the reading.
        /// @param timeStamp The time the reading was taken.
        /// @param reading The atmospheric conditions measured by the sensor.
        void report(string sensorId, string timeStamp, AtmosphericConditions reading);
    }
}
