// Copyright (c) ZeroC, Inc.

// Slice module EarlyRiser in AlarmClock.ice maps to C# namespace EarlyRiser.
using EarlyRiser;

// Create an Ice communicator to initialize the Ice runtime.
using Ice.Communicator communicator = Ice.Util.initialize(ref args);

// Create an object adapter with no name and no configuration. This object adapter does not need to be activated.
Ice.ObjectAdapter adapter = communicator.createObjectAdapter("");

// Sets this object adapter as the default object adapter on the communicator.
communicator.setDefaultObjectAdapter(adapter);

// Register the MockAlarmClock servant with the adapter. The wake up service knows we use identity "alarmClock".
var mockAlarmClock = new Client.MockAlarmClock();
adapter.add(mockAlarmClock, Ice.Util.stringToIdentity("alarmClock"));

// Create a proxy to the wake-up service.
WakeUpServicePrx wakeUpService = WakeUpServicePrxHelper.createProxy(
    communicator,
    "wakeUpService:tcp -h localhost -p 4061");

// Schedule a wake-up call in 5 seconds. This call establishes the connection to the server; incoming requests over this
// connection are handled by the communicator's default object adapter.
await wakeUpService.WakeMeUpAsync(DateTime.Now.AddSeconds(5).ToUniversalTime().Ticks);
Console.WriteLine("Wake-up call scheduled, falling asleep...");

// Wait until the "stop" button is pressed on the mock alarm clock.
await mockAlarmClock.StopPressed;
Console.WriteLine("Stop button pressed, exiting...");
