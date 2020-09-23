// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Threading.Tasks;
using ZeroC.Ice;

namespace Demo
{
    public class Logger : ILogger
    {
        public string Prefix => "";
        public ILogger CloneWithPrefix(string prefix) => new Logger();
        public void Error(string message) => Console.WriteLine($"ERROR: {message}");
        public void Print(string message) => Console.WriteLine($"PRINT: {message}");
        public void Trace(string category, string message) => Console.WriteLine($"TRACE({category}): {message}");
        public void Warning(string message) => Console.WriteLine($"WARNING: {message}");
    }

    public class LoggerPlugin : IPlugin
    {
        public void Initialize(PluginInitializationContext context) => context.Logger = new Logger();
        public ValueTask DisposeAsync() => default;
    }

    public class LoggerPluginFactory : IPluginFactory
    {
        public IPlugin Create(Communicator communicator, string name, string[] args) => new LoggerPlugin();
    }
}
