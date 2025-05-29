// Copyright (c) ZeroC, Inc.

import Foundation

// Helper methods to convert between dates and time stamps.
extension Date {
    // January 1, 0001 00:00:00 UTC as a Date.
    private static let origin = Date(timeIntervalSince1970: -719162.0 * 24 * 3600)
    private static let ticksPerSecond: Double = 10_000_000

    /// Returns the number of 100-nanosecond intervals since January 1, 0001 00:00:00 UTC.
    public var timeStamp: Int64 {
        Int64(self.timeIntervalSince(Self.origin) * Self.ticksPerSecond)
    }

    /// Initializes a Date from the number of 100-nanosecond intervals since January 1, 0001 00:00:00 UTC.
    /// - Parameter timeStamp: The time stamp.
    public init(timeStamp: Int64) {
        self = Self.origin.addingTimeInterval(TimeInterval(timeStamp) / Self.ticksPerSecond)
    }
}
