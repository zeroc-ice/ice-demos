// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Threading.Tasks;
using ZeroC.Ice;

namespace Demo
{
    public sealed class CallbackReceiver : ICallbackReceiver
    {
        public void Callback(Current current, CancellationToken cancel) => Console.WriteLine("received callback");
    }
}
