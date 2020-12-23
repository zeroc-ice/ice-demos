// Copyright (c) ZeroC, Inc. All rights reserved.

using System.Threading;
using ZeroC.Ice;
using ZeroC.IceBox;

namespace Demo
{
    class HelloService : IService
    {
        private ObjectAdapter? _adapter;

        public void Start(string name, Communicator communicator, string[] args)
        {
            _adapter = communicator.CreateObjectAdapter(name);
            _adapter.Add("hello", new Hello());
            _adapter.ActivateAsync();
        }

        public void Stop() => _adapter?.DisposeAsync();
    }

    public class Hello : IHello
    {
        public void SayHello(Current current, CancellationToken cancel) => System.Console.WriteLine("Hello World!");
    }
}
