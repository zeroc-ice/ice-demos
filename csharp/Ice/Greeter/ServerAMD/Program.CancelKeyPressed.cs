// Copyright (c) ZeroC, Inc.

// Extends Program to provide the CancelKeyPressed property.
public static partial class Program
{
    /// <summary>Gets a task that completes when the console receives a Ctrl+C.</summary>
    public static Task CancelKeyPressed => _cancelKeyPressedTcs.Task;

    private static readonly TaskCompletionSource _cancelKeyPressedTcs = new();

    static Program() =>
        Console.CancelKeyPress += (sender, eventArgs) =>
        {
            eventArgs.Cancel = true;
            _ = _cancelKeyPressedTcs.TrySetResult();
        };
}
