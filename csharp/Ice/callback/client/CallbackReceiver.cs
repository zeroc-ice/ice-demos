// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using ZeroC.Ice;

namespace Demo
{
    public sealed class CallbackReceiver : ICallbackReceiver
    {
        public void Callback(Current current) => Console.WriteLine("received callback");
    }
}
