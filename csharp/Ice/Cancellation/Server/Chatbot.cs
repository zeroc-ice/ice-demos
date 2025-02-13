// Copyright (c) ZeroC, Inc.

using VisitorCenter;

namespace Server;

/// <summary>Chatbot is an Ice servant that implements Slice interface Greeter.</summary>
internal class Chatbot : GreeterDisp_, IDisposable
{
    private bool _disposed;
    private readonly object _mutex = new(); // protects _tcs and _disposed
    private TaskCompletionSource? _tcs; // not null when the chatbot is paused.

    /// <inheritdoc/>
    public override async Task<string> GreetAsync(string name, Ice.Current current)
    {
        Console.WriteLine($"Dispatching greet request {{ name = '{name}' }}");
        await WaitUntilResumedAsync();

        Console.WriteLine($"Returning response to '{name}'");
        return $"Hello, {name}!";
    }

    /// <summary>Resumes this chatbot if it is paused, and prevents more pauses.</summary>
    public void Dispose()
    {
        lock (_mutex)
        {
            if (_disposed)
            {
                return;
            }
            _disposed = true;
            _tcs?.TrySetResult();
            _tcs = null;
        }
    }

    /// <summary>Pauses the chatbot.</summary>
    internal void Pause()
    {
        lock (_mutex)
        {
            // The chatbot was resumed but _tcs was not cleared yet, so we clear it.
            if (_tcs?.Task.IsCompleted ?? false)
            {
                _tcs = null;
            }

            if (!_disposed)
            {
                // We run the continuation asynchronously because we set this TaskCompletionSource with _mutex locked.
                _tcs ??= new TaskCompletionSource(TaskCreationOptions.RunContinuationsAsynchronously);
            }
        }
    }

    /// <summary>Resumes the chatbot.</summary>
    internal void Resume()
    {
        lock (_mutex)
        {
            _tcs?.TrySetResult(); // WaitUntilResumedAsync() will clear _tcs
        }
    }

    /// <summary>Waits for the chatbot to be resumed.</summary>
    private async Task WaitUntilResumedAsync()
    {
        Task? task = null;

        lock (_mutex)
        {
            task = _tcs?.Task;
        }

        if (task is not null)
        {
            // Wait until Resume completes the task.
            await task;
            lock (_mutex)
            {
                _tcs = null;
            }
        }
    }
}
