// Copyright (c) ZeroC, Inc.

import { Ice, IceStorm } from "@zeroc/ice";
import { ClearSky } from "./WeatherStation.js";
import process from "node:process";

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
await using communicator = new Ice.Communicator(process.argv);

// Create a proxy to the IceStorm topic manager.
const topicManager = new IceStorm.TopicManagerPrx(communicator, "ClearSky/TopicManager:tcp -p 4061 -h localhost");

// Ask the topic manager to create or retrieve the "weather" topic and return the corresponding proxy.
const topic = await topicManager.createOrRetrieve("weather");

// The proxy returned by createOrRetrieve is never null.
console.assert(topic !== null);

// Create a WeatherStation proxy using the publisher proxy of the topic.
const publisher = await topic!.getPublisher();
console.assert(publisher != null); // The proxy returned by getPublisher is never null.
const weatherStation = ClearSky.WeatherStationPrx.uncheckedCast(publisher!);

// The proxy returned by IceStorm is a two-way proxy. We can convert it into a oneway proxy if we don't need
// acknowledgments from IceStorm.
console.assert(weatherStation.ice_isTwoway());

// Generate a sensor ID for this weather sensor.
const sensorId = `sensor-${Math.trunc(Math.random() * 9999)
    .toString()
    .padStart(4, "0")}`;

// Send a reading every second to the weather station(s) via IceStorm. We keep sending until the user presses Ctrl+C.
console.log(`${sensorId} reporting. Press Ctrl+C to stop...`);

async function delay(ms: number): Promise<void> {
    return new Promise((resolve) => setTimeout(resolve, ms));
}

while (true) {
    // Create a new AtmosphericConditions object with random values.
    const reading = new ClearSky.AtmosphericConditions(
        Math.trunc(190 + Math.random() * (230 - 190)) / 10.0,
        Math.trunc(450 + Math.random() * (550 - 450)) / 10.0,
    );

    // Send the reading to the weather station(s).
    const timeStamp = new Date().toTimeString();
    await weatherStation.report(sensorId, timeStamp, reading);

    // Wait for one second before sending the next reading.
    await delay(1000);
}
