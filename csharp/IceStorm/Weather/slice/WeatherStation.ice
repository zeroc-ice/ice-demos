// Copyright (c) ZeroC, Inc.

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
        /// Reports a new reading to the weather station(s) via the IceStorm service.
        /// @param sensorId The unique identifier of the sensor that took the reading.
        /// @param timeStamp The time the reading was taken.
        /// @param reading The atmospheric conditions measured by the sensor.
        ["cs:identifier:Report"] // We prefer PascalCase for C# methods.
        void report(string sensorId, string timeStamp, AtmosphericConditions reading);
    }
}
