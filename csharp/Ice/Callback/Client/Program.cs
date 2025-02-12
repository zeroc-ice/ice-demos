// Copyright (c) ZeroC, Inc.

// Slice module EarlyRiser in AlarmClock.ice maps to C# namespace EarlyRiser.
using EarlyRiser;

// Create an Ice communicator to initialize the Ice runtime.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create an object adapter that listens for incoming requests and dispatches them to servants.
// Since we don't specify a port, the OS will choose an ephemeral port. This allows multiple client applications to
// run concurrently on the same host.
Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints("AlarmClockAdapter", "tcp");

// Register the MockAlarmClock servant with the adapter, and get an alarm clock proxy.
var mockAlarmClock = new Client.MockAlarmClock();
AlarmClockPrx alarmClock = AlarmClockPrxHelper.uncheckedCast(
    adapter.add(mockAlarmClock, Ice.Util.stringToIdentity("alarmClock")));

// Start dispatching requests.
adapter.activate();
Console.WriteLine("Listening on ephemeral port...");

// Create a proxy to the wake-up service.
WakeUpServicePrx wakeUpService = WakeUpServicePrxHelper.createProxy(
    communicator,
    "wakeUpService:tcp -h localhost -p 4061");

// Schedule a wake-up call in 5 seconds.
await wakeUpService.WakeMeUpAsync(alarmClock, DateTime.Now.AddSeconds(5).ToUniversalTime().Ticks);
Console.WriteLine("Wake-up call scheduled, falling asleep...");

// Wait until the "stop" button is pressed on the mock alarm clock.
await mockAlarmClock.StopPressed;
Console.WriteLine("Stop button pressed, exiting...");
