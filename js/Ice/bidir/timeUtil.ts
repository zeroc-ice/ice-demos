const millisecondsBeforeEpoch = 719162n * 24n * 60n * 60n * 1000n; // daysBeforeEpoch converted to milliseconds
const ticksPerMillisecond = 10000n; // number of ticks per millisecond

// Convert a timestamp representing the number of milliseconds elapsed since the epoch to a BigInt representing
// the number of ticks (100 nanoseconds) since January 1, 0001 00:00:00 UTC in the Gregorian calendar.
export function toTicks(timestamp: number) {
    return (BigInt(timestamp) + millisecondsBeforeEpoch) * ticksPerMillisecond;
}
