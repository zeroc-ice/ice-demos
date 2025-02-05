// Copyright (c) ZeroC, Inc.

#pragma once

module Demo
{
    /// The interface implemented by the subscriber. It accepts time updates.
    interface Clock
    {
        /// Sends the current time to the clock.
        /// @param time The current time.
        void tick(string time);
    }
}
