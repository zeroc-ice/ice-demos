// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using ZeroC.Ice;

namespace Demo
{
    public class Hello : IHello
    {
        private readonly string _name;
        public Hello(string name) => _name = name;

        public void SayHello(Current current) => Console.WriteLine($"{_name} says Hello World!");

        public void Shutdown(Current current)
        {
            Console.WriteLine($"{_name} shutting down...");
            current.Communicator.DisposeAsync();
        }
    }
}
