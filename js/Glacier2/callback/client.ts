// Copyright (c) ZeroC, Inc.

import { Ice, Glacier2 } from "@zeroc/ice";
import { EarlyRiser } from "./AlarmClock.js";
import { MockAlarmClock } from "./MockAlarmClock.js";
import { toTicks } from "../../common/timeUtil.js";
import process from "node:process";

// Retrieve my name
const name = process.env.USER || process.env.USERNAME || "masked user";

// Create an Ice communicator. We'll use this communicator to create proxies, manage outgoing connections, and create
// an object adapter.
await using communicator = Ice.initialize(process.argv);

// Create a proxy to the Glacier2 router. The addressing information (transport, host and port number) is derived from
// the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
const router = new Glacier2.RouterPrx(communicator, "Glacier2/router:tcp -h localhost -p 4063");

// Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
// username/password combination. This call establishes a network connection to the Glacier2 router; the lifetime of the
// session is the same as the lifetime of the connection.
const session = await router.createSession(name, "password");

// The proxy returned by createSession is null because we did not configure a SessionManager on the Glacier2 router.
console.assert(session === null);

// Obtain a category string from the router. We need to use this category for the identity of server->client callbacks
// invoked through the Glacier2 router.
const clientCategory = await router.getCategoryForClient();

// Create an object adapter with no name and no configuration, but with our router proxy. This object adapter is a
// "bidirectional" object adapter, like the one created by the Ice/bidir client application. It does not listen on any
// port and it does not need to be activated.
const adapter = await communicator.createObjectAdapterWithRouter("", router);

// Register the MockAlarmClock servant with the adapter. It uses the category retrieved from the router. You can verify
// the ring callback is never delivered if you provide a different category.
const mockAlarmClock = new MockAlarmClock();
const alarmClock = EarlyRiser.AlarmClockPrx.uncheckedCast(
    adapter.add(mockAlarmClock, new Ice.Identity("alarmClock", clientCategory)),
);

// Create a proxy to the wake-up service behind the Glacier2 router. Typically, the client cannot connect directly to
// this server because it's on an unreachable network.
let wakeUpService = new EarlyRiser.WakeUpServicePrx(communicator, "wakeUpService:tcp -h localhost -p 4061");

// Configure the proxy to route requests using the Glacier2 router.
wakeUpService = wakeUpService.ice_router(router);

// Schedule a wake-up call in 5 seconds. This call establishes the connection to the server; incoming requests over this
// connection are handled by the communicator's default object adapter.
await wakeUpService.wakeMeUp(alarmClock, toTicks(Date.now() + 5000));
console.log("Wake-up call scheduled, falling asleep...");

// Wait until the "stop" button is pressed on the mock alarm clock.
await mockAlarmClock.StopPressed;
console.log("Stop button pressed, exiting...");
