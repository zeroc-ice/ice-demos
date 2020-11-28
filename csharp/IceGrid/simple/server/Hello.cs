// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Threading;
using ZeroC.Ice;

namespace Demo
{
    public class Hello : IHello
    {
        private readonly string _name;
        public Hello(string name) => _name = name;

        public void SayHello(Current current, CancellationToken cancel) =>
            Console.WriteLine($"{_name} says Hello World!");

        public void Shutdown(Current current, CancellationToken cancel)
        {
            Console.WriteLine($"{_name} shutting down...");
            current.Communicator.ShutdownAsync();
        }
    }
}
