// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using ZeroC.Ice;

namespace Demo
{
    public class Hello : IHello
    {
        public string RecieveResponse(Current current, global::System.Threading.CancellationToken cancel)
        {
            return "Hi there!";
        }
        public void SayHello(Current current, global::System.Threading.CancellationToken cancel)
        {
            Console.Out.WriteLine("Hello World!");
        }
    }
}
