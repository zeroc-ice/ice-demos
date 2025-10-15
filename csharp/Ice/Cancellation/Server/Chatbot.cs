// Copyright (c) ZeroC, Inc.

using VisitorCenter;

namespace Server;

/// <summary>Chatbot is an Ice servant that implements Slice interface Greeter.</summary>
internal class Chatbot : AsyncGreeterDisp_
{
    private readonly TimeSpan _delay;
    private readonly CancellationToken _cancellationToken;

    /// <inheritdoc/>
    public override async Task<string> GreetAsync(string name, Ice.Current current)
    {
        Console.WriteLine($"Dispatching greet request {{ name = '{name}' }}");

        try
        {
            await Task.Delay(_delay, _cancellationToken);
        }
        catch (OperationCanceledException)
        {
            Console.WriteLine($"greet dispatch canceled {{ name = '{name}' }}");
            throw;
        }

        return $"Hello, {name}!";
    }

    /// <summary>Creates a new Chatbot instance.</summary>
    /// <param name="delay">The delay before returning each greeting.</param>
    /// <param name="cancellationToken">A cancellation token that triggers the cancellation of all dispatches.</param>
    public Chatbot(TimeSpan delay, CancellationToken cancellationToken)
    {
        _delay = delay;
        _cancellationToken = cancellationToken;
    }
}
