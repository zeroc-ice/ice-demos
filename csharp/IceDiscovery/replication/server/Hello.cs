// Copyright (c) ZeroC, Inc. All rights reserved.

using ZeroC.Ice;

namespace Demo
{
    public class Hello : IHello
    {
        private readonly string _name;

        public Hello(string name) => _name = name;

        public string GetGreeting(Current current) => $"{_name} says Hello World!";

        public void Shutdown(Current current)
        {
            System.Console.Out.WriteLine("Shutting down...");
            current.Communicator.DisposeAsync();
        }
    }

}
