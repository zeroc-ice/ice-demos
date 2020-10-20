// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using ZeroC.Ice;

namespace Demo
{
    public class Hello : IHello
    {
        public string recieveResponse(Current current, CancellationToken cancel)
        {
            return "Hi there!";
        }
        public void SayHello(Current current, CancellationToken cancel)
        {
            Console.Out.WriteLine("Hello World!");
        }
    }
}
