// Copyright (c) ZeroC, Inc.

#pragma once

module EarlyRiser
{
    /// Indicates the button pressed when the alarm rang.
    enum ButtonPressed { Snooze, Stop }

    /// Represents a simple alarm clock. It's the "callback" in this demo, and it's implemented by the client.
    interface AlarmClock
    {
        /// Rings the alarm clock.
        /// @param message The message to display.
        /// @return The button pressed by the user.
        ["cs:identifier:Ring"] // We prefer PascalCase for C# methods.
        ButtonPressed ring(string message);
    }

    /// Represents the wake up service provided by the server.
    interface WakeUpService
    {
        /// Schedules a call to {@link AlarmClock::ring} at the specified time.
        /// @param alarmClock A proxy to the alarm clock to ring.
        /// @param timeStamp The time to ring the alarm clock. It's encoded as the number of ticks (100 nanoseconds)
        /// since January 1, 0001 00:00:00 UTC in the Gregorian calendar.
        ["cs:identifier:WakeMeUp"] // We prefer PascalCase for C# methods.
        void wakeMeUp(AlarmClock* alarmClock, long timeStamp);
    }
}
