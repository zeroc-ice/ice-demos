// Copyright (c) ZeroC, Inc.

// Slice module ClearSky in WeatherStation1.ice maps to C# namespace ClearSky.
using ClearSky;
using System.Security.Cryptography; // for RandomNumberGenerator

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create a proxy to the weather station.
WeatherStationPrx weatherStation = WeatherStationPrxHelper.createProxy(
    communicator,
    "weatherStation:tcp -p 4061 -h localhost");

// Create an AtmosphericConditions object with random values.
var reading = new AtmosphericConditions
{
    Temperature = RandomNumberGenerator.GetInt32(190, 230) / 10.0,
    Humidity = RandomNumberGenerator.GetInt32(450, 550) / 10.0,
};

// Report this reading to the weather station.
await weatherStation.ReportAsync("sensor v1", reading);

Console.WriteLine("sensor v1: sent reading to weather station");
