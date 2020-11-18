// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Threading;
using ZeroC.Ice;

namespace Demo
{
    public class Context : IContext
    {
        public void Call(Current current, CancellationToken cancel)
        {
            if (current.Context.TryGetValue("type", out string? value))
            {
                Console.WriteLine($"Type = {value}");
            }
            else
            {
                Console.WriteLine("Type = None");
            }
        }

        public void Shutdown(Current current, CancellationToken cancel)
        {
            Console.WriteLine("Shutting down...");
            current.Adapter.Communicator.DisposeAsync();
        }
    }
}
