// Copyright (c) ZeroC, Inc.

import { Ice } from "@zeroc/ice";
import { EarlyRiser } from "./AlarmClock.js";
import { MockAlarmClock } from "./MockAlarmClock.js";

// Create an Ice communicator to initialize the Ice runtime. The communicator is disposed before the program exits.
await using communicator = Ice.initialize(process.argv);

// Create an object adapter with no name and no configuration. This object adapter does not need to be activated.
const adapter = await communicator.createObjectAdapter("");

// Sets this object adapter as the default object adapter on the communicator.
communicator.setDefaultObjectAdapter(adapter);

// Register the MockAlarmClock servant with the adapter. The wake up service knows we use identity "alarmClock".
var mockAlarmClock = new MockAlarmClock();
adapter.add(mockAlarmClock, Ice.stringToIdentity("alarmClock"));

// Create a proxy to the wake-up service.
const wakeUpService = new EarlyRiser.WakeUpServicePrx(communicator, "wakeUpService:tcp -h localhost -p 4061");

// Schedule a wake-up call in 5 seconds. This call establishes the connection to the server; incoming requests over this
// connection are handled by the communicator's default object adapter.

await wakeUpService.wakeMeUp(toTicks(Date.now() + 5000));
console.log("Wake-up call scheduled, falling asleep...");

// Wait until the "stop" button is pressed on the mock alarm clock.
await mockAlarmClock.StopPressed;
console.log("Stop button pressed, exiting...");

const millisecondsBeforeEpoch = 719162n * 24n * 60n * 60n * 1000n; // daysBeforeEpoch converted to milliseconds
const ticksPerMillisecond = 10000n; // number of ticks per millisecond

// Convert a timestamp representing the number of milliseconds elapsed since the epoch to a BigInt representing
// the number of ticks (100 nanoseconds) since January 1, 0001 00:00:00 UTC in the Gregorian calendar.
function toTicks(timestamp: number) {
    return (BigInt(timestamp) + millisecondsBeforeEpoch) * ticksPerMillisecond;
}
