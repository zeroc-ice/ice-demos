// Copyright (c) ZeroC, Inc.

module ClearSky
{

    enum UpdateTag
    {
        TemperatureUpdated,
        HumidityUpdated
    }

    /// Represents the atmospheric conditions measured by a sensor.
    struct AtmosphericConditions
    {
        /// The temperature in degrees Celsius.
        float temperature;

        /// The humidity in percent.
        float humidity;
    }
}
