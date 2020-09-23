// Copyright (c) ZeroC, Inc. All rights reserved.

using System.Threading.Tasks;
using ZeroC.Ice;

namespace Demo
{
    public class Hello : IHello
    {
        public void SayHello(Current current) =>
            current.Communicator.Logger.Print("Hello World!");

        public void Shutdown(Current current)
        {
            current.Communicator.Logger.Print("Shutting down...");
            current.Communicator.DisposeAsync();
        }
    }

    public class HelloPlugin : IPlugin
    {

        public void Initialize(PluginInitializationContext context)
        {
            ObjectAdapter adapter = context.Communicator.CreateObjectAdapter("Hello");
            adapter.Add("hello", new Hello());
            adapter.Activate();
        }

        public ValueTask DisposeAsync() => default;
    }

    public class HelloPluginFactory : IPluginFactory
    {
        public IPlugin Create(Communicator communicator, string name, string[] args) => new HelloPlugin();
    }
}
