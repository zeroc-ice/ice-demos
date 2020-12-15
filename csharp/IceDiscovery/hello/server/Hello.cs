// Copyright (c) ZeroC, Inc. All rights reserved.

using System.Threading;
using ZeroC.Ice;

namespace Demo
{
    public class Hello : IHello
    {
        public void SayHello(int delay, Current current, CancellationToken cancel)
        {
            if (delay > 0)
            {
                System.Threading.Thread.Sleep(delay);
            }
            System.Console.Out.WriteLine("Hello World!");
        }

        public void Shutdown(Current current, CancellationToken cancel)
        {
            System.Console.Out.WriteLine("Shutting down...");
            current.Communicator.DisposeAsync();
        }
    }
}
