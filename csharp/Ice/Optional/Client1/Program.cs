// Copyright (c) ZeroC, Inc.

// Slice module ClearSky in WeatherStation1.ice maps to C# namespace ClearSky.
using ClearSky;

// Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create a proxy to the Weather station.
WeatherStationPrx weatherStation = WeatherStationPrxHelper.createProxy(communicator, "weatherStation:tcp -p 4061");

// Create an AtmosphericConditions object with random values.
var rand = new Random();
var reading = new AtmosphericConditions
{
    Temperature = rand.Next(190, 230) / 10.0,
    Humidity = rand.Next(450, 550) / 10.0,
};

// Report this reading to the weather station.
await weatherStation.ReportAsync("sensor v1", reading);

Console.WriteLine("sensor v1: sent reading to weather station");
