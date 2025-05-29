// Copyright (c) ZeroC, Inc.

import Foundation

// Helper methods to convert between dates and time stamps.
// A time stamp is the number of 100-nanosecond intervals (ticks) since January 1, 0001 00:00:00 UTC.
extension Date {
    // January 1, 0001 00:00:00 UTC as a Date.
    private static let yearOne = Date(timeIntervalSince1970: -daysBetweenYearOneAnd1970 * 24 * 3600)

    private static let daysBetweenYearOneAnd1970: Double = 719162.0

    // A tick is 100 nanoseconds, and there are 10 million ticks in a second.
    private static let ticksPerSecond: Double = 10_000_000

    /// Converts this date into a time stamp - the number of ticks since January 1, 0001 00:00:00 UTC.
    public var timeStamp: Int64 {
        Int64(self.timeIntervalSince(Self.yearOne) * Self.ticksPerSecond)
    }

    /// Initializes a Date from the number ticks since January 1, 0001 00:00:00 UTC.
    /// - Parameter timeStamp: The time stamp.
    public init(timeStamp: Int64) {
        self = Self.yearOne.addingTimeInterval(TimeInterval(timeStamp) / Self.ticksPerSecond)
    }
}
