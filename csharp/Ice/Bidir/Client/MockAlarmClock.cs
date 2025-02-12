// Copyright (c) ZeroC, Inc.

using EarlyRiser;

namespace Client;

/// <summary>MockAlarmClock is an Ice servant that implements Slice interface AlarmClock.</summary>
internal class MockAlarmClock : AlarmClockDisp_
{
    /// <summary>Gets a task that completes when we return <see cref="ButtonPressed.Stop" /> from <see cref="Ring"/>.
    /// </summary>
    public Task StopPressed => _tcs.Task;

    // RunContinuationsAsynchronously because we don't want to continue Main in an Ice thread pool thread.
    private readonly TaskCompletionSource _tcs = new(TaskCreationOptions.RunContinuationsAsynchronously);

    private bool _needMoreTime = true;

    /// <inheritdoc/>
    // Implements the abstract method Ring from the AlarmClockDisp_ class generated by the Slice compiler.
    public override ButtonPressed Ring(string message, Ice.Current current)
    {
        Console.WriteLine($"Dispatching ring request {{ message = '{message}' }}");
        if (_needMoreTime)
        {
            Console.WriteLine($"Returning {ButtonPressed.Snooze} to request more time.");
            _needMoreTime = false; // we only snooze one time
            return ButtonPressed.Snooze;
        }
        else
        {
            _tcs.TrySetResult();
            Console.WriteLine($"Returning {ButtonPressed.Stop} to stop the alarm.");
            return ButtonPressed.Stop;
        }
    }
}
