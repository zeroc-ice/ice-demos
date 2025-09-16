// Copyright (c) ZeroC, Inc.

package com.example.ice.bidir.util;

public class Time {
    // daysBeforeEpoch converted to milliseconds
    private static final long MILLIS_BEFORE_EPOCH = 719162 * 24 * 60 * 60 * 1000;

    // number of ticks per millisecond
    private static final long TICKS_PER_MILLISECOND = 10_000;

    /**
     *  Converts a zoned date time to a time stamp.
     *
     * @param dateTime The date time.
     * @return The time stamp.
     */
    public static long toTimeStamp(java.time.ZonedDateTime dateTime) {
        return dateTime.toInstant().toEpochMilli() * TICKS_PER_MILLISECOND + MILLIS_BEFORE_EPOCH;
    }

    /**
     * Converts a time stamp to a UTC zoned date time.
     *
     * @param timeStamp The time stamp.
     * @return The zoned date time.
     */
    public static java.time.ZonedDateTime toZonedDateTime(long timeStamp) {
        long epochMilli = (timeStamp - MILLIS_BEFORE_EPOCH) / TICKS_PER_MILLISECOND;
        return java.time.Instant.ofEpochMilli(epochMilli).atZone(java.time.ZoneOffset.UTC);
    }

    private Time() {
    }
}
