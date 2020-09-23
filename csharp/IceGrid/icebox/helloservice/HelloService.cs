// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using ZeroC.Ice;
using ZeroC.IceBox;

namespace Demo
{
    class HelloService : IService
    {
        private ObjectAdapter? _adapter;

        public void Start(string name, Communicator communicator, string[] args)
        {
            _adapter = communicator.CreateObjectAdapter($"Hello-{name}");

            string identity = communicator.GetProperty("Hello.Identity") ??
                throw new InvalidOperationException("property `Hello.Identity' was not set");
            string programName = communicator.GetProperty("Ice.ProgramName") ??
                throw new InvalidOperationException("property `Ice.ProgramName' was not set");
            _adapter.Add(identity, new Hello(programName));
            _adapter.Activate();
        }

        public void Stop() => _adapter?.DisposeAsync();
    }

    public class Hello : IHello
    {
        private readonly string _serviceName;

        public Hello(string serviceName) => _serviceName = serviceName;

        public void SayHello(Current current)
        {
            Console.WriteLine(Environment.GetEnvironmentVariable("LANG") ?? "en" switch
            {
                "fr" => $"Bonjour, {_serviceName}",
                "de" => $"Hallo, {_serviceName}",
                "es" => $"Hola, {_serviceName}",
                "it" => $"Ciao, {_serviceName}",
                _ => $"Hello, {_serviceName}"
            });
        }
    }

}
