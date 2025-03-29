# Copyright (c) ZeroC, Inc.

"""
Utility functions to convert between datetime objects and custom timestamps
based on 100-nanosecond ticks since 0001-01-01T00:00:00 UTC.
"""

from datetime import datetime, timezone

SECONDS_BEFORE_EPOCH = 62135596800
TICKS_PER_SECOND = 10_000_000

def toTimestamp(value: datetime) -> int:
    """
    Converts a datetime to a timestamp.

    Parameters
    ----------
    value : datetime
        The datetime object to convert. It must be timezone-aware and in UTC.

    Returns
    -------
    int
        The timestamp represented as the number of ticks since January 1, 0001 00:00:00 UTC.
        Each tick is 100 nanoseconds.
    """
    return int((value.timestamp() + SECONDS_BEFORE_EPOCH) * TICKS_PER_SECOND)

def toDatetime(timestamp: int) -> datetime:
    """
    Converts a timestamp to a datetime.

    Parameters
    ----------
    timestamp : int
        The timestamp represented as the number of ticks since January 1, 0001 00:00:00 UTC.
        Each tick is 100 nanoseconds.

    Returns
    -------
    datetime
        The datetime aware object using UTC timezone.
    """
    return datetime.fromtimestamp((timestamp / TICKS_PER_SECOND) - SECONDS_BEFORE_EPOCH, tz=timezone.utc)
