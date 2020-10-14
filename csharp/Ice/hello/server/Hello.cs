// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using ZeroC.Ice;

namespace Demo
{
    public class Hello : IHello
    {
        public void SayHello(Current current, global::System.Threading.CancellationToken cancel)
        {
            Console.Out.WriteLine("Hello World!");
        }
    }
    public class Hi : IHi
    {
        public void SayHi(Current current, global::System.Threading.CancellationToken cancel)
        {
            Console.Out.WriteLine("Hi");
        }
    }
}
