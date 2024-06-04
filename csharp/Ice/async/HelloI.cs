// Copyright (c) ZeroC, Inc.

using Demo;

internal class HelloI : HelloDisp_
{
    private readonly CancellationTokenSource _cts;

    public override async Task sayHelloAsync(int delay, Ice.Current current)
    {
        if (delay == 0)
        {
            Console.WriteLine("Hello World!");
        }
        else
        {
            try
            {
                await Task.Delay(delay, _cts.Token);
                Console.WriteLine("Belated Hello World!");
            }
            catch (OperationCanceledException)
            {
                Console.WriteLine("sayHello was canceled by shutdown");
                throw new RequestCanceledException();
            }
        }
    }

    public override void shutdown(Ice.Current current)
    {
        _cts.Cancel();
    }

    internal HelloI(CancellationTokenSource cts)
    {
        _cts = cts;
    }
}
