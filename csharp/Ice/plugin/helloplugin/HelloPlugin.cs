// Copyright (c) ZeroC, Inc. All rights reserved.

using System.Threading;
using System.Threading.Tasks;
using ZeroC.Ice;

namespace Demo
{
    public class Hello : IHello
    {
        public void SayHello(Current current, CancellationToken cancel) =>
            current.Communicator.Logger.Print("Hello World!");

        public void Shutdown(Current current, CancellationToken cancel)
        {
            current.Communicator.Logger.Print("Shutting down...");
            _ = current.Communicator.ShutdownAsync();
        }
    }

    public class HelloPlugin : IPlugin
    {
        private Communicator _communicator;

        public async Task ActivateAsync(CancellationToken cancel)
        {
            ObjectAdapter adapter = _communicator.CreateObjectAdapter("Hello");
            adapter.Add("hello", new Hello());
            await adapter.ActivateAsync(cancel);
        }

        public ValueTask DisposeAsync() => default;

        internal HelloPlugin(Communicator communicator) => _communicator = communicator;
    }

    public class HelloPluginFactory : IPluginFactory
    {
        public IPlugin Create(Communicator communicator, string name, string[] args) => new HelloPlugin(communicator);
    }
}
