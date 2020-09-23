// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using ZeroC.Ice;

namespace Demo
{
    public class Hello : IHello
    {
        public void SayHello(int delay, Current current)
        {
            if (delay > 0)
            {
                System.Threading.Thread.Sleep(delay);
            }
            Console.Out.WriteLine("Hello World!");
        }

        public void Shutdown(Current current)
        {
            Console.Out.WriteLine("Shutting down...");
            current.Adapter.Communicator.DisposeAsync();
        }
    }
}
