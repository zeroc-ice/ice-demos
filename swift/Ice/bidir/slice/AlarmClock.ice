// Copyright (c) ZeroC, Inc.

#pragma once

module EarlyRiser
{
    /// Indicates the button pressed when the alarm rang.
    enum ButtonPressed
    {
        ["swift:identifier:snooze"]
        Snooze,

        ["swift:identifier:stop"]
        Stop
    }

    /// Represents a simple alarm clock. It's the "callback" in this demo, and it's implemented by the client.
    interface AlarmClock
    {
        /// Rings the alarm clock.
        /// @param message The message to display.
        /// @return The button pressed by the user.
        ButtonPressed ring(string message);
    }

    /// Represents the wake up service provided by the server.
    interface WakeUpService
    {
        /// Schedules a call to the caller's {@link AlarmClock::ring} at the specified time.
        /// @param timeStamp The time to ring the alarm clock. It's encoded as the number of ticks (100 nanoseconds)
        /// since January 1, 0001 00:00:00 UTC in the Gregorian calendar.
        void wakeMeUp(long timeStamp);
    }
}
