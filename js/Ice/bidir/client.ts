// Copyright (c) ZeroC, Inc.

import { Ice } from "@zeroc/ice";
import { EarlyRiser } from "./AlarmClock.js";
import { MockAlarmClock } from "./MockAlarmClock.js";
import { toTicks } from "./timeUtil.js";
import process from "node:process";

// Create an Ice communicator. We'll use this communicator to create proxies, manage outgoing connections, and create
// an object adapter.
await using communicator = new Ice.Communicator(process.argv);

// Create an object adapter with no name and no configuration. This object adapter does not need to be activated.
const adapter = await communicator.createObjectAdapter("");

// Sets this object adapter as the default object adapter on the communicator.
communicator.setDefaultObjectAdapter(adapter);

// Register the MockAlarmClock servant with the adapter. The wake up service knows we use identity "alarmClock".
const mockAlarmClock = new MockAlarmClock();
adapter.add(mockAlarmClock, new Ice.Identity("alarmClock"));

// Create a proxy to the wake-up service.
const wakeUpService = new EarlyRiser.WakeUpServicePrx(communicator, "wakeUpService:tcp -h localhost -p 4061");

// Schedule a wake-up call in 5 seconds. This call establishes the connection to the server; incoming requests over this
// connection are handled by the communicator's default object adapter.
await wakeUpService.wakeMeUp(toTicks(Date.now() + 5000));
console.log("Wake-up call scheduled, falling asleep...");

// Wait until the "stop" button is pressed on the mock alarm clock.
await mockAlarmClock.StopPressed;
console.log("Stop button pressed, exiting...");
