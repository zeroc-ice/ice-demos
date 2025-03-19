// Copyright (c) ZeroC, Inc.

// Slice module EarlyRiser in AlarmClock.ice maps to C# namespace EarlyRiser.
using EarlyRiser;

using System.Diagnostics;

// Create an Ice communicator to initialize the Ice runtime.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create a proxy to the Glacier2 router. The addressing information (transport, host and port number) is derived from
// the value of Glacier2.Client.Endpoints in the glacier2 router configuration file.
Glacier2.RouterPrx router =
    Glacier2.RouterPrxHelper.createProxy(communicator, "Glacier2/router:tcp -h localhost -p 4063");

// Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
// username/password combination. This call establishes a network connection to the Glacier2 router; the lifetime of the
// session is the same as the lifetime of the connection.
Glacier2.SessionPrx? session = await router.createSessionAsync(Environment.UserName, "password");

// The proxy returned by createSession is null because we did not configure a SessionManager on the Glacier2 router.
Debug.Assert(session is null);

// Obtain a category string from the router. We need to use this category for the identity of server->client callbacks
// invoked through the Glacier2 router.
string clientCategory = await router.getCategoryForClientAsync();

// Create an object adapter with no name and no configuration, but with our router proxy. This object adapter is a
// "bidirectional" object adapter, like the one created by the Ice/Bidir client application. It does not listen on any
// port and it does not need to be activated.
Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithRouter("", router);

// Register the MockAlarmClock servant with the adapter. It uses the category retrieved from the router. You can verify
// the Ring callback is never delivered if you provide a different category.
var mockAlarmClock = new Client.MockAlarmClock();
AlarmClockPrx alarmClock = AlarmClockPrxHelper.uncheckedCast(
    adapter.add(mockAlarmClock, new Ice.Identity { name = "alarmClock", category = clientCategory }));

// Create a proxy to the wake-up service behind the Glacier2 router. Typically, the client cannot connect directly to
// this server because it's on an unreachable network.
WakeUpServicePrx wakeUpService = WakeUpServicePrxHelper.createProxy(
    communicator,
    "wakeUpService:tcp -h localhost -p 4061");

// Configure the proxy to route requests using the Glacier2 router.
wakeUpService = WakeUpServicePrxHelper.uncheckedCast(wakeUpService.ice_router(router));

// Schedule a wake-up call in 5 seconds. This call establishes the connection to the server; incoming requests over this
// connection are handled by the adapter.
await wakeUpService.WakeMeUpAsync(alarmClock, DateTime.Now.AddSeconds(5).ToUniversalTime().Ticks);
Console.WriteLine("Wake-up call scheduled, falling asleep...");

// Wait until the "stop" button is pressed on the mock alarm clock.
await mockAlarmClock.StopPressed;
Console.WriteLine("Stop button pressed, exiting...");
