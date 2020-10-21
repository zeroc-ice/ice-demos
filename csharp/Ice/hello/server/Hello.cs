// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Threading;
using ZeroC.Ice;

namespace Demo
{
    public class Hello : IHello
    {
        public string SayHello(Current current, CancellationToken cancel, string[] greeting)
        {
            Console.Out.WriteLine("Hello World!");
            if (greeting.Length > 0)
            {
            return greeting;
            }
        }
    }
}
