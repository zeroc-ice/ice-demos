// Copyright (c) ZeroC, Inc.

import { Ice } from "@zeroc/ice";
import { ClearSky } from "./WeatherStation1.js";
import process from "node:process";

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
await using communicator = new Ice.Communicator(process.argv);

// Create a proxy to the weather station.
const weatherStation = new ClearSky.WeatherStationPrx(communicator, "weatherStation:tcp -h localhost -p 4061");

// Create an AtmosphericConditions object with random values.
const reading = new ClearSky.AtmosphericConditions(
    Math.trunc(190 + Math.random() * (230 - 190)) / 10.0,
    Math.trunc(450 + Math.random() * (550 - 450)) / 10.0,
);

// Report this reading to the weather station.
await weatherStation.report("sensor v1", reading);

console.log("sensor v1: sent reading to weather station");
